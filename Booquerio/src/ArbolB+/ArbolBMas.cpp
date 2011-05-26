/*
 * ArbolBMas.cpp
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(string path, unsigned int tamanioBloque) {

//	this->archivoNodos = new ArchivoBloques(path, tamanioBloque);
//
//	this->tamanioNodo = tamanioBloque;
//
//	if (Util().existeArchivo(path)){
//		raiz = obtenerBloque(0);
//	}
//	else{
//		raiz = new Bloque();
//		guardarBloque(raiz, 0);
//	}
//	ultimaHojaVisitada = raiz;

}

resultadoOperacion ArbolBMas::insertar(string clave, unsigned int valor) {

	resultadoOperacion resultadoRaiz (OK);
	resultadoRaiz= this->insertarRecursivo(this->raiz, clave, valor);

	//tengo que ver los resultados de la raiz - overflow etc

	if (resultadoRaiz.getCodigo() == HUBO_MODIFICACION) {

		try {
			guardarBloque(raiz, 0);

		} catch (ExceptionBloque& e) {
			// => estamos en presencia de un overflow en lo que antes era la raiz => nueva raiz
			Bloque* nuevaRaiz = new Bloque();
			nuevaRaiz->setAtributoBloque(raiz->getAtributoBloque() + 1);
			//la raiz vieja se grabara en un nuevo lugar(posicionOverflow), el bloque producto de split
			//tmb se grabara en alguno libre => se lo asigna el resolverOverflow
			this-> resolverOverflow(raiz, 0, nuevaRaiz);

			//grabo la nueva raiz en la pos 0
			guardarBloque(nuevaRaiz, 0);
			delete raiz;
			if(ultimaHojaVisitada==raiz)
				ultimaHojaVisitada= nuevaRaiz;
			raiz = nuevaRaiz;

		}
	} else {
		//sino no hubo overflow
	}
	if (resultadoRaiz.getCodigo() == CLAVE_REPETIDA)
		return resultadoOperacion(CLAVE_REPETIDA);

	return resultadoOperacion(OK);
}

resultadoOperacion ArbolBMas::insertarRecursivo(Bloque* bloqueActual,
		string clave, unsigned int valor) {

	//pregunto por el caso de fin de recursion, ¿Nodo Hoja?
	if (bloqueActual->getAtributoBloque() == 0) {
		//debo escribir el registro, mi papa se encargara de
		//ver si entre en overflow o no cuando intente grabarme

		Registro* registroAAgregar = this->crearRegistroClave(clave, valor);
		try{
			this->agregarRegistroEnOrden(bloqueActual, *registroAAgregar);
		}
		catch (ExceptionDuplicacionClaves& e){
			//agregue un registro con clave duplicado
			delete registroAAgregar;
			return resultadoOperacion(CLAVE_REPETIDA);
		}
		delete registroAAgregar;
		return resultadoOperacion(HUBO_MODIFICACION);

	}
	//sino soy hoja entonces debo ver por donde sigo la recursion y
	//manejar eventos que hayan sucedido a mi hijo

	else {

		resultadoOperacion resultadoHijo = NULL;
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
			bloqueABajar = obtenerBloque(nroBloqueABajar);
		} else {
			//sino bajo por la IZQUIERDA del mayor
			nroBloqueABajar = itRegistros->getReferenciai(1);
			bloqueABajar = obtenerBloque(nroBloqueABajar);
		}

		//llamo a la siguiente recursion
		resultadoHijo = insertarRecursivo(bloqueABajar, clave, valor);

		//volviendo de la recursion debo encargarme de resolver los
		//problemas de mi hijo

		//si fue modificado lo intento grabar
		//-> ojo puede venir exception por overflow

		if (resultadoHijo.getCodigo() == HUBO_MODIFICACION) {
			try {
				guardarBloque(bloqueABajar, nroBloqueABajar);
			} catch (ExceptionBloque& e) {
				// => estamos en presencia de un overflow
				this-> resolverOverflow(bloqueABajar, nroBloqueABajar,
						bloqueActual);
				delete bloqueABajar;
				return resultadoOperacion(HUBO_MODIFICACION);
			}
		}
		delete bloqueABajar;
		if (resultadoHijo.getCodigo() == CLAVE_REPETIDA){
			return resultadoOperacion(CLAVE_REPETIDA);
		}

		return resultadoOperacion(OK);
	}
}

void ArbolBMas::resolverOverflow(Bloque* bloqueOverflow,
		unsigned int nroBloqueOverflow, Bloque* bloqueActual) {

	Bloque* bloqueSplit = new Bloque();
	bloqueSplit->setAtributoBloque(bloqueOverflow->getAtributoBloque());
	list<Registro>* listaRegistros = bloqueOverflow->obtenerRegistros();
	Registro registroASplit;
	//realizo el split
	while (obtenerOcupacionBloque(bloqueSplit) <= PORCENTAJE_OVERFLOW) {
		registroASplit = listaRegistros->back();
		listaRegistros->pop_back();
		this->agregarRegistroEnOrden(bloqueSplit, registroASplit);
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
	guardarBloque(bloqueSplit, posLibre);
	delete bloqueSplit;

	//el siguiente a overflow ahora es el bloquesplit
	bloqueOverflow->setSiguiente(posLibre);

	//relocalizacion de la raiz por overflow
	if (nroBloqueOverflow == 0) {
		nroBloqueOverflow = this->archivoNodos->getBloqueLibre();
	}

	//GRABO el bloque q antes me dio overflow
	guardarBloque(bloqueOverflow, nroBloqueOverflow);

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
	delete registroOrientador;
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
	if(itRegistros != listaRegistros->end()&& this->compareRegistros(claveRegistroAAgregar,&(*itRegistros)) == 0)
		throw ExceptionDuplicacionClaves();

	listaRegistros->insert(itRegistros, unRegistro);
	--itRegistros;
	return itRegistros;
}

resultadoOperacion ArbolBMas::eliminar(string clave) {
	resultadoOperacion resultadoRaiz =  this->eliminarRecursivo(raiz,clave);
	if (resultadoRaiz.getCodigo() == HUBO_MODIFICACION){
		guardarBloque(raiz, 0);
		return resultadoOperacion(OK);
	}
	return resultadoRaiz;
}

resultadoOperacion ArbolBMas::eliminarRecursivo(Bloque* bloqueActual,string clave) {

	bool bajePorUltimo = false;

	//Estoy en la hoja y por lo tanto el Reg a eliminar deberia estar aqui(caso salida de la recursion)

	if (bloqueActual->getAtributoBloque() == 0) {
		list<Registro>* listaReg = bloqueActual->obtenerRegistros();
		list<Registro>::iterator itRegistros = listaReg->begin();

		while (itRegistros != listaReg->end() && compareRegistros(clave,
				&(*itRegistros)) > 0) {
			itRegistros++;
		}
		// Registro a eliminar encontrado
		if (itRegistros != listaReg->end() && compareRegistros(clave, &*itRegistros) == 0) {
			listaReg->erase(itRegistros);
			return (resultadoOperacion(HUBO_MODIFICACION));
		} else {
			return (resultadoOperacion(NO_ENCONTRADO));
		}
	}

	//Caso Recursivo
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
			bajePorUltimo = true;
			nroBloqueABajar = itRegistros->getReferenciai(2);
			bloqueABajar = obtenerBloque(nroBloqueABajar);
		} else {
			//sino bajo por la IZQUIERDA del mayor
			nroBloqueABajar = itRegistros->getReferenciai(1);
			bloqueABajar = obtenerBloque(nroBloqueABajar);
		}

		//llamo a la siguiente recursion
		resultadoOperacion resultadoHijo = eliminarRecursivo(bloqueABajar,
				clave);

		//chekeo lo sucedido con mi hijo y voy a "resolver sus problemas"

		if (resultadoHijo.getCodigo() == OK){
			delete bloqueABajar;
			return resultadoOperacion (OK);
		}

		if (resultadoHijo.getCodigo() == NO_ENCONTRADO){
			delete bloqueABajar;
			return resultadoOperacion (NO_ENCONTRADO);
		}

		else {
			//verifico si mi hijo quedo en underflow
			if (obtenerOcupacionBloque(bloqueABajar) < PORCENTAJE_UNDERFLOW) {
				this->resolverUnderflow(bloqueABajar,nroBloqueABajar,bloqueActual, itRegistros, bajePorUltimo);
				return resultadoOperacion(HUBO_MODIFICACION);

			}
			else {
				guardarBloque(bloqueABajar, nroBloqueABajar);
			}
			delete bloqueABajar;
			return resultadoOperacion (OK);
		}
	}
}

void ArbolBMas::resolverUnderflow(Bloque* bloqueUnderflow, unsigned int nroBloqueUnderflow, Bloque* bloqueActual, list<Registro>::iterator itRegistros, bool bajePorUltimo) {
	bool bajePorAnteultimo=false;
	bool bajePorPrimero = itRegistros==bloqueActual->obtenerRegistros()->begin();

	if (!bajePorUltimo){
		itRegistros++;
		bajePorAnteultimo= (itRegistros == bloqueActual->obtenerRegistros()->end());
		itRegistros--;
	}

	unsigned int bloqueDer;
	unsigned int bloqueIzq;
	if(bajePorPrimero && !bajePorUltimo){
		if (bajePorAnteultimo){//=>unico
			bloqueIzq = 0;
			bloqueDer = itRegistros->getReferenciai(2);
		}
		else{//no es el unico
			bloqueIzq= 0;
			itRegistros++;
			bloqueDer= itRegistros->getReferenciai(1);
			itRegistros--;
		}
	}else{
		if(bajePorUltimo){
				bloqueDer = 0;
				bloqueIzq = itRegistros->getReferenciai(1);
		}
		else{
			if(bajePorAnteultimo && !bajePorPrimero){
				bloqueDer = itRegistros->getReferenciai(2);
				itRegistros--;
				bloqueIzq = itRegistros->getReferenciai(1);
				itRegistros++;
			}
			else{//caso comun
				itRegistros++;
				bloqueDer= itRegistros->getReferenciai(1);
				itRegistros--;
				itRegistros--;
				bloqueIzq= (itRegistros)->getReferenciai(1);
				itRegistros++;
			}
		}
	}

	Bloque* bloqueAChequear= NULL;
	bool usoBloqueDer= false;
	bool underflowEnHojas = false;
	//Si el derecho existe lo intento usar para balanceo, sino usare el izquierdo
	if (bloqueDer!=0) {
		bloqueAChequear= obtenerBloque(bloqueDer);
		usoBloqueDer= true;
	}
	else bloqueAChequear= obtenerBloque(bloqueIzq);

	underflowEnHojas =bloqueUnderflow->getAtributoBloque()==0;

	//Intento Balancear
	// ***********************Si es un BALANCEO HOJA no hace falta incluir el padre en el balanceo****************************
	if (underflowEnHojas) {
		while((obtenerOcupacionBloque(bloqueUnderflow)<PORCENTAJE_UNDERFLOW || obtenerOcupacionBloque(bloqueAChequear)<PORCENTAJE_UNDERFLOW) &&bloqueAChequear->obtenerRegistros()->size()>0) {
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
		//subo el ultimo que pase arriba
		string claveParaArriba;
	}
	else {

		// *******************BALANCEO en NODO INTERNO (debo balancear con el padre)****************************************
		string claveAux=this->consultarClave(&*itRegistros);


		while((obtenerOcupacionBloque(bloqueUnderflow)<PORCENTAJE_UNDERFLOW || obtenerOcupacionBloque(bloqueAChequear)<PORCENTAJE_UNDERFLOW) && bloqueAChequear->obtenerRegistros()->size()>0) {
			//caso derecho
			if (usoBloqueDer) {
				Registro* registroAPasar = this->crearRegistroClave(claveAux);

				//ver el tema que ante una fusion un bloque alla quedado en 0 para fusionarse
				if(bloqueUnderflow->obtenerRegistros()->size() == 0){
					registroAPasar->agregarReferencia(bloqueUnderflow->getSiguiente());
					bloqueUnderflow->setSiguiente(0);
				}
				else{
				registroAPasar->agregarReferencia(bloqueUnderflow->obtenerRegistros()->back().getReferenciai(2));
				bloqueUnderflow->obtenerRegistros()->back().getReferencias()->pop_back();
				}

				registroAPasar->agregarReferencia(bloqueAChequear->obtenerRegistros()->front().getReferenciai(1));

				claveAux = this->consultarClave(&(bloqueAChequear->obtenerRegistros()->front()));

				bloqueUnderflow->obtenerRegistros()->push_back(*registroAPasar);
				delete registroAPasar;

				//antes de hacer pop del ultimo me guardo la referencia del nodo de abajo
				if(bloqueAChequear->obtenerRegistros()->size() == 1)
					bloqueAChequear->setSiguiente(bloqueAChequear->obtenerRegistros()->front().getReferenciai(2));
				bloqueAChequear->obtenerRegistros()->pop_front();
			}
			//caso izquierdo
			else {
				Registro* registroAPasar = this->crearRegistroClave(claveAux);

				registroAPasar->agregarReferencia(bloqueAChequear->obtenerRegistros()->back().getReferenciai(2));

				//ver el tema que ante una fusion un bloque alla quedado en 0 para fusionarse
				if ( bloqueUnderflow->obtenerRegistros()->size() == 0){
					registroAPasar->agregarReferencia(bloqueUnderflow->getSiguiente());
					bloqueUnderflow->setSiguiente(0);
				}

				claveAux= this->consultarClave(&(bloqueAChequear->obtenerRegistros()->back()));

				unsigned int unaRef = bloqueAChequear->obtenerRegistros()->back().getReferenciai(1);
				//antes de hacer pop del ultimo me guardo la referencia del nodo de abajo
				if(bloqueAChequear->obtenerRegistros()->size() == 1)
					bloqueAChequear->setSiguiente(bloqueAChequear->obtenerRegistros()->front().getReferenciai(1));

				bloqueAChequear->obtenerRegistros()->pop_back();

				if(bloqueAChequear->obtenerRegistros()->size() > 0){
				bloqueAChequear->obtenerRegistros()->back().agregarReferencia(unaRef);
				}

				bloqueUnderflow->obtenerRegistros()->push_front(*registroAPasar);
				delete registroAPasar;
			}
		}
		this->setearClave(&*itRegistros,claveAux);
	}

	//Chequeo si el balanceo fue efectivo
	if (obtenerOcupacionBloque(bloqueUnderflow)>=PORCENTAJE_UNDERFLOW && obtenerOcupacionBloque(bloqueAChequear)>=PORCENTAJE_UNDERFLOW){
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
		guardarBloque(bloqueUnderflow, nroBloqueUnderflow);
		//caso derecho
		if (usoBloqueDer){
		guardarBloque(bloqueAChequear, bloqueDer);
		}
		//caso izquierdo
		else{
		guardarBloque(bloqueAChequear, bloqueIzq);
		}
		delete bloqueAChequear;
		return;
	}//fin balanceo

	//Hubo FUSION de Nodos(balanceo no efectivo)
	else {
		//***********************FUSION HOJAS -> underflow en hojas => ya tengo pasados todos al bloque underflow*******************
		if (underflowEnHojas){
			//caso derecho(fusion hojas)
			if (usoBloqueDer){
				unsigned int refSiguiente = bloqueAChequear->getSiguiente();
				bloqueUnderflow->setSiguiente(refSiguiente);
				//la referencia al underflow se la tengo que pasar al siguiente como 1er ref o al anterior como 2da ref

				if (bajePorAnteultimo && bloqueActual->obtenerRegistros()->size() > 1){
					itRegistros--;
					itRegistros->agregarReferencia(nroBloqueUnderflow);
					itRegistros++;
				}
				else if(bloqueActual->obtenerRegistros()->size() == 1){
					//por cuestion de comodidad guardo la referencia al bloque underflow ahi para no perderla
					bloqueActual->setSiguiente(nroBloqueUnderflow);
				}
					else{
						itRegistros++;
						itRegistros->getReferencias()->pop_front();
						itRegistros->getReferencias()->push_front(nroBloqueUnderflow);
						itRegistros--;
					}

				//elimino el nodo descartado por la fusion
				this->archivoNodos->eliminarBloque(bloqueDer);

				//elimino el registro en el padre que referenciaba a dicho nodo, pero guardo una referencia a este nodo hoja
				//producto de la fusion, sera usado en probables fusion es de nodos internos(hecho unas lineas arriba)
				bloqueActual->obtenerRegistros()->erase(itRegistros);

				//grabo el bloque underflow casos excepcionales cuando termino la recursion guardo al actual y a los hijos
				if (bloqueActual == raiz){
					if (bloqueActual->obtenerRegistros()->size() == 0){
					//hay nueva raiz
						guardarBloque(bloqueUnderflow,0);
						this->archivoNodos->eliminarBloque(nroBloqueUnderflow);
						delete raiz;
						raiz = bloqueUnderflow;
						delete bloqueAChequear;
						return;
					}
					else{
					//sigue la raiz de antes
						guardarBloque(bloqueActual,0);
						guardarBloque(bloqueUnderflow,nroBloqueUnderflow);
					}
				}
				else{
				guardarBloque(bloqueUnderflow,nroBloqueUnderflow);
				}

			}
			//caso izquierdo(fusion hojas)
			else{
				//la clave aca va a ser grabar el nodo underflow en el lugar del nodo que queda vacio por la fusion, y liberar
				//el lugar donde se guardaba el nodo de underflow de esta manera no perdere la referencia como siguiente
				//que podia tener en cualquier otra parte del arbol , ni siquiera se si esta a mi izquierda

				if (bajePorUltimo){
					if (bloqueActual->obtenerRegistros()->size() > 1){
					//bajo estas 2 condiciones seguro tengo un registro a izquierda
					//debo agregarle una referencia a derecha en el nodo anterior apuntando al nodo underflow(ojo ahora se guardara
					// en la posicion donde se encontraba el bloque q ahora quedo vacio
					itRegistros--;
					itRegistros->agregarReferencia(bloqueIzq);
					itRegistros++;
					}
					//si era el unico registro
					else if (bloqueActual->obtenerRegistros()->size() == 1){
						//por cuestion de comodidad guardo la referencia al bloque underflow ahi para no perderla
						bloqueActual->setSiguiente(bloqueIzq);
						}
				}
				//sino baje por el ultimo
				else{
					// al de la derecha le toco la referencia para que siga apuntando a donde voy a guardar el bloq.underflow
					itRegistros++;
					itRegistros->getReferencias()->pop_front();
					itRegistros->getReferencias()->push_front(bloqueIzq);
					itRegistros--;
				}

				//elimino el nodo descartado por la fusion
				this->archivoNodos->eliminarBloque(nroBloqueUnderflow);

				//elimino el registro padre que referenciaba a dicho nodo
				bloqueActual->obtenerRegistros()->erase(itRegistros);

				//grabo el bloque underflow casos excepcionales cuando termino la recursion guardo al actual y a los hijos
				if (bloqueActual == raiz){
					if (bloqueActual->obtenerRegistros()->size() == 0){
					//hay nueva raiz
						guardarBloque(bloqueUnderflow,0);
						this->archivoNodos->eliminarBloque(nroBloqueUnderflow);
						delete raiz;
						raiz = bloqueUnderflow;
					}
					else{
					//sigue la raiz de antes
						guardarBloque(bloqueActual,0);
						guardarBloque(bloqueUnderflow,bloqueIzq);
					}
				}
				else{
				//(recordar el tip se guarda en el otro espacio)
				guardarBloque(bloqueUnderflow,bloqueIzq);
				}

			}
		}
		//**********************************************FUSION EN NODOS INTERNOS***********************************************
		else{
			//caso derecho
			if (usoBloqueDer){
				//bajo el que me quedo del balanceo arriba al hijo
				Registro* registroQueBaja = this->crearRegistroClave(this->consultarClave(&*itRegistros));
				registroQueBaja->agregarReferencia(bloqueUnderflow->obtenerRegistros()->back().getReferenciai(2));
				bloqueUnderflow->obtenerRegistros()->back().getReferencias()->pop_back();
				registroQueBaja->agregarReferencia(bloqueAChequear->getSiguiente());
				bloqueAChequear->setSiguiente(0);

				bloqueUnderflow->agregarRegistro(*registroQueBaja);
				delete registroQueBaja;

				//aca tengo resuelto el pasaje de todos los registros al underflow
				// => de aca en mas misma logica de la fusion de nodos hojas

				//la referencia al underflow se la tengo que pasar al siguiente como 1er ref o al anterior como 2da ref
				if (bajePorAnteultimo && bloqueActual->obtenerRegistros()->size() > 1){
					itRegistros--;
					itRegistros->agregarReferencia(nroBloqueUnderflow);
					itRegistros++;
				}
				else if(bloqueActual->obtenerRegistros()->size() == 1){
					//por cuestion de comodidad guardo la referencia al bloque underflow ahi para no perderla
					bloqueActual->setSiguiente(nroBloqueUnderflow);
				}
					else{
						itRegistros++;
						itRegistros->getReferencias()->pop_front();
						itRegistros->getReferencias()->push_front(nroBloqueUnderflow);
						itRegistros--;
					}
				//elimino el nodo descartado por la fusion
				this->archivoNodos->eliminarBloque(bloqueDer);

				//elimino el registro en el padre que referenciaba a dicho nodo,
				bloqueActual->obtenerRegistros()->erase(itRegistros);

				//grabo el bloque underflow casos excepcionales cuando termino la recursion guardo al actual y a los hijos
				if (bloqueActual == raiz){
					if (bloqueActual->obtenerRegistros()->size() == 0){
					//hay nueva raiz
						guardarBloque(bloqueUnderflow,0);
						this->archivoNodos->eliminarBloque(nroBloqueUnderflow);
						delete raiz;
						raiz = bloqueUnderflow;
					}
					else{
					//sigue la raiz de antes
						guardarBloque(bloqueActual,0);
						guardarBloque(bloqueUnderflow,nroBloqueUnderflow);
					}
				}
				else{
				//grabo el bloque underflow
				guardarBloque(bloqueUnderflow,nroBloqueUnderflow);
				}
			}

			//caso izquierdo
			else{
				//bajo el que me quedo del balanceo arriba al hijo
				Registro* registroQueBaja = this->crearRegistroClave(this->consultarClave(&*itRegistros));
				registroQueBaja->agregarReferencia(bloqueAChequear->getSiguiente());
				bloqueAChequear->setSiguiente(0);

				bloqueUnderflow->obtenerRegistros()->push_front(*registroQueBaja);
				delete registroQueBaja;

				//aca tengo resuelto el pasaje de todos los registros al underflow
				// => de aca en mas misma logica de la fusion de nodos hojas

				if (bajePorUltimo){
					if (bloqueActual->obtenerRegistros()->size() > 1){
					//bajo estas 2 condiciones seguro tengo un registro a izquierda
					//debo agregarle una referencia a derecha en el nodo anterior apuntando al nodo underflow(ojo ahora se guardara
					// en la posicion donde se encontraba el bloque q ahora quedo vacio
					itRegistros--;
					itRegistros->agregarReferencia(bloqueIzq);
					itRegistros++;
					}
				//si era el unico registro
				else if (bloqueActual->obtenerRegistros()->size() == 1){
				//por cuestion de comodidad guardo la referencia al bloque underflow ahi para no perderla
						bloqueActual->setSiguiente(bloqueIzq);
						}
				}
				//sino baje por el ultimo
				else{
				// al de la derecha le toco la referencia para que siga apuntando a donde voy a guardar el bloq.underflow
					itRegistros++;
					itRegistros->getReferencias()->pop_front();
					itRegistros->getReferencias()->push_front(bloqueIzq);
					itRegistros--;
				}

				//elimino el nodo descartado por la fusion
				this->archivoNodos->eliminarBloque(nroBloqueUnderflow);

				//elimino el registro padre que referenciaba a dicho nodo
				bloqueActual->obtenerRegistros()->erase(itRegistros);

				//grabo el bloque underflow casos excepcionales cuando termino la recursion guardo al actual y a los hijos
				if (bloqueActual == raiz){
					if (bloqueActual->obtenerRegistros()->size() == 0){
					//hay nueva raiz
						guardarBloque(bloqueUnderflow,0);
						this->archivoNodos->eliminarBloque(bloqueIzq);
						delete raiz;
						raiz = bloqueUnderflow;
					}
					else{
					//sigue la raiz de antes
						guardarBloque(bloqueActual,0);
						guardarBloque(bloqueUnderflow,bloqueIzq);
					}
				}
				else{
				//grabo el bloque underflow(recordar el tip se guarda en el otro espacio)
				guardarBloque(bloqueUnderflow,bloqueIzq);
				}
			}
		}
		delete bloqueAChequear;
	}//fin fusion
}

Registro* ArbolBMas::siguiente() {
	this->ultimoRegistroVisitado++;
	if(ultimoRegistroVisitado!=ultimaHojaVisitada->obtenerRegistros()->end()){
		return &*ultimoRegistroVisitado;
	}
	else{
		if(ultimaHojaVisitada->getSiguiente()!=0){
			unsigned int nroBloqueSig= ultimaHojaVisitada->getSiguiente();
			delete ultimaHojaVisitada;
			ultimaHojaVisitada= obtenerBloque(nroBloqueSig);
			ultimoRegistroVisitado= ultimaHojaVisitada->obtenerRegistros()->begin();
			return &(ultimaHojaVisitada->obtenerRegistros()->front());
		}
		else {
			ultimoRegistroVisitado--;
			return NULL;
		}
	}
}


Bloque* ArbolBMas::buscarBloque(string clave) {
	list<Registro>* listaReg = raiz->obtenerRegistros();

	//si la raiz es una hoja (nivel 0) => solo puede estar ahi
	if (raiz->getAtributoBloque() == 0) {
		this->ultimaHojaVisitada = raiz;
		return raiz;
	}

	// itero sobre los registros de la raiz
	list<Registro>::iterator itReg = listaReg->begin();

	//hasta encontrar uno mayor
	while (itReg != listaReg->end() && compareRegistros(clave, &(*itReg)) >= 0) {
		itReg++;
	}

	//si llegaste al fin de la lista sin encontra un mayor => bajo por derecha
	if (itReg == listaReg->end()) {
		itReg--;
		return this->buscarBloqueRecursivo(clave, (*itReg).getReferenciai(2));
	}

	//sino bajo por la izquierda del mayor
	return this->buscarBloqueRecursivo(clave, (*itReg).getReferenciai(1));
}

Bloque* ArbolBMas::buscarBloqueRecursivo(string clave, unsigned int refBloque) {

	//bajo el bloque a mem
	Bloque* esteBloque = obtenerBloque(refBloque);

	//si se trata de un nodo hoja
	if (esteBloque->getAtributoBloque() == 0) {
		this->ultimaHojaVisitada = esteBloque;
		return esteBloque;

		//si es un nodo Interno
	} else {
		list<Registro>* listaRegistros = esteBloque->obtenerRegistros();

		//itero por los registro del bloque
		list<Registro>::iterator itRegistros = listaRegistros->begin();
		//hasta encontrar uno mayor
		while (itRegistros != listaRegistros->end() && compareRegistros(clave, &(*itRegistros)) >= 0) {
					itRegistros++;
		}
		//si llegaste al fin de la lista sin encontra un mayor => bajo por DERECHA
		if (itRegistros == listaRegistros->end()) {
			itRegistros--;
			return this->buscarBloqueRecursivo(clave, (*itRegistros).getReferenciai(2));
		}
		else {

			//sino bajo por la IZQUIERDA del mayor
			return this->buscarBloqueRecursivo(clave, (*itRegistros).getReferenciai(1));
		}
	}
}

Registro* ArbolBMas::buscarRegistro(string clave, resultadoOperacion* resultOperacion){
	Bloque* bloqueEncontrado= this->buscarBloque(clave);
	list<Registro>* listaRegistros= bloqueEncontrado->obtenerRegistros();
	list<Registro>::iterator itRegistros= listaRegistros->begin();
	while (itRegistros != listaRegistros->end() && compareRegistros(clave, &(*itRegistros)) > 0) {
		itRegistros++;
	}
	if(itRegistros!=listaRegistros->end()){
		ultimoRegistroVisitado= itRegistros;
		if (compareRegistros(clave, &(*itRegistros)) == 0)
			resultOperacion->setCodigo(ENCONTRADO);
		else resultOperacion->setCodigo(NO_ENCONTRADO);
		return &*itRegistros;
	}
	else {
		resultOperacion->setCodigo(NO_ENCONTRADO);
		unsigned int nroBloqueSig= bloqueEncontrado->getSiguiente();
		if (nroBloqueSig != 0){
			bloqueEncontrado= obtenerBloque(nroBloqueSig);
			delete ultimaHojaVisitada;
			ultimaHojaVisitada= bloqueEncontrado;
			ultimoRegistroVisitado= bloqueEncontrado->obtenerRegistros()->begin();
			return &(bloqueEncontrado->obtenerRegistros()->front());
		}
		else {
			itRegistros--;
			ultimoRegistroVisitado=itRegistros;
			return &*itRegistros;
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
	bloqueImpreso += "Nivel ";
	conversor << unBloque->getAtributoBloque();
	bloqueImpreso += conversor.str();
	conversor.str("");
	bloqueImpreso += ", ";
	bloqueImpreso += "Cant. Elem.";
	list<Registro>* registros = unBloque->obtenerRegistros();
	conversor << registros->size();
	bloqueImpreso += conversor.str();
	conversor.str("");
	bloqueImpreso += "; ";
	list<Registro>::iterator itRegistros = registros->begin();
	if (unBloque->getAtributoBloque()!=0){
		while (itRegistros != registros->end()) {
			if (itRegistros->getReferencias()->size() != 0) {
				conversor << itRegistros->getReferenciai(1);
				bloqueImpreso += conversor.str();
				conversor.str("");
			}
			bloqueImpreso += "(";
			if(itRegistros->getString()!=""){
				if(itRegistros->getAtributosEnteros()->size()>0){
					conversor << itRegistros->getAtributosEnteros()->front();
					bloqueImpreso += conversor.str();
					conversor.str("");
				}
				else bloqueImpreso += "0";
				bloqueImpreso += "|";
			}
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
	}
	else {
		while (itRegistros != registros->end()) {
					bloqueImpreso += "(";
					if(itRegistros->getString()!=""){
						if(itRegistros->getAtributosEnteros()->size()>0){
							conversor << itRegistros->getAtributosEnteros()->front();
							bloqueImpreso += conversor.str();
							conversor.str("");
						}
						else bloqueImpreso += "0";
						bloqueImpreso += "|";
					}

					bloqueImpreso += this->consultarClave(&*itRegistros);
					bloqueImpreso += "|";
					conversor << itRegistros->getReferenciai(1);
					bloqueImpreso += conversor.str();
					conversor.str("");
					bloqueImpreso += ")";
					itRegistros++;
		}
		bloqueImpreso += "; ";
		bloqueImpreso += "Bloque Siguiente: ";
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
		delete bloqueLeido;
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
	delete bloqueLeido;
	return textoAExportar;
}

void ArbolBMas::exportar(string path) {
	string textoAExportar = "";
	Bloque* bloqueLeido = this->archivoNodos->recuperarBloque(0);
	textoAExportar += this->imprimirBloque(bloqueLeido, 0);
	if (raiz->getAtributoBloque() != 0) {

		list<Registro>* listaReg = raiz->obtenerRegistros();
		list<Registro>::iterator itRegistros = listaReg->begin();
		while (itRegistros != listaReg->end()) {
			textoAExportar += exportarRecursivo(itRegistros->getReferenciai(1),
					0);
			itRegistros++;
		}
		itRegistros--;
		unsigned int ref2 = itRegistros->getReferenciai(2);
		textoAExportar += exportarRecursivo(ref2, 0);
	}
	textoAExportar += "\nNOTA: Entre parentesis se listan, respectivamente, la cantidad de caracteres "
			"que la clave comparte con la previa (cuando corresponda), el remanente de la clave que "
			"esta almacenada en el registro y la referencia al bloque en el que se encuentra la lista "
			"de IDs correspondientes al mismo en el caso de tratarse de un arbol cuya clave es string.\n"
			"En caso de tener el arbol que indiza al archivo de libros se ubica primero la clave numerica "
			"y luego el offset en el cual se ubica el libro en dicho archivo.\n";
	fstream archivo;
	archivo.open(path.c_str(), ios::out | ios::app);
	archivo << textoAExportar;
	archivo << "-------------------------------------------------------------"
			<< endl;
	archivo.close();
	delete bloqueLeido;
}

float ArbolBMas::obtenerOcupacionBloque(Bloque* unBloque){
	return this->archivoNodos->getOcupacionBloque(unBloque);
}

void ArbolBMas::guardarBloque(Bloque* unBloque, unsigned int nroBloque){
	this->archivoNodos->grabarBloque(unBloque, nroBloque);
}

Bloque* ArbolBMas::obtenerBloque(unsigned int nroBloque){
	return this->archivoNodos->recuperarBloque(nroBloque);
}

ArbolBMas::~ArbolBMas() {
	delete this->archivoNodos;
	if (ultimaHojaVisitada != raiz)
		delete ultimaHojaVisitada;
	delete raiz;
}

