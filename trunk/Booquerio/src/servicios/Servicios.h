/*
 * Servicios.h
 *
 *  Created on: 22/04/2011
 *      Author: pablo
 */

#ifndef SERVICIOS_H_
#define SERVICIOS_H_

#define ERROR_RUTA_INVALIDA -20
#define ERROR_RUTA_ARCHIVO_LIBROS -40

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
	static int tomarTexto(string ruta);
	/*
	 * Resuelve el comando procesamiento del indice
	 * por autores
	 */
	static int procesarAutores();
	/*
	 * Resuelve el comando procesamiento del indice
	 * por editoriales
	 */
	static int procesarEditoriales();
	/*
	 * Resuelve el comando procesamiento del indice
	 * por titulos
	 */
	static int procesarTitulos();
	/*
	 * Resuelve el comando procesamiento del indice
	 * por palabras
	 */
	static int procesarPalabras();

	/*
	 * Lista por pantalla los libros
	 * que estan almacenados esten o no
	 * indexados
	 */
	static int listarLibros();


	/*
	 *Recrea en un txt el libro
	 *solicitado
	 */
	static int obtenerLibro(string unId);


	/*
	 * Elimina un libro de los libros
	 * disponibles (reg variables, indices,etc)
	 */
	static int quitarArchivo(string unId);

	/*
	 * Lista las estructuras del indice de
	 * Autores
	 */
	static int verEstructuraAutor();

	/*
	 * Lista las estructuras del indice de
	 * Editoriales
	 */
	static int verEstructuraEditorial();

	/*
	 * Lista las estructuras del indice de
	 * Titulos
	 */
	static int verEstructuraTitulos();

	/*
	 * Lista las estructuras del indice de
	 * Palabras
	 */
	static int verEstructuraPalabras();

private:
	static void modificarListaIds(string clavePasada,unsigned int id);
	static int procesarLibro(int indice);
	static int agregarIndiceAutores(Libro *unLibro);
	static int agregarIndiceEditoriales(Libro *unLibro);
	static int agregarIndiceTitulos(Libro *unLibro);
	static int agregarIndicePalabras(Libro *unLibro);

};

#endif /* SERVICIOS_H_ */
