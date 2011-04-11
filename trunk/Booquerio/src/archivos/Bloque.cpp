/*
 * Bloque.cpp
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#include "Bloque.h"

Bloque::Bloque() {

}

Bloque::Bloque(Registro unRegistro) {
	this->listaRegistros.push_back(unRegistro);
}

void Bloque::agregarRegistro(Registro unRegistro) {
	this->listaRegistros.push_back(unRegistro);
}

Registro* Bloque::recuperarRegistro(unsigned int id) {
	//este metodo esta pensado para el indice de ids, o sea los registros en realidad
	//contendrian solo un id, se hizo asi para que sea mas gral
	Registro* resultado = NULL;
	bool encontrado = false;
	list<Registro>::iterator it = this->listaRegistros.begin();
	while (it != this->listaRegistros.end() && !encontrado) {
		if ((*it).buscarID(id))
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

Bloque::~Bloque() {

}
