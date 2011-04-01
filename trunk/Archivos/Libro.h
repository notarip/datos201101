/*
 * Libro.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */


#ifndef LIBRO_H_
#define LIBRO_H_
#include <string>

using namespace std;

class Libro {
public:
	Libro(string titulo, string editorial, string autor, string texto);
	virtual ~Libro();
	string getTitulo();
	string getEditorial();
	string getAutor();
	string getTexto();


private:
	string titulo,editorial,autor,texto;



};

#endif /* LIBRO_H_ */
