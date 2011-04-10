/*
 * Registro.cpp
 *
 *  Created on: 05/04/2011
 *      Author: rjc
 */

#include "Registro.h"

Registro::Registro(string unString) {
  this->miString= unString;
}

Registro::Registro(unsigned int id, unsigned short int unaReferencia) {
	this->identificadores.push_back(id);
	this->referencias.push_back(unaReferencia);
}

Registro::Registro(string unString, unsigned int id) {
	this->miString= unString;
	this->identificadores.push_back(id);
}

string Registro::getString(){
	return this->miString;
}

bool Registro::buscarID(unsigned int idBuscado){
	list<unsigned int>::iterator it= this->identificadores.begin();
	bool encontrado= false;
	while (it!=this->identificadores.end()&&!encontrado) {
		if (*it==idBuscado) encontrado= true;
		it++;
	}
	return encontrado;
}

list<unsigned int>* Registro::getIdentificadores(){
	return &this->identificadores;
}

list<unsigned short int>* Registro::getReferencias(){
	return &this->referencias;
}

void Registro::agregarId (unsigned int id){
  this->identificadores.push_back(id);
}

void Registro::agregarReferencia(unsigned short int ref) {
  this->referencias.push_back(ref);
}


Registro::~Registro() {

}
