/*
 * Parser.h
 *
 *  Created on: 26/03/2011
 *      Author: pablo
 */

#ifndef PARSER_H_
#define PARSER_H_

#define ARCHIVO_INVALIDO 1
#define SIN_AUTOR 2
#define SIN_EDITORIAL 3
#define SIN_TITULO 4

#define TAG_PATH_STOP_WORDS "path_stopWords"

#include <string>
#include <list>
#include <fstream>
#include "../util/Parametros.h"
using namespace std;


/*
 * Clase encargada del parseo de los libros desde
 * un archivo de texto plano.
 */

class Parser {
public:
	/*
	 * Cuando se construye levanta del archivo de propiedades
	 * el path del archivo de stop words y lo carga en la lista
	 */
	Parser();
	virtual ~Parser();
	/*
	 * Metodo encargado del parseo del @archivo
	 * si el archivo es invalido retorna 		1
	 * si no se encontro el autor retorna 		2
	 * si no se encontro la editorial retorna 	3
	 * si no encontro el titulo retorna			4
	 *
	 * si se pudo parsear correctamente retorna 0
	 */
	int parsear(string archivo);

	/*
	 * Metodo para pruebas
	 * lista por consola las stop words que
	 * levanto del archivo
	 */
	void listarStopWords();

private:
	string autor;
	string editorial;
	string titulo;
	string texto;
	list<string> stopWords;
	/*
	 * Metodo privado que levanta las palabras
	 * del archivo de stopWords y las guarda en la
	 * lista.
	 */
	void levantarPalabras(fstream *archStopW);
};

#endif /* PARSER_H_ */
