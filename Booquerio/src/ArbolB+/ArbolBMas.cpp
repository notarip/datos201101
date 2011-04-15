/*
 * ArbolBMas.cpp
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(string path, int tipoClave) {
	// TODO Auto-generated constructor stub

}

resultadoOperacion* ArbolBMas::buscar(string clave, Registro* regEncontrado){
	list<Registro>* listaReg= raiz->obtenerRegistros();
	if (listaReg==NULL){
		regEncontrado= NULL;
		//return resultadoOp: Arbol Vacio
	}
	list<Registro>::iterator itReg= listaReg->begin();

	while (itReg->getString()<=clave && itReg!=listaReg->end()) {
		regEncontrado= &(*itReg);
		itReg++;
	}
	if (itReg==listaReg->end()) {
		//Buscar interno con ref derecha
	}
	else {
		//Buscar Interno con ref izquierda
	}
}

ArbolBMas::~ArbolBMas() {
	// TODO Auto-generated destructor stub
}
