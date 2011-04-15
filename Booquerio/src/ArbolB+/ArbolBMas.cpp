/*
 * ArbolBMas.cpp
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(string path, float ocupacion,unsigned int tamanioBloque) {
	this->ocupacionNodo = ocupacion;
	this->archivoNodos = new ArchivoBloques(path, tamanioBloque);

	this->tamanioNodo = tamanioBloque;
	ultimaHojaVisitada = NULL;
	ultimoValorBuscado = NULL;

	raiz = new Bloque();

	archivoNodos->grabarBloque(raiz, 0);

}

resultadoOperacion* ArbolBMas::insertar(string clave, unsigned int valor){return 0;}

resultadoOperacion* ArbolBMas::eliminar(string clave, unsigned int valor){return 0;}

resultadoOperacion* ArbolBMas::siguiente(Registro* regSiguiente){return 0;}

resultadoOperacion* ArbolBMas::buscarBloque(string clave,
		Bloque* bloqueEncontrado) {
	list<Registro>* listaReg = raiz->obtenerRegistros();

	//si la raiz es una hoja (nivel 0) => solo puede estar ahi
	if (raiz->getAtributoBloque() == 0) {
		bloqueEncontrado = raiz;
		//return resultadoOp: encontre un bloque donde puede estar
	}

	// itero sobre los registros de la raiz
	list<Registro>::iterator itReg = listaReg->begin();

	//hasta encontrar uno mayor
	while (compareRegistros(clave, &(*itReg)) < 0 && itReg != listaReg->end()) {
		itReg++;
	}

	//si llegaste al fin de la lista sin encontra un mayor => bajo por derecha
	if (itReg == listaReg->end()) {
		return this->buscarBloqueRecursivo(clave, (*itReg).getReferenciai(2),
				bloqueEncontrado);
	}

	//sino bajo por la izquierda del mayor
	return this->buscarBloqueRecursivo(clave, (*itReg).getReferenciai(1),
			bloqueEncontrado);
}

resultadoOperacion* ArbolBMas::buscarBloqueRecursivo(string clave,
		unsigned int refBloque, Bloque* bloqueEncontrado) {

	//bajo el bloque a mem
	Bloque* esteBloque = archivoNodos->recuperarBloque(refBloque);

	//si se trata de un nodo Raiz
	if (esteBloque->getAtributoBloque() == 0) {
		bloqueEncontrado = esteBloque;
		this->ultimaHojaVisitada = refBloque;
		//return resultadoOperacion

	//si es un nodo Interno
	} else {
		list<Registro>* listaRegistros = esteBloque->obtenerRegistros();

		//itero por los registro del bloque
		list<Registro>::iterator itRegistros = listaRegistros->begin();
		//hasta encontrar uno mayor
		while (compareRegistros(clave, &(*itRegistros)) < 0 && itRegistros
				!= listaRegistros->end()) {
			itRegistros++;
		}
		//si llegaste al fin de la lista sin encontra un mayor => bajo por DERECHA
		if (itRegistros == listaRegistros->end()) {
			return this->buscarBloqueRecursivo(clave,
					(*itRegistros).getReferenciai(2), bloqueEncontrado);
		}

		//sino bajo por la IZQUIERDA del mayor
		return this->buscarBloqueRecursivo(clave,
				(*itRegistros).getReferenciai(1), bloqueEncontrado);
	}

}

ArbolBMas::~ArbolBMas() {
	delete this->archivoNodos;
	delete raiz;
}


