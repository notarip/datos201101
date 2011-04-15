/*
 * ArbolBMasNumerico.cpp
 *
 *  Created on: 15/04/2011
 *      Author: martin
 */

#include "ArbolBMasNumerico.h"

ArbolBMasNumerico::ArbolBMasNumerico(string path, float ocupacion,
		unsigned int tamanioBloque) :
	ArbolBMas(path, ocupacion, tamanioBloque) {

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

resultadoOperacion* ArbolBMasNumerico::insertarNum(unsigned int clave,
		unsigned int valor) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	insertar(claveAlfabetica, valor);

}

resultadoOperacion* ArbolBMasNumerico::buscarBloqueNum(unsigned int clave,
		Bloque* bloqueEncontrado) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	buscarBloque(claveAlfabetica, bloqueEncontrado);
}

resultadoOperacion* ArbolBMasNumerico::eliminarNum(unsigned int clave,
		unsigned int valor) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	eliminar(claveAlfabetica, valor);

}

resultadoOperacion* ArbolBMasNumerico::siguienteNum(Registro* regSiguiente) {

	siguiente(regSiguiente);

}

ArbolBMasNumerico::~ArbolBMasNumerico() {

}

