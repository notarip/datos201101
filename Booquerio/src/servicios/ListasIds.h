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


#define TAMANIO_B_LISTA_IDS 4096
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

};

#endif /* LISTASIDS_H_ */
