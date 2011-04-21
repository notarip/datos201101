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

resultadoOperacion* ArbolBMasNumerico::insertarNumerico(unsigned int clave,
		unsigned int valor) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	return insertar(claveAlfabetica, valor);


}

resultadoOperacion* ArbolBMasNumerico::buscarBloqueNumerico(unsigned int clave,
		Bloque* bloqueEncontrado) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	buscarBloque(claveAlfabetica, bloqueEncontrado);
}

resultadoOperacion* ArbolBMasNumerico::eliminarNumerico(unsigned int clave,
		unsigned int valor) {
	stringstream buffer(stringstream::in | stringstream::out);
	buffer << clave;
	string claveAlfabetica;
	buffer >> claveAlfabetica;
	eliminar(claveAlfabetica, valor);

}

resultadoOperacion* ArbolBMasNumerico::siguienteNumerico(Registro* regSiguiente) {

	siguiente(regSiguiente);

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

