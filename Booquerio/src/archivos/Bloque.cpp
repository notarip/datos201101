/*
 * Bloque.cpp
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#include "Bloque.h"

Bloque::Bloque() {
	this->siguiente= 0;
	this->atributoBloque= 0;
}

Bloque::Bloque(Registro unRegistro) {
	this->siguiente= 0;
	this->atributoBloque= 0;
	this->listaRegistros.push_back(unRegistro);
}

void Bloque::agregarRegistro(Registro unRegistro) {
	this->listaRegistros.push_back(unRegistro);
}

void Bloque::agregarRegistroAlInicio(Registro unRegistro){
	this->listaRegistros.push_front(unRegistro);


}

Registro* Bloque::recuperarRegistro(unsigned int atribEntero) {
	//este metodo esta pensado para el indice de atribEnteros, o sea los registros en realidad
	//contendrian solo un atribEntero, se hizo asi para que sea mas gral
	Registro* resultado = NULL;
	bool encontrado = false;
	list<Registro>::iterator it = this->listaRegistros.begin();
	while (it != this->listaRegistros.end() && !encontrado) {
		if ((*it).buscarAtribEntero(atribEntero))
			resultado = &(*it);
		it++;
	}
	return resultado;
}

Registro* Bloque::recuperarRegistro(string palabra) {
	Registro* resultado = NULL;
	bool encontrado = false;
	list<Registro>::iterator it = this->listaRegistros.begin();
	while (it != this->listaRegistros.end() && !encontrado) {
		if ((*it).getString()==palabra) {
			resultado = &(*it);
			encontrado= true;
		}
		it++;
	}
	return resultado;
}

list<Registro>* Bloque::obtenerRegistros(){
	return &this->listaRegistros;
}

void Bloque::setSiguiente(unsigned int siguiente){
	this->siguiente= siguiente;
}

unsigned int Bloque::getSiguiente(){
	return this->siguiente;
}

void Bloque::setAtributoBloque(unsigned int atributoBloque){
	this->atributoBloque=atributoBloque;
}

unsigned int Bloque::getAtributoBloque(){
	return this->atributoBloque;
}

list<Registro>::iterator Bloque::agregarRegAlfa (Registro unRegistro){
	list<Registro>::iterator itRegistros= this->listaRegistros.begin();
	while (itRegistros->getString()<=unRegistro.getString() && itRegistros!=this->listaRegistros.end()){
		itRegistros++;
	}
	this->listaRegistros.insert(itRegistros,unRegistro);
}
list<Registro>::iterator Bloque::agregarRegNum (Registro unRegistro){
	list<Registro>::iterator itRegistros= this->listaRegistros.begin();
		while (itRegistros->getAtributosEnteros()->front()<=unRegistro.getAtributosEnteros()->front() && itRegistros!=this->listaRegistros.end()){
			itRegistros++;
		}
		this->listaRegistros.insert(itRegistros,unRegistro);
}

Bloque::~Bloque() {

}
