/*
 * ArbolBMas.cpp
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(string path, float ocupacion, unsigned int tamanioBloque) {
	this->ocupacionNodo = ocupacion;
	this->archivoNodos = new ArchivoBloques(path, tamanioBloque);

	this->tamanioNodo = tamanioBloque;
	ultimaHojaVisitada = NULL;
	ultimoValorBuscado = NULL;

	raiz = new Bloque();

	archivoNodos->grabarBloque(raiz, 0);

}

resultadoOperacion* ArbolBMas::insertar(string clave, unsigned int valor) {

	this->insertarRecursivo(this->raiz,clave,valor);

}

resultadoOperacion* ArbolBMas::insertarRecursivo(Bloque* bloqueActual,
		string clave, unsigned int valor) {

	//pregunto por el caso de fin de recurison, ¿Nodo Hoja?
	if (bloqueActual->getAtributoBloque() == 0) {
		//debo escribir el registro, mi papa se encaragara de
		//ver si entre en overflow o no cuando intente grabarme

		Registro* registroAAgregar = this->crearRegistroClave(clave);

		/************************************************************/
		bloqueActual->agregarRegistro(*registroAAgregar);
		//MUCHO OJO!!! NO SE INSERTAN EN ORDEN => TO DO
		/************************************************************/
		return new resultadoOperacion(HUBO_MODIFICACION);

	}
	//sino soy hoja entonces debo ver por donde sigo la recursion y
	//manejar eventos que hayan sucedido a mi hijo

	else {

		resultadoOperacion* resultadoHijo = NULL;
		list<Registro>* listaRegistros = bloqueActual->obtenerRegistros();
		Bloque* bloqueABajar = NULL;
		unsigned int nroBloqueABajar;

		//itero por los registro del bloque
		list<Registro>::iterator itRegistros = listaRegistros->begin();
		//hasta encontrar uno mayor
		while (compareRegistros(clave, &(*itRegistros)) < 0 && itRegistros
				!= listaRegistros->end()) {
			itRegistros++;
		}
		//si llegaste al fin de la lista sin encontra un mayor => bajo por DERECHA
		if (itRegistros == listaRegistros->end()) {
			nroBloqueABajar = itRegistros->getReferenciai(2);
			bloqueABajar = this->archivoNodos->recuperarBloque(nroBloqueABajar);
		}
		else{
		//sino bajo por la IZQUIERDA del mayor
			nroBloqueABajar =itRegistros->getReferenciai(1);
			bloqueABajar = this->archivoNodos->recuperarBloque(nroBloqueABajar);
		}

		//llamo a la siguiente recursion
		resultadoHijo = insertarRecursivo(bloqueABajar,clave,valor);

		//volviendo de la recursion debo encargarme de resolver los
		//problemas de mi hijo

		//si fue modificado lo intento grabar
		//-> ojo puede venir exception por overflow
		if(resultadoHijo->getCodigo() == HUBO_MODIFICACION){
			try{
				this->archivoNodos->grabarBloque(bloqueABajar,nroBloqueABajar);
			}
			catch(ExceptionBloque& e){
				Bloque* bloqueSplit = new Bloque();
				list<Registro>* listaRegistros = bloqueABajar->obtenerRegistros();
				Registro registroASplit;

				//realizo el split
				while (archivoNodos->getOcupacionBloque(bloqueSplit) <= 0.50){
					registroASplit = listaRegistros->back();
					listaRegistros->pop_back();
					bloqueSplit->agregarRegistro(registroASplit);
				}
				//grabo el bloque q antes me dio overflow
				archivoNodos->grabarBloque(bloqueABajar,nroBloqueABajar);
				//grabo el nuevo bloque producto de split donde me diga archivoNodos
				unsigned int posLibre = archivoNodos->getBloqueLibre();
				archivoNodos->grabarBloque(bloqueSplit,posLibre);

				//me agrego a mi bloque un orientador con la referencia a este nuevo nodo
				//el lugar donde va ese registro orientador quedo guardado en itregistros
				//antes de ver por donde bajaba
				// => Ahora debo agregar el registro ahi:
				//con doble referencia si baje por el ultimo
				//con referencia simple y modificando la referencia del que me sigue
				//si baje por alguno del medio.

				/*Registro* registroOrientador = this->crearRegistroClave();
				registroOrientador->agregarReferencia()
				bloqueActual->agregarRegistro(*registroOrientador);*/

				delete resultadoHijo;
				return new resultadoOperacion(HUBO_MODIFICACION);

				/*FALTO VER LOS OVERFLOW DE NODOS INTERNOS*/
			}

			//sino hubo overflow
			delete resultadoHijo;
			return new resultadoOperacion(OK);

		}


	}

}

resultadoOperacion* ArbolBMas::eliminar(string clave, unsigned int valor) {
	return 0;
}

resultadoOperacion* ArbolBMas::siguiente(Registro* regSiguiente) {
	return 0;
}

resultadoOperacion* ArbolBMas::buscarBloque(string clave,
		Bloque* bloqueEncontrado) {
	list<Registro>* listaReg = raiz->obtenerRegistros();

	//si la raiz es una hoja (nivel 0) => solo puede estar ahi
	if (raiz->getAtributoBloque() == 0) {
		bloqueEncontrado = raiz;
		//return resultadoOp: encontre un bloque donde puede estar
	}

	// itero sobre los registros de la raiz
	list<Registro>::iterator itReg = listaReg->begin();

	//hasta encontrar uno mayor
	while (compareRegistros(clave, &(*itReg)) < 0 && itReg != listaReg->end()) {
		itReg++;
	}

	//si llegaste al fin de la lista sin encontra un mayor => bajo por derecha
	if (itReg == listaReg->end()) {
		return this->buscarBloqueRecursivo(clave, (*itReg).getReferenciai(2),
				bloqueEncontrado);
	}

	//sino bajo por la izquierda del mayor
	return this->buscarBloqueRecursivo(clave, (*itReg).getReferenciai(1),
			bloqueEncontrado);
}

resultadoOperacion* ArbolBMas::buscarBloqueRecursivo(string clave,
		unsigned int refBloque, Bloque* bloqueEncontrado) {

	//bajo el bloque a mem
	Bloque* esteBloque = archivoNodos->recuperarBloque(refBloque);

	//si se trata de un nodo hoja
	if (esteBloque->getAtributoBloque() == 0) {
		bloqueEncontrado = esteBloque;
		this->ultimaHojaVisitada = refBloque;
		//return resultadoOperacion

		//si es un nodo Interno
	} else {
		list<Registro>* listaRegistros = esteBloque->obtenerRegistros();

		//itero por los registro del bloque
		list<Registro>::iterator itRegistros = listaRegistros->begin();
		//hasta encontrar uno mayor
		while (compareRegistros(clave, &(*itRegistros)) < 0 && itRegistros
				!= listaRegistros->end()) {
			itRegistros++;
		}
		//si llegaste al fin de la lista sin encontra un mayor => bajo por DERECHA
		if (itRegistros == listaRegistros->end()) {
			return this->buscarBloqueRecursivo(clave,
					(*itRegistros).getReferenciai(2), bloqueEncontrado);
		}
		else{

		//sino bajo por la IZQUIERDA del mayor
		return this->buscarBloqueRecursivo(clave,
				(*itRegistros).getReferenciai(1), bloqueEncontrado);
		}
	}
}

ArbolBMas::~ArbolBMas() {
	delete this->archivoNodos;
	delete raiz;
}

