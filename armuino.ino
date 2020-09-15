#include "./AccelStepper.h"
#include "./BinaryInput.h"
#include "./pins.h"
#include "./config.h"

int status = STATUS_READY;
int lu = 0;
char buffer[READ_BUFFER_SIZE];

Axis target;
Axis endStops;

BinaryInput axeBendStop(B_PIN_ENDSTOP, INPUT_PULLUP);
BinaryInput axeXendStop(X_PIN_ENDSTOP, INPUT_PULLUP);
BinaryInput axeYendStop(Y_PIN_ENDSTOP, INPUT_PULLUP);

AccelStepper stepperB(AccelStepper::DRIVER, B_PIN_PULSE, B_PIN_DIR);
AccelStepper stepperX(AccelStepper::DRIVER, X_PIN_PULSE, X_PIN_DIR);
AccelStepper stepperY(AccelStepper::DRIVER, Y_PIN_PULSE, Y_PIN_DIR);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_PIN_PULSE, Z_PIN_DIR);

/*
 * Starts a reset motion.
 * All axis max speed are reduced to avoid end stop destruction
 */
void startReset()
{
    endStops.B = 0;
    endStops.X = 0;
    endStops.Y = 0;
    status = STATUS_BOOT;
    stepperB.setMaxSpeed(MAX_SPEED / 8);
    stepperX.setMaxSpeed(MAX_SPEED / 10);
    stepperY.setMaxSpeed(MAX_SPEED / 10);
}

/*
 * Reset all axis
 * Search the endstop then move to the idle position
 */
void reset()
{
    // B axis
    if (axeBendStop.current && endStops.B == 0)
    {
        endStops.B = 1;
        stepperB.setCurrentPosition(B_MAX);
        stepperB.setMaxSpeed(MAX_SPEED);
        stepperB.moveTo(B_IDLE);
    }
    else
    {
        if (endStops.B == 0 && stepperB.distanceToGo() == 0)
            stepperB.move(10000);
        if (endStops.B == 1 && stepperB.distanceToGo() == 0)
            endStops.B = 2;
    }

    // X
    if (axeXendStop.current && endStops.X == 0)
    {
        endStops.X = 1;
        stepperX.setCurrentPosition(X_MIN);
        stepperX.setMaxSpeed(MAX_SPEED);
        stepperX.moveTo(X_IDLE);
    }
    else
    {
        if (endStops.X == 0 && stepperX.distanceToGo() == 0)
            stepperX.move(-10000);
        if (endStops.X == 1 && stepperX.distanceToGo() == 0)
            endStops.X = 2;
    }

    // Y
    if (axeYendStop.current && endStops.Y == 0)
    {
        endStops.Y = 1;
        stepperY.setCurrentPosition(Y_MIN);
        stepperY.setMaxSpeed(MAX_SPEED);
        stepperY.moveTo(Y_IDLE);
    }
    else
    {
        if (endStops.Y == 0 && stepperY.distanceToGo() == 0)
            stepperY.move(-10000);
        if (endStops.Y == 1 && stepperY.distanceToGo() == 0)
            endStops.Y = 2;
    }

    // Exit the mode when all axis are reset
    if (endStops.B == 2 && endStops.X == 2 && endStops.Y == 2)
        status = STATUS_READY;
}

/*
 * Initialize application
 */
void setup() 
{
    SERIAL.begin(9600);
    SERIAL.flush();
    Update();
    Update();
    stepperB.setAcceleration(ACCEL);
    stepperB.setMaxSpeed(MAX_SPEED);
    stepperX.setAcceleration(ACCEL);
    stepperX.setMaxSpeed(MAX_SPEED);
    stepperY.setAcceleration(ACCEL);
    stepperY.setMaxSpeed(MAX_SPEED);
    stepperZ.setAcceleration(ACCEL);
    stepperZ.setMaxSpeed(MAX_SPEED);
}

/*
 * Update sensors values
 */
