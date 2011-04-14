/*
 * Bloque.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#ifndef BLOQUE_H_
#define BLOQUE_H_
#include <string>
#include <list>
#include "Registro.h"

using namespace std;

class Bloque {
public:
	Bloque();
	Bloque(Registro unRegistro);
	virtual ~Bloque();
	void agregarRegistro(Registro unRegistro);
	Registro* recuperarRegistro(unsigned int id);
	Registro* recuperarRegistro(string palabra);
	list<Registro>* obtenerRegistros();
	void setSiguiente(unsigned int siguiente);
	unsigned int getSiguiente();
	void setAtributoBloque(unsigned int atributoBloque);
	unsigned int getAtributoBloque();

private:
	list<Registro> listaRegistros;
	unsigned int siguiente;
	unsigned int atributoBloque;
};

#endif /* BLOQUE_H_ */
