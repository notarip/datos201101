/*
 * ArbolBMas.cpp
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(string path, unsigned int tamanioBloque) {
	this->archivoNodos = new ArchivoBloques(path, tamanioBloque);

	this->tamanioNodo = tamanioBloque;
	ultimaHojaVisitada = NULL;
	ultimoValorBuscado = NULL;

	raiz = new Bloque();

	archivoNodos->grabarBloque(raiz, 0);

}

resultadoOperacion* ArbolBMas::insertar(string clave, unsigned int valor) {
	resultadoOperacion* resultadoRaiz = this->insertarRecursivo(this->raiz,
			clave, valor);

	//tengo que ver los resultados de la raiz - overflow etc

	if (resultadoRaiz->getCodigo() == HUBO_MODIFICACION) {

		try {
			this->archivoNodos->grabarBloque(raiz, 0);

		} catch (ExceptionBloque& e) {
			// => estamos en presencia de un overflow en lo que antes era la raiz => nueva raiz
			Bloque* nuevaRaiz = new Bloque();
			nuevaRaiz->setAtributoBloque(raiz->getAtributoBloque() + 1);
			//la raiz vieja se grabara en un nuevo lugar(posicionOverflow), el bloque producto de split
			//tmb se grabara en alguno libre => se lo asigna el resolverOverflow
			this-> resolverOverflow(raiz, 0 , nuevaRaiz);

			//grabo la nueva raiz en la pos 0
			archivoNodos->grabarBloque(nuevaRaiz,0);
			delete raiz;
			raiz = nuevaRaiz;

		}
	} else {
		//sino no hubo overflow
	}

	delete resultadoRaiz;
	return new resultadoOperacion(OK);
}

resultadoOperacion* ArbolBMas::insertarRecursivo(Bloque* bloqueActual,
		string clave, unsigned int valor) {

	//pregunto por el caso de fin de recurison, ¿Nodo Hoja?
	if (bloqueActual->getAtributoBloque() == 0) {
		//debo escribir el registro, mi papa se encaragara de
		//ver si entre en overflow o no cuando intente grabarme

		/* CHEKEAR SI AGREGAR IMPLICA HACER UN REGISTRO NUEVO O SOLO AGREGAR UN ID*/

		Registro* registroAAgregar = this->crearRegistroClave(clave);
		this->agregarRegistroEnOrden(bloqueActual, *registroAAgregar);

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
		while ( itRegistros	!= listaRegistros->end() && compareRegistros(clave, &(*itRegistros)) >= 0) {
			itRegistros++;
		}
		//si llegaste al fin de la lista sin encontra un mayor => bajo por DERECHA
		if (itRegistros == listaRegistros->end()) {
			itRegistros--;
			nroBloqueABajar = itRegistros->getReferenciai(2);
			bloqueABajar = this->archivoNodos->recuperarBloque(nroBloqueABajar);
		} else {
			//sino bajo por la IZQUIERDA del mayor
			nroBloqueABajar = itRegistros->getReferenciai(1);
			bloqueABajar = this->archivoNodos->recuperarBloque(nroBloqueABajar);
		}

		//llamo a la siguiente recursion
		resultadoHijo = insertarRecursivo(bloqueABajar, clave, valor);

		//volviendo de la recursion debo encargarme de resolver los
		//problemas de mi hijo

		//si fue modificado lo intento grabar
		//-> ojo puede venir exception por overflow

		if (resultadoHijo->getCodigo() == HUBO_MODIFICACION) {
			try {
				this->archivoNodos->grabarBloque(bloqueABajar, nroBloqueABajar);
			} catch (ExceptionBloque& e) {
				// => estamos en presencia de un overflow
				this-> resolverOverflow(bloqueABajar, nroBloqueABajar,
						bloqueActual);
				return new resultadoOperacion(HUBO_MODIFICACION);
				delete resultadoHijo;
			}
		} else {
			//sino hubo overflow
			delete resultadoHijo;
		}

		return new resultadoOperacion(OK);
	}
}

