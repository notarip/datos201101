/*
 * ElementoBloque.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#ifndef ELEMENTOBLOQUEDATOS_H_
#define ELEMENTOBLOQUEDATOS_H_

#include <list>
#include <string>
using namespace std;

class ElementoBloqueDatos {
public:
	ElementoBloqueDatos();
	virtual ~ElementoBloqueDatos();

	string getIndiceSec();
	bool buscarIdentificador();
	list<unsigned int>* getIdentificadores();


private:
	string indiceSec;
	list<unsigned int> identificadores;
	unsigned short int posicionIdsExtras;



};

#endif /* ELEMENTOBLOQUEDATOS_H_ */