bool Update(void)
{
    unsigned long current = millis();
    axeBendStop.update(current);
    axeXendStop.update(current);
    axeYendStop.update(current);
}

/*
 * Parse serial command 
 */
bool parse(void)
{
    String toAnalyze(buffer);

    if (toAnalyze.startsWith(WORD_MOVE))
    {
        int espace = toAnalyze.indexOf(' ');
        int virgule1 = toAnalyze.indexOf(',');

        if (espace == -1 || virgule1 == -1)
            return false;
        int virgule2 = toAnalyze.indexOf(',', virgule1 + 1);
        int virgule3 = toAnalyze.lastIndexOf(',');
        if (virgule2 == -1 || virgule3 == -1 || virgule2 == virgule3 || virgule2 == virgule1)
            return false;

        String paramB = toAnalyze.substring(espace + 1, virgule1);
        String paramX = toAnalyze.substring(virgule1 + 1, virgule2);
        String paramY = toAnalyze.substring(virgule2 + 1, virgule3);
        String paramZ = toAnalyze.substring(virgule3 + 1);

        target.B = paramB.toInt();
        target.X = paramX.toInt();
        target.Y = paramY.toInt();
        target.Z = paramZ.toInt();

        stepperB.move(target.B);
        stepperX.move(target.X);
        stepperY.move(target.Y);
        stepperZ.move(target.Z);

        SERIAL.println(stepperB.distanceToGo());
            
        return true;
    }
    if (toAnalyze.startsWith(WORD_RESET))
    {
        startReset();
        return true;
    }
    if (toAnalyze.startsWith(WORD_OPEN))
    {
        return true;
    }
    if (toAnalyze.startsWith(WORD_CLOSE))
    {
        return true;
    }
    if (toAnalyze.startsWith(WORD_STATUS))
    {
        SERIAL.println("Endstops:");
        SERIAL.print("  B: ");SERIAL.println(axeBendStop.current); 
        SERIAL.print("  X: ");SERIAL.println(axeXendStop.current); 
        SERIAL.print("  Y: ");SERIAL.println(axeYendStop.current); 
        SERIAL.println("\nPositions:");
        SERIAL.print("  B: ");SERIAL.println(stepperB.currentPosition()); 
        SERIAL.print("  X: ");SERIAL.println(stepperX.currentPosition()); 
        SERIAL.print("  Y: ");SERIAL.println(stepperY.currentPosition()); 
        SERIAL.print("  Z: ");SERIAL.println(stepperZ.currentPosition()); 
        return true;
    }
    if (toAnalyze.startsWith(WORD_HELP))
    {
        SERIAL.println("Command are the following:");
        SERIAL.println("  " WORD_MOVE " b,x,y,z\t\tMove the arm in the given b,x,y,z location");
        SERIAL.println("  " WORD_OPEN "\t\tOpens the hand"); 
        SERIAL.println("  " WORD_CLOSE "\t\tClose the hand");
        SERIAL.println("  " WORD_RESET "\t\tRecalibrate the arm");
        SERIAL.println("  " WORD_STATUS "\t\tPrint all status");
        return true;
    }
    return false;
}


void loop() 
{
    Update();
    
    /*
     * Retrieve character to read on SERIAL if any
     */
    if (status==STATUS_READY && lu < READ_BUFFER_SIZE && SERIAL.available())
    {
        char justRead = SERIAL.read();
        if (justRead == 13 || justRead == 10)
        {
            if (parse())
                SERIAL.println("OK");
            else
            {
                SERIAL.print("BAD ");
                SERIAL.println(buffer);
            }
            for (int i=0;i<READ_BUFFER_SIZE;i++)
                buffer[i] = 0;
            lu = 0;            
        }
        else
            buffer[lu++] = justRead;

    }

    if (status==STATUS_BOOT)
    {
        reset();
    }
    
    /*
     * Depending on status, move axis
     */ 

    /*
     * Write axis and status to SERIAL
     */
     stepperB.run();
     stepperX.run();
     stepperY.run();
     stepperZ.run();
}