void ArbolBMas::resolverOverflow(Bloque* bloqueOverflow,
		unsigned int nroBloqueOverflow, Bloque* bloqueActual) {

	Bloque* bloqueSplit = new Bloque();
	list<Registro>* listaRegistros = bloqueOverflow->obtenerRegistros();
	Registro registroASplit;

	//realizo el split
	while (archivoNodos->getOcupacionBloque(bloqueSplit) <= 0.50) {
		registroASplit = listaRegistros->back();
		listaRegistros->pop_back();
		this->agregarRegistroEnOrden(bloqueSplit, registroASplit);
		//bloqueSplit->agregarRegistro(registroASplit);
	}

	Registro registroASubir = bloqueSplit->obtenerRegistros()->front();
	string claveASubir = this->consultarClave(&registroASubir);

	//si es un interno lo que sube al bloque hay que sacarlo del bloque split
	if (bloqueOverflow->getAtributoBloque() != 0) {
		bloqueSplit->obtenerRegistros()->pop_front();
	}

	//el siguiente de bloque split es el que era siguiente del bloque overflow
	bloqueSplit->setSiguiente(bloqueOverflow->getSiguiente());
	//dejo el grabado del bloque split para el final por una cuestion de los bloques libres

	//GRABO el nuevo bloque producto de split donde me diga archivoNodos
	unsigned int posLibre = archivoNodos->getBloqueLibre();
	archivoNodos->grabarBloque(bloqueSplit, posLibre);

	//el siguiente a overflow ahora es el bloquesplit
	bloqueOverflow->setSiguiente(posLibre);

	//relocalizacion de la raiz por overflow
	if(nroBloqueOverflow == 0){
		nroBloqueOverflow = this->archivoNodos->getBloqueLibre();
	}

	//GRABO el bloque q antes me dio overflow
	archivoNodos->grabarBloque(bloqueOverflow, nroBloqueOverflow);

	//me agrego a mi bloque un orientador con la referencia a este nuevo nodo
	//el lugar donde va ese registro orientador quedo guardado en itregistros
	//antes de ver por donde bajaba
	// => Ahora debo agregar el registro ahi:
	//con doble referencia si baje por el ultimo
	//con referencia simple y modificando la referencia del que me sigue
	//si baje por alguno del medio.

	Registro* registroOrientador = this->crearRegistroClave(claveASubir);
	registroOrientador->agregarReferencia(nroBloqueOverflow);

	//agrego el orientador en el bloque actual
	list<Registro>::iterator posAgregado = this->agregarRegistroEnOrden(
			bloqueActual, *registroOrientador);

	//si es el ultimo
	if (posAgregado++ == (bloqueActual->obtenerRegistros()->end())) {
		//posAgregado--;
		cout<< boolalpha<<  (posAgregado == bloqueActual->obtenerRegistros()->begin())<<endl;
		posAgregado->agregarReferencia(posLibre);
		//si hay donde meti la doble ref no es el primero => debo borrarle la 2da ref al anterior a mi
		if (posAgregado != bloqueActual->obtenerRegistros()->begin()) {
			posAgregado--;
			posAgregado->getReferencias()->pop_back();
		}
	} else {
		posAgregado++;
		posAgregado->getReferencias()->pop_front();
		posAgregado->getReferencias()->push_front(posLibre);
	}

}

list<Registro>::iterator ArbolBMas::agregarRegistroEnOrden(Bloque* unBloque,
		Registro unRegistro) {
	list<Registro>* listaRegistros = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = listaRegistros->begin();
	string claveRegistroAAgregar = this->consultarClave(&unRegistro);

	/*if (listaRegistros->size() == 0) {
	 listaRegistros->push_front(unRegistro);
	 return listaRegistros->end();
	 }*/

	while (itRegistros != listaRegistros->end() && this->compareRegistros(
			claveRegistroAAgregar, &(*itRegistros)) > 0) {
		itRegistros++;
	}
	//itRegistros++;
	listaRegistros->insert(itRegistros, unRegistro);
	return itRegistros--;
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
		} else {

			//sino bajo por la IZQUIERDA del mayor
			return this->buscarBloqueRecursivo(clave,
					(*itRegistros).getReferenciai(1), bloqueEncontrado);
		}
	}
}

void ArbolBMas::imprimirBloque(fstream* archivo, Bloque* unBloque,
		unsigned int nroBloque) {
	cout << "Bloque " << nroBloque << ": ";
	cout << unBloque->getAtributoBloque() << ", ";
	list<Registro>* registros = unBloque->obtenerRegistros();
	cout << registros->size() << "; ";
	list<Registro>::iterator itRegistros = registros->begin();
	while (itRegistros != registros->end()) {
		if (itRegistros->getReferencias()->size() != 0)
			cout << itRegistros->getReferenciai(1);
		cout << "(" << this->consultarClave(&*itRegistros) << ")";
		itRegistros++;
	}
	itRegistros--;
	if (itRegistros->getReferencias()->size() != 0)
		cout << itRegistros->getReferenciai(2);
	cout << " ";
	if (unBloque->getAtributoBloque() == 0)
		cout << unBloque->getSiguiente();
	cout << endl;
}

void ArbolBMas::exportarRecursivo(fstream* archivo, unsigned int nroBloque,
		unsigned int nivelRecursion) {
	nivelRecursion++;
	Bloque* bloqueLeido = this->archivoNodos->recuperarBloque(nroBloque);
	for (unsigned int i = 0; i < nivelRecursion; i++)
		cout << '\t';
	imprimirBloque(archivo, bloqueLeido, nroBloque);
	if (bloqueLeido->getAtributoBloque() == 0) {
		return;
	}
	list<Registro>* listaReg = bloqueLeido->obtenerRegistros();
	list<Registro>::iterator itRegistros = listaReg->begin();
	while (itRegistros != listaReg->end()) {
		exportarRecursivo(archivo, itRegistros->getReferenciai(1),
				nivelRecursion);
		itRegistros++;
	}
	itRegistros--;
	exportarRecursivo(archivo, itRegistros->getReferenciai(2), nivelRecursion);
	return;
}

void ArbolBMas::exportar(string path) {
	fstream archivo;
	archivo.open(path.c_str(), ios::in | ios::app);
	this->imprimirBloque(&archivo, this->raiz, 0);
	if (raiz->getAtributoBloque() == 0)
		return;

	list<Registro>* listaReg = raiz->obtenerRegistros();
	list<Registro>::iterator itRegistros = listaReg->begin();
	while (itRegistros != listaReg->end()) {
		exportarRecursivo(&archivo, itRegistros->getReferenciai(1), 0);
		itRegistros++;
	}
	itRegistros--;
	exportarRecursivo(&archivo, itRegistros->getReferenciai(2), 0);

	archivo.close();
}

ArbolBMas::~ArbolBMas() {
	delete this->archivoNodos;
	delete raiz;
}

