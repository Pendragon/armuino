/*
 *
 */

/*
 * Neo PIXEL strip 
 */
#define SERIAL  Serial1

/*
 * Neo PIXEL strip 
 */
#define NEOPIXELS   2

/*
 * B axis 
 * Base centere vertical axis
 */
#define B_PIN_DIR       5
#define B_PIN_PULSE     9
#define B_PIN_ENDSTOP   24 
#define B_MIN       -7200
#define B_MAX       7200
#define B_IDLE      0

/*
 * X axis 
 * Shoulder axis
 */
#define X_PIN_DIR       4
#define X_PIN_PULSE     8
#define X_PIN_ENDSTOP   26
#define X_MIN           -4400
#define X_MAX           4400
#define X_IDLE          0

/*
 * Y axis 
 * Elbow axis
 */
#define Y_PIN_DIR       7
#define Y_PIN_PULSE     11
#define Y_PIN_ENDSTOP   22 //6
#define Y_MIN           -1250
#define Y_MAX           2500
#define Y_IDLE          0
    
/*
 * Z axis 
 * Rist axis
 */
#define Z_PIN_DIR       6
#define Z_PIN_PULSE     10
#define Z_PIN_ENDSTOP
