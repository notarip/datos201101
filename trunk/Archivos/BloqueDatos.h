/*
 * Bloque.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#ifndef BLOQUEDATOS_H_
#define BLOQUEDATOS_H_

#include <list>
#include "Bloque.h"
#include "ElementoBloqueDatos.h"

class BloqueDatos :public Bloque {
public:
	BloqueDatos(unsigned int tamanio); //tamaño en bytes
	virtual ~BloqueDatos();
	void agregarElemento(ElementoBloqueDatos unElemento);
	ElementoBloqueDatos* recuperarElemento(unsigned int id);
	unsigned int getEspacioLibre();



private:
	list<ElementoBloqueDatos> listaElementos;
	unsigned int espacioLibre;


};

#endif /* BLOQUEDATOS_H_ */
