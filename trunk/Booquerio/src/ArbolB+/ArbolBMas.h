/*
 * ArbolBMas.h
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#ifndef ARBOLBMAS_H_
#define ARBOLBMAS_H_

#define PORCENTAJE_UNDERFLOW 0.4
#define PORCENTAJE_OVERFLOW 0.5

#include "../archivos/Registro.h"
#include "../archivos/ArchivoBloques.h"
#include "../archivos/Bloque.h"
#include "../archivos/ExceptionBloque.h"
#include "../util/Parametros.h"
#include "resultadoOperacion.h"
#include <sstream>

using namespace std;

class ArbolBMas {
public:
	ArbolBMas(string path, unsigned int tamanioBloque);
	virtual ~ArbolBMas();
	resultadoOperacion insertar(string clave, unsigned int valor);

	resultadoOperacion eliminar(string clave, unsigned int valor);

	Registro* siguiente();

	//busca un bloque en donde se puede encontrar una clave, sino esta en ese bloque => no esta
	Bloque* buscarBloque(string clave);

	Registro* buscarRegistro (string clave, resultadoOperacion* resultOperacion);

	void exportar(string path);


private:
	list<Registro>::iterator ultimoRegistroVisitado;
	Bloque* raiz;
	Bloque* ultimaHojaVisitada;
	ArchivoBloques* archivoNodos;
	unsigned int tamanioNodo;

	/*PARA DEFINIR UN ARBOL B+ HAY QUE INDICAR 2 ESTRATEGIAS:
	 * 1- En cual de todos los atritutos de un registro vas a guardar la clave(identificador)
	 * 2- Como comparo las claves (alfabeticas vs numericas)
	 * */
	virtual char compareRegistros(string clave, Registro* unRegistro ) = 0;
	virtual Registro* crearRegistroClave(string clave, unsigned int valor)= 0;
	virtual Registro* crearRegistroClave(string clave)= 0;
	virtual string consultarClave(Registro* unRegistro) = 0;
	virtual void setearClave(Registro* registroAModificar, string clave)= 0;

	resultadoOperacion insertarRecursivo( Bloque* bloqueActual, string clave,unsigned int valor);
	resultadoOperacion eliminarRecursivo( Bloque* bloqueActual, string clave,unsigned int valor);
	void resolverOverflow(Bloque* bloqueOverflow, unsigned int nroBloqueOverflow, Bloque* bloqueActual);
	void resolverUnderflow (Bloque* bloqueUnderflow, unsigned int nroBloqueUnderflow, Bloque* bloqueActual, list<Registro>::iterator itRegistros, bool bajePorUltimo);
	Bloque* buscarBloqueRecursivo(string clave ,unsigned int refBloque);
	list<Registro>::iterator agregarRegistroEnOrden(Bloque* unBloque,Registro unRegistro);

	string exportarRecursivo(unsigned int nroBloque, unsigned int nivelRecursion);
	string imprimirBloque(Bloque* unBloque, unsigned int nroBloque);
};

#endif /* ARBOLBMAS_H_ */
