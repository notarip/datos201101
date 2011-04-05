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

class ArchivoLibros {
public:
	ArchivoLibros(string path);
	virtual ~ArchivoLibros();

	void agregarLibro(Libro* unLibro);
	void suprimirLibro(unsigned int id);
	Libro* recuperarLibro(unsigned int id); // ¿se recupera por id o por offset?
	list<Libro>* recuperacionComprensiva();

	void reestructurar();



};

#endif /* ARCHIVOLIBROS_H_ */
