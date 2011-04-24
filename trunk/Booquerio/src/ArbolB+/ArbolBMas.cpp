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
			this-> resolverOverflow(raiz, 0, nuevaRaiz);

			//grabo la nueva raiz en la pos 0
			archivoNodos->grabarBloque(nuevaRaiz, 0);
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
		while (itRegistros != listaRegistros->end() && compareRegistros(clave,
				&(*itRegistros)) >= 0) {
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
	bloqueSplit->setAtributoBloque(bloqueOverflow->getAtributoBloque());
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
		unsigned int refAPasar =
				bloqueSplit->obtenerRegistros()->front().getReferenciai(1);
		listaRegistros->back().agregarReferencia(refAPasar);
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
	if (nroBloqueOverflow == 0) {
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

	if (consultarClave(registroOrientador) == consultarClave(
			&(bloqueActual->obtenerRegistros()->back()))) {
		posAgregado->agregarReferencia(posLibre);
		//si hay donde meti la doble ref no es el primero => debo borrarle la 2da ref al anterior a mi

		if (bloqueActual->obtenerRegistros()->size() > 1) {
			posAgregado = (bloqueActual->obtenerRegistros()->end());
			posAgregado--;
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

	while (itRegistros != listaRegistros->end() && this->compareRegistros(
			claveRegistroAAgregar, &(*itRegistros)) > 0) {
		itRegistros++;
	}

	listaRegistros->insert(itRegistros, unRegistro);
	--itRegistros;
	return itRegistros;
}

resultadoOperacion* ArbolBMas::eliminarRecursivo(Bloque* bloqueActual,
		string clave, unsigned int valor) {

	bool bajePorUltimo = false;

	//Estoy en la hoja y por lo tanto el Reg a eliminar deberia estar aqui(caso salida de la recursion)
	if (bloqueActual->getAtributoBloque() == 0) {
		list<Registro>* listaReg = bloqueActual->obtenerRegistros();
		list<Registro>::iterator itRegistros = listaReg->begin();

		while (itRegistros != listaReg->end() && compareRegistros(clave,
				&(*itRegistros)) >= 0) {
			itRegistros++;
		}
		// Registro a eliminar encontrado
		if (compareRegistros(clave, &*itRegistros) == 0) {
			listaReg->erase(itRegistros);
			return (new resultadoOperacion(HUBO_MODIFICACION));
		} else {
			return (new resultadoOperacion(NO_ENCONTRADO));
		}
	}

	else {
		list<Registro>* listaReg = bloqueActual->obtenerRegistros();
		Bloque* bloqueABajar = NULL;
		unsigned int nroBloqueABajar;
		//itero por los registro del bloque
		list<Registro>::iterator itRegistros = listaReg->begin();
		//hasta encontrar uno mayor
		while (itRegistros != listaReg->end() && compareRegistros(clave,
				&(*itRegistros)) >= 0) {
			itRegistros++;
		}


		//si llegaste al fin de la lista sin encontra un mayor => bajo por DERECHA
		if (itRegistros == listaReg->end()) {
			itRegistros--;
			bool bajePorUltimo = true;
			nroBloqueABajar = itRegistros->getReferenciai(2);
			bloqueABajar = this->archivoNodos->recuperarBloque(nroBloqueABajar);
		} else {
			//sino bajo por la IZQUIERDA del mayor
			nroBloqueABajar = itRegistros->getReferenciai(1);
			bloqueABajar = this->archivoNodos->recuperarBloque(nroBloqueABajar);
		}




		//llamo a la siguiente recursion
		resultadoOperacion* resultadoHijo = insertarRecursivo(bloqueABajar,
				clave, valor);

		//chekeo lo sucedido con mi hijo y voy a "resolver sus problemas"

		//si no hubo eliminacion sigo enviando ese mensaje hacia arriba
		if (resultadoHijo->getCodigo() == NO_ENCONTRADO)
			return resultadoHijo;

		else {
			//verifico si mi hijo quedo en underflow
			if (this->archivoNodos->getOcupacionBloque(bloqueABajar) < 0.5) {

				//this->resolverUnderflow(bloqueABajar,nroBloqueABajar,bloqueActual, bloqueDer, bloqueIzq);
				delete resultadoHijo;
				return new resultadoOperacion(HUBO_MODIFICACION);

			}
			return new resultadoOperacion (OK);
		}
	}
}

resultadoOperacion* ArbolBMas::eliminar(string clave, unsigned int valor) {
	return 0;
}
void ArbolBMas::resolverUnderflow(Bloque* bloqueUnderflow, unsigned int nroBloqueUnderflow, Bloque* bloqueActual, list<Registro>::iterator itRegistros, bool bajePorUltimo) {
	bool bajePorAnteultimo=false;

	if (!bajePorUltimo){
		itRegistros++;
		bajePorAnteultimo= (itRegistros == bloqueActual->obtenerRegistros()->end());
		itRegistros--;
	}


	unsigned int bloqueDer;
	unsigned int bloqueIzq;
	if(bajePorUltimo){
		bloqueDer = 0;
		bloqueIzq = itRegistros->getReferenciai(1);
	}
	//no baje por el ultimo
	else{
		itRegistros++;
		bloqueDer= itRegistros->getReferenciai(1);
		itRegistros--;
		if (itRegistros==bloqueActual->obtenerRegistros()->begin())
			bloqueIzq= 0;
		else {
			itRegistros--;
			bloqueIzq= (itRegistros)->getReferenciai(1);
			itRegistros++;
		}
	}

	Bloque* bloqueAChequear= NULL;
	bool usoBloqueDer= false;
	bool underflowEnHojas = false;
	//Si el derecho existe lo intento usar para balanceo
	if (bloqueDer!=0) {
		bloqueAChequear= this->archivoNodos->recuperarBloque(bloqueDer);
		usoBloqueDer= true;
	}
	else bloqueAChequear= this->archivoNodos->recuperarBloque(bloqueIzq);

	underflowEnHojas =bloqueUnderflow->getAtributoBloque()==0;

	//Intento Balancear
	// Si es una hoja no hace falta incluir el padre en el balanceo
	if (underflowEnHojas)
		while(archivoNodos->getOcupacionBloque(bloqueUnderflow)<0.5&&bloqueAChequear->obtenerRegistros()->size()>0) {
			//caso derecho
			if (usoBloqueDer) {
				bloqueUnderflow->obtenerRegistros()->push_back(bloqueAChequear->obtenerRegistros()->front());
				bloqueAChequear->obtenerRegistros()->pop_front();
			}
			//caso izquierdo
			else {
				bloqueUnderflow->obtenerRegistros()->push_front(bloqueAChequear->obtenerRegistros()->back());
				bloqueAChequear->obtenerRegistros()->pop_back();
			}
		}
	else {
		// Balanceo en nodo interno (debo balancear con el padre)
		string claveAux=this->consultarClave(&*itRegistros);
		while(archivoNodos->getOcupacionBloque(bloqueUnderflow)<0.5&&bloqueAChequear->obtenerRegistros()->size()>0) {
			//caso derecho
			if (usoBloqueDer) {
				Registro* registroAPasar = this->crearRegistroClave(claveAux);

				registroAPasar->agregarReferencia(bloqueUnderflow->obtenerRegistros()->back().getReferenciai(2));
				bloqueUnderflow->obtenerRegistros()->back().getReferencias()->pop_back();
				registroAPasar->agregarReferencia(bloqueAChequear->obtenerRegistros()->back().getReferenciai(1));

				claveAux = this->consultarClave(&(bloqueAChequear->obtenerRegistros()->front()));

				bloqueUnderflow->obtenerRegistros()->push_back(*registroAPasar);
				bloqueAChequear->obtenerRegistros()->pop_front();

			}
			//caso izquierdo
			else {
				Registro* registroAPasar = this->crearRegistroClave(claveAux);
				registroAPasar->agregarReferencia(bloqueAChequear->obtenerRegistros()->back().getReferenciai(2));

				claveAux= this->consultarClave(&(bloqueAChequear->obtenerRegistros()->back()));

				unsigned int unaRef = bloqueAChequear->obtenerRegistros()->back().getReferenciai(1);
				bloqueAChequear->obtenerRegistros()->pop_back();
				bloqueAChequear->obtenerRegistros()->back().agregarReferencia(unaRef);

				bloqueUnderflow->obtenerRegistros()->push_front(*registroAPasar);
			}
		}
		this->setearClave(&*itRegistros,claveAux);
	}

	//Chequeo si el balanceo fue efectivo
	if (archivoNodos->getOcupacionBloque(bloqueUnderflow)>=0.5 && archivoNodos->getOcupacionBloque(bloqueAChequear)>=0.5){
		//verifico posibles cambios en  padre por el balanceo
		//en el caso de las hojas
		if (underflowEnHojas){
			if (usoBloqueDer){
				this->setearClave(&*itRegistros,this->consultarClave(&(bloqueAChequear->obtenerRegistros()->front())));
			}
			else{
				this->setearClave(&*itRegistros,this->consultarClave(&(bloqueUnderflow->obtenerRegistros()->front())));
			}
		}

		//grabo los nodos producto de el balanceo
		this->archivoNodos->grabarBloque(bloqueUnderflow, nroBloqueUnderflow);
		//caso derecho
		if (usoBloqueDer){
		this->archivoNodos->grabarBloque(bloqueAChequear, bloqueDer);
		}
		//caso izquierdo
		else{
		this->archivoNodos->grabarBloque(bloqueAChequear, bloqueIzq);
		}
		return;
	}//fin balanceo

	//Hubo Fusion de Nodos
	else {
		//underflow en hojas => ya tengo pasados todos al bloque underflow
		if (underflowEnHojas){
			//caso derecho
			if (usoBloqueDer){
				unsigned int refSiguiente = bloqueAChequear->getSiguiente();
				bloqueUnderflow->setSiguiente(refSiguiente);
				//la referencia al underflow se la tengo que pasar al siguiente como 1er ref o al anterior como 2da ref

				if (bajePorAnteultimo){
					itRegistros--;
					itRegistros->agregarReferencia(nroBloqueUnderflow);
					itRegistros++;
				}
				else{
					itRegistros++;
					itRegistros->getReferencias()->pop_front();
					itRegistros->agregarReferencia(nroBloqueUnderflow);
					itRegistros--;
				}

				//elimino el nodo descartado por la fusion
				this->archivoNodos->eliminarBloque(bloqueDer);

				//elimino el registro en el padre que referenciaba a dicho nodo
				bloqueActual->obtenerRegistros()->erase(itRegistros);

			}
			//caso izquierdo
			else{
				//actualizo el siguiente de 2 nodos anteriores al de underflow porque sino queda referenciando como siguiente al nodo borrado
				//para llegar a el debo moverme 1 o 2 posiciones hacia atras con el iterador s/ por donde halla bajado en el underflow
				if (bajePorUltimo){
					itRegistros--;
					unsigned int nrobloqueACambiarSiguiente = itRegistros->getReferenciai(1);
					Bloque* bloqueACambiarSiguiente = this->archivoNodos->recuperarBloque(nrobloqueACambiarSiguiente);
					bloqueACambiarSiguiente->setSiguiente(nroBloqueUnderflow);
					this->archivoNodos->grabarBloque(bloqueACambiarSiguiente,nrobloqueACambiarSiguiente);
					delete bloqueACambiarSiguiente;

					itRegistros++;

					//ademas en este caso debo agregarle una referencia a derecha en el nodo anterior apuntando al nodo underflow
					itRegistros--;
					itRegistros->agregarReferencia(nroBloqueUnderflow);
					itRegistros++;



				}
				else{
					itRegistros--;itRegistros--;
					unsigned int nrobloqueACambiarSiguiente = itRegistros->getReferenciai(1);
					Bloque* bloqueACambiarSiguiente = this->archivoNodos->recuperarBloque(nrobloqueACambiarSiguiente);
					bloqueACambiarSiguiente->setSiguiente(nroBloqueUnderflow);
					this->archivoNodos->grabarBloque(bloqueACambiarSiguiente,nrobloqueACambiarSiguiente);
					delete bloqueACambiarSiguiente;

					itRegistros++;itRegistros++;

				}

				//elimino el nodo descartado por la fusion
				this->archivoNodos->eliminarBloque(bloqueDer);

				//elimino el registro padre que referenciaba a dicho nodo
				bloqueActual->obtenerRegistros()->erase(itRegistros);

			}
		}
		//fusion en internos
		else{
			/* TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo*/
		}


	}//fin fusion


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

string ArbolBMas::imprimirBloque(Bloque* unBloque, unsigned int nroBloque) {
	stringstream conversor;
	string bloqueImpreso = "";
	bloqueImpreso += "Bloque ";
	conversor << nroBloque;
	bloqueImpreso += conversor.str();
	conversor.str("");
	bloqueImpreso += ": ";
	conversor << unBloque->getAtributoBloque();
	bloqueImpreso += conversor.str();
	conversor.str("");
	bloqueImpreso += ", ";
	list<Registro>* registros = unBloque->obtenerRegistros();
	conversor << registros->size();
	bloqueImpreso += conversor.str();
	conversor.str("");
	bloqueImpreso += "; ";
	list<Registro>::iterator itRegistros = registros->begin();
	while (itRegistros != registros->end()) {
		if (itRegistros->getReferencias()->size() != 0) {
			conversor << itRegistros->getReferenciai(1);
			bloqueImpreso += conversor.str();
			conversor.str("");
		}
		bloqueImpreso += "(";
		bloqueImpreso += this->consultarClave(&*itRegistros);
		bloqueImpreso += ")";
		itRegistros++;
	}
	itRegistros--;
	if (itRegistros->getReferencias()->size() != 0) {
		conversor << itRegistros->getReferenciai(2);
		bloqueImpreso += conversor.str();
		conversor.str("");
	}
	bloqueImpreso += " ";
	if (unBloque->getAtributoBloque() == 0) {
		conversor << unBloque->getSiguiente();
		bloqueImpreso += conversor.str();
		conversor.str("");
	}
	bloqueImpreso += '\n';
	return bloqueImpreso;
}

string ArbolBMas::exportarRecursivo(unsigned int nroBloque,
		unsigned int nivelRecursion) {
	nivelRecursion++;
	string textoAExportar = "";
	Bloque* bloqueLeido = this->archivoNodos->recuperarBloque(nroBloque);
	for (unsigned int i = 0; i < nivelRecursion; i++)
		textoAExportar += '\t';
	textoAExportar += imprimirBloque(bloqueLeido, nroBloque);
	if (bloqueLeido->getAtributoBloque() == 0) {
		return textoAExportar;
	}
	list<Registro>* listaReg = bloqueLeido->obtenerRegistros();
	list<Registro>::iterator itRegistros = listaReg->begin();
	while (itRegistros != listaReg->end()) {
		textoAExportar += exportarRecursivo(itRegistros->getReferenciai(1),
				nivelRecursion);
		itRegistros++;
	}
	itRegistros--;
	textoAExportar += exportarRecursivo(itRegistros->getReferenciai(2),
			nivelRecursion);
	return textoAExportar;
}

void ArbolBMas::exportar(string path) {
	string textoAExportar = "";

	textoAExportar += this->imprimirBloque(this->raiz, 0);
	if (raiz->getAtributoBloque() != 0) {

		list<Registro>* listaReg = raiz->obtenerRegistros();
		list<Registro>::iterator itRegistros = listaReg->begin();
		while (itRegistros != listaReg->end()) {
			textoAExportar += exportarRecursivo(itRegistros->getReferenciai(1),
					0);
			itRegistros++;
		}
		itRegistros--;
		textoAExportar += exportarRecursivo(itRegistros->getReferenciai(2), 0);
	}
	fstream archivo;
	archivo.open(path.c_str(), ios::out | ios::app);
	archivo << textoAExportar;
	archivo << "-------------------------------------------------------------"
			<< endl;
	archivo.close();
}

ArbolBMas::~ArbolBMas() {
	delete this->archivoNodos;
	delete raiz;
}

