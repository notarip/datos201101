/*
 * ArbolBMas.h
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#ifndef ARBOLBMAS_H_
#define ARBOLBMAS_H_

#include "../archivos/Registro.h"
#include "../archivos/ArchivoBloques.h"
#include "../archivos/Bloque.h"
#include "../archivos/ExceptionBloque.h"
#include "../util/Parametros.h"
#include "resultadoOperacion.h"

using namespace std;

class ArbolBMas {
public:
	ArbolBMas(string path, float ocupacion, unsigned int tamanioBloque);
	virtual ~ArbolBMas();
	resultadoOperacion* insertar(string clave, unsigned int valor);

	//busca un bloque en donde se puede encontrar una clave, sino esta en ese bloque => no esta
	resultadoOperacion* buscarBloque(string clave, Bloque* bloqueEncontrado);

	resultadoOperacion* eliminar(string clave, unsigned int valor);

	resultadoOperacion* siguiente(Registro* regSiguiente);

	void exportar();

private:
	unsigned int ultimaHojaVisitada, ultimoValorBuscado;
	Bloque* raiz;
	ArchivoBloques* archivoNodos;
	float ocupacionNodo;
	unsigned int tamanioNodo;

	/*PARA DEFINIR UN ARBOL B+ HAY QUE INDICAR 2 ESTRATEGIAS:
	 * 1- En cual de todos los atritutos de un registro vas a guardar la clave(identificador)
	 * 2- Como comparo las claves (alfabeticas vs numericas)
	 * */
	virtual char compareRegistros(string clave, Registro* unRegistro ) = 0;
	virtual Registro* crearRegistroClave(string clave)= 0;

	resultadoOperacion* buscarBloqueRecursivo(string clave ,unsigned int refBloque, Bloque* bloqueEncontrado );

};

#endif /* ARBOLBMAS_H_ */
