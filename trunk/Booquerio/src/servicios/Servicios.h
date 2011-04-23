/*
 * Servicios.h
 *
 *  Created on: 22/04/2011
 *      Author: pablo
 */

#ifndef SERVICIOS_H_
#define SERVICIOS_H_

#define INDICE_AUTORES 0
#define INDICE_EDITORIALES 1
#define INDICE_TITULOS 2
#define INDICE_PALABRAS 3

#include <string>
#include <stdlib.h>
#include "../parser/Parser.h"
#include "../archivos/Libro.h"
#include "../servicios/SinIndice.h"
#include "../archivos/ArchivoLibros.h"

using namespace std;

class Servicios {
public:

	/*
	 * Resuelve el comando de toma de
	 * textos nuevos
	 * si la ruta no es absoluta lo resuelve
	 * como relativa segun el archivo "config.propiedades"
	 * y el path_libros
	 */
	static void tomarTexto(string ruta);
	/*
	 * Resuelve el comando procesamiento del indice
	 * por autores
	 */
	static void procesarAutores();
	/*
	 * Resuelve el comando procesamiento del indice
	 * por editoriales
	 */
	static void procesarEditoriales();
	/*
	 * Resuelve el comando procesamiento del indice
	 * por titulos
	 */
	static void procesarTitulos();
	/*
	 * Resuelve el comando procesamiento del indice
	 * por palabras
	 */
	static void procesarPalabras();

	/*
	 * Lista por pantalla los libros
	 * que estan almacenados esten o no
	 * indexados
	 */
	static void listarLibros();


	/*
	 *Recrea en un txt el libro
	 *solicitado
	 */
	static void obtenerLibro(string unId);
private:
	static void procesarLibro(int indice);
	static void agregarIndiceAutores(Libro *unLibro);
	static void agregarIndiceEditoriales(Libro *unLibro);
	static void agregarIndiceTitulos(Libro *unLibro);
	static void agregarIndicePalabras(Libro *unLibro);

};

#endif /* SERVICIOS_H_ */
