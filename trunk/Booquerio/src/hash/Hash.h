/*
 * Hash.h
 *
 *  Created on: 09/04/2011
 *      Author: hernan
 */

#ifndef HASH_H_
#define HASH_H_

typedef unsigned int elemLista;

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

	//
	// Inserta un registro dentro del archivo de datos de hash.
	// si el registro pudo insertarse devuelve "true" caso contrario "false".
	//
	void insertar(Registro *registro);


	 //
	 //Borra del hash el registro  con clave "que", si lo borra retorna ok
	 // si no lo encontro retorna falso.
	 //
	void borrar(string que);


	//
	// Funcion de hashing extensivo para registros con claves-
	// de cadenas de longitud variable.
	//

	unsigned int hasheo(string key);

	//
	// busca en el hash la clave "que", y devuelve el registro si lo encuentra,
	// en caso contrario devuelve NULL.
	//
	Registro* buscar(string que);


	//
	// Devuelve la longitud de la tabla hash.
	//
	unsigned int getTamanioTabla();

	// metodo de test.
	// Devuelve un puntero a la tabla hash en memoria (deberia ser privado, DESPUES LO MUEVO).
	//
	unsigned int* getTabla();

	// metodo de test.
	// setea el tamaño de la tabla hash (deberia ser privado, DESPUES LO MUEVO).
	//
	void setTamTabla(unsigned int t){ this->tamanioTabla=t;}

	// metodo de test.
	// setea el contenido de la tabla hash en memoria (deberia ser privado, DESPUES LO MUEVO).
	//
	void setTabla(unsigned int* ta);

	//
	// Muesta el contenido de la tabla actualmente, y del contenido de los buckets
	// (solo contiene las claves de los registros.)
	//
	void mostrar();

	void mostrar2(string s);

	//
	// Guarda la tabla hash en el disco para que posteriormente pueda ser recuperada.
	//
	virtual ~Hash();


private:
	string pathHash;
	unsigned int tamanioTabla;
	unsigned int *tabla;


	//
	// Crea un hash.
	//
	int crear();

	//
	// Abre el hash si ya fue creado, cargando la tabla hash a memoria.
	//
	int abrir();


	//
	// Determina el tamaño de dispersion para un Bucket.
	//
	unsigned int tamDispersion(unsigned int numbloque);

	//
	// Actualiza el estado de la tabla hash en memoria,
	// luego de realizar una eliminacion.
	//
	void actualizarTabla_eliminacion(unsigned int tamDispersion,unsigned int pos_eliminado);

	//
	// Actualiza el estado de la tabla hash en memoria,
	//luego de realizar una insercion.
	//
	void actualizarTabla_insercion(unsigned int tamDispersion,unsigned int pos_desbordado,unsigned int bl_nuevo);


	//
	// Reestructura el archivo en los casos que se inserta un-
	// registro y se produce desborde en un Bucket.
	//
	void reestructurar_archivo(ArchivoBloques archivo,unsigned int nro_desbordado,unsigned int nro_libre,Registro* registro_a_ins); //TODO VER BIEN!!!!!!!*/

	//
	// Guarda la tabla en el archivo del hash.
	//
	void guardarTabla();

};

#endif /* HASH_H_ */
