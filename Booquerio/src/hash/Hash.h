/*
 * Hash.h
 *
 *  Created on: 09/04/2011
 *      Author: pablo
 */

#ifndef HASH_H_
#define HASH_H_

typedef unsigned int elemLista;

// #define EXTENCION_TABLA ".table" ya no sirve (creo).
#define EXTENSION_DATOS ".hash"
#define TAMANIO_TABLA 8
#define BYTES_TABLA TAMANIO_TABLA*sizeof(elemLista)
#define TAMANIO_BLOQUE 4096
#define TAMANIO_BLOQUE_TABLA 1024

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

	/*
	 * Inserta un registro es pre que este
	 * abierto el hash, retorna el resutado de la
	 * operacion
	 */
	void insertar(Registro *registro);

	/*
	 * Borra del hash el registro @que
	 * si lo borra retorna ok
	 * si no lo encontro retorna
	 * error
	 *
	 */
	void borrar(string que);

	unsigned int hasheo(string key);
	/*
	 * busca en el hash la clave @que
	 * y devuelve el registro, si no encuentra la
	 * clave devuelve null
	 */
	Registro* buscar(string que);

	unsigned int getTamanioTabla();

	void mostrar();

	virtual ~Hash();

private:
	string pathHash;
	unsigned int tamanioTabla;
	unsigned int *tabla;
	/*
	 * Crea un hash
	 */
	int crear();
	/*
	 * Abre un hash
	 */
	int abrir();

	/*
	 * Retorna el nro de elemento en la lista
	 */


	/*
	 * Determina el tamaño de dispersion para un elemento
	 */
	unsigned int tamDispersion(unsigned int numbloque);


	/*
	 * Actualiza la tabla cuando se realiza una insercion.
	 */
	void actualizarTabla_insercion(unsigned int tamDispersion,unsigned int pos_desbordado,unsigned int bl_nuevo);

	/*
	 * Actualiza la tabla cuando se realiza una eliminacion.
	 */
	void actualizarTabla_eliminacion(unsigned int tamDispersion,unsigned int pos_eliminado);

	/*
	 * Reestructura el archivo en los casos de overflow.
	 */
	void reestructurar_archivo(ArchivoBloques archivo,unsigned int nro_desbordado,unsigned int nro_libre,Registro* registro_a_ins); //TODO VER BIEN!!!!!!!*/

	void guardarTabla();

};

#endif /* HASH_H_ */
