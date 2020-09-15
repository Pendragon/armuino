
#ifndef __BINARYINPUT_H__
#define __BINARYINPUT_H__

#include "Arduino.h"

/*
 * Binary Input est une classe permettant de gerer une entree digitale
 *
 * Chaque changement d'état est detecté. Il est possible de gerer de veritables entrees, ou 
 * des entress virtuelles qui ne sont pas associées à des IO. 
 */
class BinaryInput {
private:
	bool 	real_port = true;
	int 	previous;
	int 	port;
	
public:
	int 	raised;
	int 	dropped;
	int 	current;
    bool 	changed = false;
	bool	inverse = false;
	unsigned long	lastChange;

			BinaryInput(int _port = -1, int mode = INPUT, bool _inverse = false);

	void 	set(int new_value);    
	void 	read(bool inverse = false);
    void 	update(unsigned long currentMillis);
    void 	reset(void);
	unsigned long	duration(void);
};


#endif /* __BINARYINPUT_H__ */
