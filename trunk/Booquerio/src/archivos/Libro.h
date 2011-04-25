/*
 * Libro.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */


#ifndef LIBRO_H_
#define LIBRO_H_

#include <string>
#include <set>
#include "../util/Util.h"

using namespace std;

class Libro {
public:
	Libro(unsigned int id,string titulo, string editorial, string autor, string texto);
	Libro(unsigned int id,string titulo, string autor, string editorial, string texto, unsigned int cantPalabras, string palabras);
	virtual ~Libro();
	unsigned int getId();
	string getTitulo();
	string getEditorial();
	string getAutor();
	string getTexto();
	string getPalabras();
	set<string>* getListaPalabras();
	unsigned int getCantPalabras();
	string toString();
	void setPalabras(string palabras, unsigned int cantidad);

private:
	unsigned int id, cantPalabras;
	string titulo,editorial,autor,texto,palabras;
	list<string> *listaPalabras;



};

#endif /* LIBRO_H_ */
