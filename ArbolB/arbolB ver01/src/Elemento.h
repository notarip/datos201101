/*
 * Elemento.h
 *
 *  Created on: 19/03/2011
 *      Author: pablo
 */


#ifndef ELEMENTO_H_
#define ELEMENTO_H_

#include "Definiciones.h"

class Elemento {
public:
	Elemento();
	virtual ~Elemento();
	#pragma pack(push,1)
	unsigned int clave;
//	char dato;
};

#endif /* ELEMENTO_H_ */
