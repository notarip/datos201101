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
	Registro();
	Registro(string unString);
	Registro(unsigned int atribEntero, unsigned int unaReferencia);
	Registro (string unString, unsigned int atribEntero);
	virtual ~Registro();

	string getString();
	bool buscarAtribEntero(unsigned int atribBuscado);
	list<unsigned int>* getAtributosEnteros();
	list<unsigned int>* getReferencias();
	void agregarAtribEntero (unsigned int atribEntero);
	void agregarReferencia(unsigned int);
	unsigned int getReferenciai(unsigned int i);




private:
	// trate de hacerlo lo mas generico posible, un string, n atribEnteros y n referencias.
	// Por mas que la referencia sea 1 sola.
	string miString;
	list<unsigned int> atribEnteros;
	list<unsigned int> referencias;

};

#endif /* REGISTRO_H_ */

