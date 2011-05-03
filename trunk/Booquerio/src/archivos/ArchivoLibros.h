/*
 * ArchivoLongVariable.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#ifndef ARCHIVOLIBROS_H_
#define ARCHIVOLIBROS_H_

#include "Libro.h"
#include <fstream>
#include <list>
#include <string.h>
#include <iostream>
using namespace std;

class ArchivoLibros {
public:
	ArchivoLibros(string path);
	virtual ~ArchivoLibros();

	unsigned int agregarLibro(Libro* unLibro);
	void suprimirLibro(unsigned int id);
	void serializar(Libro* unLibro,char** tiraBytes);
	void deserializar(char* tiraBytes, Libro** unLibro);
	Libro* recuperarLibro(unsigned int id); // ¿se recupera por id o por offset?

	list<Libro>* recuperacionComprensiva();

	void reestructurar(); //como lo hago?? Si no tengo los espacios vacios.//

private:
	string path;
	unsigned int longReg;
	unsigned int obtenerOffset(unsigned int id);
	char* levantarRegistro(unsigned int id);

};

#endif /* ARCHIVOLIBROS_H_ */
