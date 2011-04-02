/*
 * Parser.h
 *
 *  Created on: 26/03/2011
 *      Author: pablo
 */

#ifndef PARSER_H_
#define PARSER_H_

#define EXITOSO 0
#define ARCHIVO_INVALIDO 1
#define SIN_AUTOR 2
#define SIN_EDITORIAL 3
#define SIN_TITULO 4

#define TAG_PATH_STOP_WORDS "path_stopWords"
#define INVALIDOS ".:,;¡!'?¿\"“”(){}[]-"


#include <string>
#include <list>
#include <fstream>
#include "../util/Parametros.h"
#include "../util/Util.h"
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
	 * Retorna la lista de palabras que
	 * contiene el libro con las siguentes
	 * restricciones:
	 * -todas la palabras estan en minusculas
	 * -no estan las stop words
	 * -puede haber duplicadas
	 */
	list<string>* obtenerPalabras();

	/*
	 * Metodo para pruebas
	 * lista por consola las stop words que
	 * levanto del archivo
	 */
	void listarStopWords();
	/*
	 * Metodo para pruebas
	 * lista por consola los
	 * atributos del libro que parseo
	 *
	 */
	void listarLibro(bool conTexto);

private:
	string autor;
	string editorial;
	string titulo;
	string texto;
	list<string> stopWords;

	/*
	 * Metodo privado que levanta las palabras
	 * del archivo de un csv y las guarda en la
	 * @lista.
	 */
	void levantarCSV(fstream *archivo, list<string> *lista);

	/*
	 * Metodo privado que levanta la informacion
	 * del libro.
	 */
	void procesarLibro(fstream *archLibro, string archivo);
	/*
	 * Metodo provado que se encarga de
	 * generar una lista de palabras filtrando
	 * las stop words
	 */
	list<string> *procesarPalabras();


	/*
	 * Retira los caracteres invalidos
	 * de @palabra, comprueba la no existencia
	 * como stop word y la agrega en la lista
	 * de palabras
	 */
	void procesarPalabra(string palabra, list<string>* palabras);


	/*
	 * Guarda una palabra en la lista de palabras
	 * siempre que no este en la lista de stop words
	 */

	void guardarPalabra(string palabra, list<string> *palabras);


	/*
	 * Desde @posIni busca en el texto
	 * del libro donde termina la siguiente palabra
	 *
	 */
	unsigned int encontrarFinPalabra(unsigned int posIni);

	/*
	 * Busca una @palabra dentro de la lista
	 * de stop words si la encuentra
	 * retorna true sino false
	 */
	bool buscarStopWord(string palabra);

	/*
	 * Separa del nombre del archivo el autor
	 * y el titulo del libro desde @nombre
	 * del archivo
	 */
	void obtenerAutorTitulo(string nombre);

	/*
	 * Obtiene del listado de editoriales
	 * la editorial del libro, el autor
	 * debe estar cargado
	 */
	void obtenerEditorial();
};

#endif /* PARSER_H_ */
