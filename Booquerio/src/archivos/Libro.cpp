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
string Libro::getPalabras(){return this->palabras;}
unsigned int Libro::getCantPalabras(){return this->cantPalabras;}


void Libro::setPalabras(string palabras, unsigned int cantidad)
{
	this->palabras = palabras;
	this->cantPalabras = cantidad;
}

set<string> *Libro::getListaPalabras()
{
	set<string> *lista = new set<string>();
	unsigned int posIni = 0;
	unsigned int posFin;
	bool fin = false;
	string palabra;

	while (!fin)
	{
		posFin = this->palabras.find(',',posIni);
		palabra = this->palabras.substr(posIni, posFin - posIni); //string a almacenar
		lista->insert(palabra);
		posIni = posFin + 1;
		if (posFin >= this->palabras.size()-1)
			fin = true;
	}

	return lista;
}

string Libro::toString()
{
	string respuesta;

	respuesta += "Autor: ";
	respuesta += this->autor;
	respuesta += "\n";
	respuesta += "Editorial: ";
	respuesta += this->editorial;
	respuesta += "\n";
	respuesta += "Titulo: ";
	respuesta += this->titulo;
	respuesta += "\n";
	respuesta += "Palabras: ";
	respuesta += Util().UIntToString(this->cantPalabras);
	respuesta += "\n";

	return respuesta;
}

Libro::~Libro() {
	// TODO Auto-generated destructor stub
}
