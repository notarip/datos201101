/*
 * Libro.cpp
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#include "Libro.h"



Libro::Libro(unsigned int id,string titulo, string autor, string editorial, string texto) {
	this->id=id;
	this->titulo=titulo;
	this->autor=autor;
	this->editorial=editorial;
	this->texto=texto;


}

unsigned int Libro::getId(){return this->id;}
string Libro::getTitulo(){return this->titulo;}
string Libro::getEditorial(){return this->editorial;}
string Libro::getAutor(){return this->autor;}
string Libro::getTexto(){return this->texto;}

Libro::~Libro() {
	// TODO Auto-generated destructor stub
}
