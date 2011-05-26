/*
 * SinIndice.h
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#ifndef SININDICE_H_
#define SININDICE_H_



#include <list>
#include "../archivos/ArchivoBloques.h"
#include "../archivos/ExceptionBloque.h"
#include "../archivos/Bloque.h"
#include "../archivos/Registro.h"
#include "../util/Parametros.h"
#include "../util/Util.h"

#define ERROR_ARCHIVO_LISTAS -30

#define TAMANIO_B_LISTAS 2048
#define LISTA_AUTORES 0
#define LISTA_EDITORIALES 1
#define LISTA_TITULOS 2
#define LISTA_PALABRAS 3



using namespace std;

class SinIndice {
public:
    static SinIndice* getInstancia();
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por autor
	 */
	 int agregarLibroPendienteAutores(unsigned int unId);
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por titulo
	 */
	 int agregarLibroPendienteTitulos(unsigned int unId);
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por palabra
	 */
	 int agregarLibroPendientePalabras(unsigned int unId);
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por editorial
	 */
	 int agregarLibroPendienteEditoriales(unsigned int unId);
	 /*
	  * Agrega un libro a todas las listas
	  */
	 int agregarLibroPendienteATodasLasListas(unsigned int unId);
	 /*
	 * retorna la lista de libros pendientes de procesar por
	 * autor
	 */
	 list<unsigned int>* getPendientesAutores();
	/*
	 * retorna la lista de libros pendientes de procesar por
	 * titulo
	 */
	 list<unsigned int>* getPendientesTitulos();
	/*
	 * retorna la lista de libros pendientes de procesar por
	 * palabra
	 */
	 list<unsigned int>* getPendientesPalabras();
	/*
	 * retorna la lista de libros pendientes de procesar por
	 * editorial
	 */
	 list<unsigned int>* getPendientesEditoriales();

	/*
	 * Vacia la lista de libros pendiente por
	 * autor
	 */
	 int limpiarListaAutores();

	/*
	 * Vacia la lista de libros pendiente por
	 * Titulo
	 */
	 int limpiarListaTitulos();

	/*
	 * Vacia la lista de libros pendiente por
	 * palabras
	 */
	 int limpiarListaPalabras();

	/*
	 * Vacia la lista de libros pendiente por
	 * editoriales
	 */
	 int limpiarListaEditoriales();

	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *autor
	 */
	 int sacarLibroListaAutores(unsigned int unId);


	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *titulo
	 */
	 int sacarLibroListaTitulos(unsigned int unId);

	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *editorial
	 */
	 int sacarLibroListaEditorial(unsigned int unId);

	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *palabra
	 */
	 int sacarLibroListaPalabras(unsigned int unId);

	 /*
	  *Elimina el libro de todas las listas
	  */
	 int sacarLibroDeTodasLasListas(unsigned int unId);


	SinIndice(void);
private:
	static SinIndice *pInstancia;
	string rutaListas;
	int agregarLibro(unsigned int unId, int lista);
	int borrarLibro(unsigned int unId, int lista);
	int vaciarLista(int lista);
	list<unsigned int>* getLista(int lista);
	void copiarBloque(int lista, Bloque *nuevoBloque, Bloque *viejoBloque);

};

#endif /* SININDICE_H_ */
