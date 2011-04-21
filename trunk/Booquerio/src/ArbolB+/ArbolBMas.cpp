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
	return this->insertarRecursivo(this->raiz, clave, valor);

}

resultadoOperacion* ArbolBMas::insertarRecursivo(Bloque* bloqueActual,
		string clave, unsigned int valor) {

	//pregunto por el caso de fin de recurison, ¿Nodo Hoja?
	if (bloqueActual->getAtributoBloque() == 0) {
		//debo escribir el registro, mi papa se encaragara de
		//ver si entre en overflow o no cuando intente grabarme

		/* CHEKEAR SI AGREGAR IMPLICA HACER UN REGISTRO NUEVO O SOLO AGREGAR UN ID*/

		Registro* registroAAgregar = this->crearRegistroClave(clave);

		this->AgregarRegistroEnOrden(bloqueActual,*registroAAgregar);

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
		bloqueSplit->agregarRegistro(registroASplit);
	}

	Registro registroASubir = bloqueSplit->obtenerRegistros()->front();
	string claveASubir = this->consultarClave(&registroASubir);

	//si es un interno lo que sube al bloque hay que sacarlo del bloque split
	if (bloqueOverflow->getAtributoBloque() == 0) {
		bloqueSplit->obtenerRegistros()->pop_front();
	}

	//el siguiente de bloque split es el que era siguiente del bloque overflow
	bloqueSplit->setSiguiente(bloqueOverflow->getSiguiente());

	//GRABO el nuevo bloque producto de split donde me diga archivoNodos
	unsigned int posLibre = archivoNodos->getBloqueLibre();
	archivoNodos->grabarBloque(bloqueSplit, posLibre);

	//el siguiente a overflow ahora es el bloquesplit
	bloqueOverflow->setSiguiente(posLibre);

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
	list<Registro>::iterator posAgregado = this->AgregarRegistroEnOrden(bloqueActual, *registroOrientador);

	//si es el ultimo
	if (posAgregado == (bloqueActual->obtenerRegistros()->end())){
		posAgregado->agregarReferencia(posLibre);
		//al anterior debo borrarle la doble referencia
		posAgregado--;
		posAgregado->getReferencias()->pop_back();
	}
	else{
		posAgregado++;
		posAgregado->getReferencias()->pop_front();
		posAgregado->getReferencias()->push_front(posLibre);
	}



}

list<Registro>::iterator ArbolBMas::AgregarRegistroEnOrden(Bloque* unBloque,Registro unRegistro){
	list<Registro>* listaRegistros = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = listaRegistros->begin();
	string claveRegistroAAgregar = this->consultarClave(&unRegistro);

	while (this->compareRegistros(claveRegistroAAgregar,&(*itRegistros)) <=0 && itRegistros!= listaRegistros->end()){
		itRegistros++;
	}
	listaRegistros->insert(itRegistros,unRegistro);
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

void ArbolBMas::imprimirBloque(fstream* archivo, Bloque* unBloque, unsigned int nroBloque){
	(*archivo)<<"Bloque "<<nroBloque<<": ";
	(*archivo)<<unBloque->getAtributoBloque()<<", ";
	list<Registro>* registros= unBloque->obtenerRegistros();
	(*archivo)<<registros->size()<<"; ";
	list<Registro>::iterator itRegistros= registros->begin();
	while (itRegistros!=registros->end()){
		if (itRegistros->getReferencias()->size()!=0)
			(*archivo)<<itRegistros->getReferenciai(1);
		(*archivo)<<"("<<this->consultarClave(&*itRegistros)<<")";
		itRegistros++;
	}
	if (itRegistros->getReferencias()->size()!=0)
		(*archivo)<<itRegistros->getReferenciai(2);
	(*archivo)<<" ";
	if (unBloque->getAtributoBloque()!=0)
		(*archivo)<<unBloque->getAtributoBloque();
	(*archivo)<<endl;
}

void ArbolBMas::exportarRecursivo(fstream* archivo, unsigned int nroBloque, unsigned int nivelRecursion){
	nivelRecursion++;
	Bloque* bloqueLeido= this->archivoNodos->recuperarBloque(nroBloque);
	for(unsigned int i=0;i<nivelRecursion;i++)
		(*archivo)<<'\t';
	imprimirBloque(archivo, bloqueLeido, nroBloque);
	if (bloqueLeido->getAtributoBloque()== 0) {
		nivelRecursion--;
		return;
	}
	list<Registro>* listaReg= bloqueLeido->obtenerRegistros();
		list<Registro>::iterator itRegistros= listaReg->begin();
		while (itRegistros!=listaReg->end()) {
			exportarRecursivo(archivo, itRegistros->getReferenciai(1), nivelRecursion);
			itRegistros++;
		}
		exportarRecursivo(archivo, itRegistros->getReferenciai(2), nivelRecursion);

	nivelRecursion--;
	return;
}

void ArbolBMas::exportar(string path){
	fstream archivo;
	archivo.open(path.c_str(),ios::in | ios::app);
	this->imprimirBloque(&archivo,this->raiz,0);
	list<Registro>* listaReg= raiz->obtenerRegistros();
	list<Registro>::iterator itRegistros= listaReg->begin();
	while (itRegistros!=listaReg->end()) {
		exportarRecursivo(&archivo, itRegistros->getReferenciai(1), 1);
		itRegistros++;
	}
	exportarRecursivo(&archivo, itRegistros->getReferenciai(2), 1);

	archivo.close();
}

ArbolBMas::~ArbolBMas() {
	delete this->archivoNodos;
	delete raiz;
}

