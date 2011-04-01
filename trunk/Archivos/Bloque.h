/*
 * Bloque.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#ifndef BLOQUE_H_
#define BLOQUE_H_
#include <string>

using namespace std;

class Bloque {
public:
	Bloque();
	virtual ~Bloque();
	virtual string Serializar()=0;  // faltaria definir como se devuelve la tira de bytes
};

#endif /* BLOQUE_H_ */
