/*
 * Registro.h
 *
 *  Created on: 05/04/2011
 *      Author: rjc
 */

#ifndef REGISTRO_H_
#define REGISTRO_H_

#include <list>
#include <string>
using namespace std;

class Registro {

public:
  Registro(string unString);
	Registro(unsigned int id, unsigned short int unaReferencia);
	Registro (string unString, unsigned int id);
	virtual ~Registro();

	string getString();
	bool buscarID(unsigned int idBuscado);
	list<unsigned int>* getIdentificadores();
	list<unsigned short int>* getReferencias();
	void agregarId (unsigned int id);
	void agregarReferencia(unsigned short int);



private:
	// trate de hacerlo lo mas generico posible, un string, n ids y n referencias.
	// Por mas que la referencia sea 1 sola.
	string miString;
	list<unsigned int> identificadores;
	list<unsigned short int> referencias;

};

#endif /* REGISTRO_H_ */
