/*
 * Binary Input est une classe permettant de gerer une entree digitale
 *
 * Chaque changement d'état est detecté. Il est possible de gerer de veritables d'entrees, ou 
 * des entress virtuelles qui ne sont pas associées à des IO. 
 */
//#include "Arduino.h"
#include ".\BinaryInput.h"


/*
 * Constructeur de la classe. L'objet est creer dans la mémoire et initialise
 */
BinaryInput::BinaryInput(int _port, int mode, bool _inverse) 
{
	if (_port == -1) // Simple stockage de donnée binaire, pas de lecteur hardware
			real_port = false;
	else {
		pinMode(_port, mode);
		port = _port;
		real_port = true;      
	}
	inverse = _inverse;
	reset();
}


/*
 * Mise à jour de la valeur, avec detection des fronts montants et descendants. 
 */
void BinaryInput::set(int new_value) 
{
	previous = current;
	current = new_value;
	changed = false;

	if (current) 
	{
		if (previous == LOW) {
			raised = HIGH;
	  		changed = true;
		}
		else 
	  		raised = LOW;
	} 
	else 
	{
		if (previous == HIGH) {
			changed = true;
			dropped = HIGH;
		}
		else
	  		dropped = LOW;
	}

	if (changed)
		lastChange = millis();
}

/*
 * Retourne le nombre de millisecondes depuis le début de l'état courrant
 */
unsigned long BinaryInput::duration(void)
{
	return (millis() - lastChange);
}

/*
 * Lecture de la valeur
 */
void BinaryInput::update(unsigned long currentMillis)
{
    read(inverse);
}

/*
 * Si le port est physique, lecture de la valeure 
 */
void BinaryInput::read(bool inverse) 
{
	// Ne lire les valeurs hardware que si c'est un port physique
	if (real_port) {
		if (inverse)
			set(!digitalRead(port));
		else
			set(digitalRead(port));
	}
}


/*
 * Remise à zero des valeurs
 */
void BinaryInput::reset(void) 
{
	previous = LOW;
	current = LOW;
	raised = LOW;
	dropped = LOW;
	lastChange = millis();
}