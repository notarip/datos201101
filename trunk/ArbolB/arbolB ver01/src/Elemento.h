/*
 * Elemento.h
 *
 *  Created on: 19/03/2011
 *      Author: pablo
 */


#ifndef ELEMENTO_H_
#define ELEMENTO_H_

#include "Definiciones.h"


#pragma pack (1)


class Elemento {
public:
	Elemento();
	~Elemento();
	unsigned int clave;
	char dato;
};

#endif /* ELEMENTO_H_ */
