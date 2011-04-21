/*
 * ArbolBMasAlfabetico.cpp
 *
 *  Created on: 15/04/2011
 *      Author: martin
 */

#include "ArbolBMasAlfabetico.h"

ArbolBMasAlfabetico::ArbolBMasAlfabetico(string path,
		unsigned int tamanioBloque) :
	ArbolBMas(path, tamanioBloque) {

}

char ArbolBMasAlfabetico::compareRegistros(string clave, Registro* unRegistro){
	string claveRegistro = unRegistro->getString();

	if (clave < claveRegistro) return -1;
	if (clave == claveRegistro) return 0;
	if (clave > claveRegistro) return 1;
}

Registro* ArbolBMasAlfabetico::crearRegistroClave(string clave){
	return new Registro(clave);
}

string ArbolBMasAlfabetico::consultarClave(Registro* unRegistro){
	return unRegistro->getString();
}

ArbolBMasAlfabetico::~ArbolBMasAlfabetico() {

}
