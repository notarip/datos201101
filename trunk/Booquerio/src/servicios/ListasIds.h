/*
 * ListasIds.h
 *
 *  Created on: 01/05/2011
 *      Author: pablo
 */

#ifndef LISTASIDS_H_
#define LISTASIDS_H_

#include <string>
#include <stdlib.h>
#include "../archivos/ArchivoBloques.h"
#include "../util/Parametros.h"


#define TAMANIO_B_LISTA_IDS 1024
#define LISTA_VACIA 1


using namespace std;

class ListasIds {
public:

	/*
	 * Agrega un id a la lista que indica el offset, si el
	 * si es una lista nueva devuelve con que offset la
	 * creo
	 */
	static int agregarIdDeLibro(unsigned int *offset, unsigned int id, bool listaNueva);


	/*
	 * Remueve de la lista el id si la lista quedo vacia devuelve 1
	 * sino 0
	 */

	static int sacarIdDelLibro(unsigned int *offset, unsigned int id);

	/*
	 * Obtiene la lista de Ids del bloque referenciado por el offset. La lista de retorno
	 * se almacena en el parametro listaIds pasado.
	 */

	static int obtenerListaIds(unsigned int offset, list<unsigned int>* listaIds);

};

#endif /* LISTASIDS_H_ */
