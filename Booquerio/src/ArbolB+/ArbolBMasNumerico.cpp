/*
 * ArbolBMasNumerico.cpp
 *
 *  Created on: 15/04/2011
 *      Author: martin
 */

#include "ArbolBMasNumerico.h"

ArbolBMasNumerico::ArbolBMasNumerico(string path,
		unsigned int tamanioBloque) :
	ArbolBMas(path, tamanioBloque) {

}

char ArbolBMasNumerico::compareRegistros(string clave, Registro* unRegistro) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	unsigned int claveNumerica;
	buffer >> claveNumerica;

	if (claveNumerica < unRegistro->getAtributosEnteros()->front())
		return -1;
	if (claveNumerica == unRegistro->getAtributosEnteros()->front())
		return 0;
	if (claveNumerica > unRegistro->getAtributosEnteros()->front())
		return 1;
}

Registro* ArbolBMasNumerico::crearRegistroClave(string clave) {
	stringstream buffer(stringstream::in | stringstream::out);

	buffer << clave;
	unsigned int claveNumerica;
	buffer >> claveNumerica;
	Registro* unRegistro = new Registro();
	unRegistro->agregarAtribEntero(claveNumerica);

	return unRegistro;
}

void ArbolBMasNumerico::setearClave(Registro* registroAModificar, string clave){

	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	unsigned int claveNumerica;
	buffer >> claveNumerica;
	registroAModificar->getAtributosEnteros()->pop_front();
	registroAModificar->getAtributosEnteros()->push_front(claveNumerica);
}

resultadoOperacion* ArbolBMasNumerico::insertarNumerico(unsigned int clave,
		unsigned int valor) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	return insertar(claveAlfabetica, valor);


}

Registro* ArbolBMasNumerico::buscarRegistroNumerico(unsigned int clave,
		resultadoOperacion* resultOperacion) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	return buscarRegistro(claveAlfabetica, resultOperacion);
}

resultadoOperacion* ArbolBMasNumerico::eliminarNumerico(unsigned int clave,
		unsigned int valor) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	return eliminar(claveAlfabetica, valor);
}

string ArbolBMasNumerico::consultarClave(Registro* unRegistro){
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << unRegistro->getAtributosEnteros()->front();
	string claveAlfabetica;
	buffer >> claveAlfabetica;

	return claveAlfabetica;



}

ArbolBMasNumerico::~ArbolBMasNumerico() {

}

