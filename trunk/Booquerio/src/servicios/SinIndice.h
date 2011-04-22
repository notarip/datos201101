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


#define TAMANIO_B_LISTAS 1024
#define RUTA_LISTAS "path_listas"
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
	 void agregarLibroPendienteAutores(unsigned int unId);
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por titulo
	 */
	 void agregarLibroPendienteTitulos(unsigned int unId);
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por palabra
	 */
	 void agregarLibroPendientePalabras(unsigned int unId);
	/*
	 * Agrega un libro a la lista de libros sin procesar
	 * por editorial
	 */
	 void agregarLibroPendienteEditoriales(unsigned int unId);
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
	 void limpiarListaAutores();

	/*
	 * Vacia la lista de libros pendiente por
	 * Titulo
	 */
	 void limpiarListaTitulos();

	/*
	 * Vacia la lista de libros pendiente por
	 * palabras
	 */
	 void limpiarListaPalabras();

	/*
	 * Vacia la lista de libros pendiente por
	 * editoriales
	 */
	 void limpiarListaEditoriales();

	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *autor
	 */
	 void sacarLibroListaAutores(unsigned int unId);


	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *titulo
	 */
	 void sacarLibroListaTitulos(unsigned int unId);

	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *editorial
	 */
	 void sacarLibroListaEditorial(unsigned int unId);

	/*
	 *Elimina un libro de la lista de libros sin procesar por
	 *palabra
	 */
	 void sacarLibroListaPalabras(unsigned int unId);


	SinIndice(void);
private:
	static SinIndice *pInstancia;
	string rutaListas;
	void agregarLibro(unsigned int unId, int lista);
	void borrarLibro(unsigned int unId, int lista);
	void vaciarLista(int lista);
	list<unsigned int>* getLista(int lista);
	void copiarBloque(int lista, Bloque *nuevoBloque, Bloque *viejoBloque);

};

#endif /* SININDICE_H_ */
