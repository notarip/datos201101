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

#include "../util/Parametros.h"
#include "../util/Util.h"
#include "../archivos/Registro.h"
#include "../archivos/ArchivoBloques.h"
#include "../archivos/Bloque.h"
#include "../archivos/ExceptionBloque.h"
#include "../util/Parametros.h"
#include "resultadoOperacion.h"
#include "ExceptionDuplicacionClaves.h"
#include <sstream>

using namespace std;

class ArbolBMas {

protected:
	ArchivoBloques* archivoNodos;

public:
	ArbolBMas(string path, unsigned int tamanioBloque);

	virtual ~ArbolBMas();

	resultadoOperacion insertar(string clave, unsigned int valor);

	resultadoOperacion eliminar(string clave);

	Registro* siguiente();

	//busca un bloque en donde se puede encontrar una clave, sino esta en ese bloque => no esta
	Bloque* buscarBloque(string clave);

	Registro* buscarRegistro (string clave, resultadoOperacion* resultOperacion);

	void exportar(string path);


private:
	list<Registro>::iterator ultimoRegistroVisitado;
	Bloque* raiz;
	Bloque* ultimaHojaVisitada;
	unsigned int tamanioNodo;

	/*PARA DEFINIR UN ARBOL B+ HAY QUE INDICAR 2 ESTRATEGIAS:
	 * 1- Brindar metodos al arbol para crear, consultar y cambiar la clave de un registro
	 * 2- Como comparar las claves de 2 registros (alfabeticas vs numericas)
	 *
	 * */
	virtual char compareRegistros(string clave, Registro* unRegistro ) = 0;
	virtual Registro* crearRegistroClave(string clave, unsigned int valor)= 0;
	virtual Registro* crearRegistroClave(string clave)= 0;
	virtual string consultarClave(Registro* unRegistro) = 0;
	virtual void setearClave(Registro* registroAModificar, string clave)= 0;

	resultadoOperacion insertarRecursivo( Bloque* bloqueActual, string clave,unsigned int valor);
	resultadoOperacion eliminarRecursivo( Bloque* bloqueActual, string clave);
	void resolverOverflow(Bloque* bloqueOverflow, unsigned int nroBloqueOverflow, Bloque* bloqueActual);
	void resolverUnderflow (Bloque* bloqueUnderflow, unsigned int nroBloqueUnderflow, Bloque* bloqueActual, list<Registro>::iterator itRegistros, bool bajePorUltimo);
	Bloque* buscarBloqueRecursivo(string clave ,unsigned int refBloque);
	list<Registro>::iterator agregarRegistroEnOrden(Bloque* unBloque,Registro unRegistro);

	string exportarRecursivo(unsigned int nroBloque, unsigned int nivelRecursion);
	string imprimirBloque(Bloque* unBloque, unsigned int nroBloque);

	virtual float obtenerOcupacionBloque(Bloque* unBloque);
	virtual void guardarBloque(Bloque* unBloque, unsigned int nroBloque);
	virtual Bloque* obtenerBloque(unsigned int nroBloque);
};

#endif /* ARBOLBMAS_H_ */
