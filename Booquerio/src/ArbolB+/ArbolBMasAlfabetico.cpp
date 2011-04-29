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

Registro* ArbolBMasAlfabetico::crearRegistroClave(string clave, unsigned int valor){
	Registro* unRegistro= new Registro();
	unRegistro->setString(clave);
	unRegistro->agregarReferencia(valor);
	return unRegistro;
}

Registro* ArbolBMasAlfabetico::crearRegistroClave(string clave){
	return new Registro(clave);
}

void ArbolBMasAlfabetico::setearClave(Registro* registroAModificar, string clave) {
	registroAModificar->setString(clave);
}

string ArbolBMasAlfabetico::consultarClave(Registro* unRegistro){
	return unRegistro->getString();
}

ArbolBMasAlfabetico::~ArbolBMasAlfabetico() {

}
