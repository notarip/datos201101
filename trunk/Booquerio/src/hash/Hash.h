/*
 * Hash.h
 *
 *  Created on: 09/04/2011
 *      Author: pablo
 */

#ifndef HASH_H_
#define HASH_H_

typedef unsigned int elemLista;
struct elemLista2{
	elemLista nroBloque;
	elemLista TD;
};

#define EXTENCION_TABLA ".table"
#define EXTENCION_DATOS ".hash"
#define TAMANIO_TABLA 8
#define BYTES_TABLA TAMANIO_TABLA*sizeof(elemLista)
#define TAMANIO_BLOQUE 512

#include <string.h>
#include <fstream>
#include "../util/Parametros.h"
#include "../util/Util.h"
#include "../archivos/Registro.h"
#include "../archivos/ArchivoBloques.h"
#include "../archivos/Bloque.h"
#include "../archivos/ExceptionBloque.h"
using namespace std;



class Hash {
public:
	Hash(string nombre);
	virtual ~Hash();
	/*
	 * Inserta un registro es pre que este
	 * abierto el hash, retorna el resutado de la
	 * operacion
	 */
	void insertar(Registro *registro);

	/*
	 * Borra del hash el @que
	 * si lo borra retorna ok
	 * si no lo encontro retorna
	 * error
	 *
	 */
	void borrar(string que);
	/*
	 * busca en el hash la clave @que
	 * y devuelve el registro, si no encuentra la
	 * clave devuelve null
	 */
	Registro* buscar(string que);
private:
	fstream archivoTabla;
	string nombre;
	string pathTabla;
	string pathDatos;
	elemLista tamanioLista;
	elemLista2 *tabla;
	unsigned int offsetUltimaBusqueda;
	/*
	 * Crea un hash
	 */
	int crear();
	/*
	 * Abre u	n hash
	 */
	int abrir();

	/*
	 * Retorna el nro de elemento en la lista
	 */
	int hasheo(string key);

};

#endif /* HASH_H_ */
