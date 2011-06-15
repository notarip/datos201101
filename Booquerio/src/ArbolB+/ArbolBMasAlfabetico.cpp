/*
 * ArbolBMasAlfabetico.cpp
 *
 *  Created on: 15/04/2011
 *      Author: martin
 */

#include "ArbolBMasAlfabetico.h"

ArbolBMasAlfabetico::ArbolBMasAlfabetico(string path,
		unsigned int tamanioBloque) :
	ArbolBMas(path, tamanioBloque) {
	this->archivoNodos = new ArchivoBloques(path, tamanioBloque);

		this->tamanioNodo = tamanioBloque;

		if (Util().existeArchivo(path)){
			raiz = obtenerBloque(0);
		}
		else{
			raiz = new Bloque();
			guardarBloque(raiz, 0);
		}
		ultimaHojaVisitada = raiz;

}

char ArbolBMasAlfabetico::compareRegistros(string clave, Registro* unRegistro){
	string claveRegistro = unRegistro->getString();

	if (clave < claveRegistro) return -1;
	if (clave == claveRegistro) return 0;
	if (clave > claveRegistro) return 1;
}

Registro* ArbolBMasAlfabetico::crearRegistroClave(string clave, unsigned int valor){
	Registro* unRegistro= new Registro();
	unRegistro->setString(clave);
	unRegistro->agregarReferencia(valor);
	return unRegistro;
}

Registro* ArbolBMasAlfabetico::crearRegistroClave(string clave){
	return new Registro(clave);
}

void ArbolBMasAlfabetico::setearClave(Registro* registroAModificar, string clave) {
	registroAModificar->setString(clave);
}

string ArbolBMasAlfabetico::consultarClave(Registro* unRegistro){
	return unRegistro->getString();
}


//DESCOMENTAR EN EL CASO DE QUERER ACTIVAR EL FRONTCODING


Bloque* ArbolBMasAlfabetico::aplicarFrontCoding (Bloque* unBloque){
	Bloque* bloqueFontCoding= new Bloque();
	bloqueFontCoding->setAtributoBloque(unBloque->getAtributoBloque());
	bloqueFontCoding->setSiguiente(unBloque->getSiguiente());
	list<Registro>* listaReg= unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros= listaReg->begin();
	if(itRegistros!=listaReg->end()){
		Registro unRegistro(*itRegistros);
	//unRegistro.agregarAtribEntero(0);
		itRegistros++;
		bloqueFontCoding->agregarRegistro(unRegistro);
		unsigned int coincidencias;
		string previo= unRegistro.getString();
		char* stringFront;
		string actual= "";
		bool coincidio= true;
		unsigned int caracteresNoCoincidentes= 0;
		while(itRegistros!=listaReg->end()){
			coincidio= true;
			Registro unRegistro(*itRegistros);
			stringFront= NULL;
			actual=unRegistro.getString();
			coincidencias= 0;
			while(coincidencias<actual.size()&&coincidencias<previo.size()&&coincidio){
				if(actual.at(coincidencias)==previo.at(coincidencias)){
					coincidencias++;
				}
				else{
					coincidio=false;
				}
			}
		caracteresNoCoincidentes= actual.size()-coincidencias;
		stringFront=new char[caracteresNoCoincidentes+1];
		stringFront[caracteresNoCoincidentes]='\0';
		//copy(dest,cuanto,desde donde)
		actual.copy(stringFront,caracteresNoCoincidentes,coincidencias);
		previo=actual;
		unRegistro.setString(stringFront);
		if (coincidencias>0)
			unRegistro.agregarAtribEntero(coincidencias);
		bloqueFontCoding->agregarRegistro(unRegistro);
		itRegistros++;
		delete []stringFront;
	}
	}
	return bloqueFontCoding;
}

Bloque* ArbolBMasAlfabetico::deshacerFrontCoding(Bloque* unBloque) {
	Bloque* bloqueDescomprimido= new Bloque();
	bloqueDescomprimido->setAtributoBloque(unBloque->getAtributoBloque());
	bloqueDescomprimido->setSiguiente(unBloque->getSiguiente());
	list<Registro>* listaReg= unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros= listaReg->begin();
	Registro unRegistro(*itRegistros);
	string previo= itRegistros->getString();
	*itRegistros++;
	bloqueDescomprimido->agregarRegistro(unRegistro);
	unsigned int coincidencias;
	string actual="";
	while(itRegistros!=listaReg->end()){
		Registro unRegistro(*itRegistros);
		actual="";
		if(unRegistro.getAtributosEnteros()->size()>0){
			coincidencias= unRegistro.getAtributosEnteros()->front();
			unRegistro.getAtributosEnteros()->pop_front();
		}
		else
			coincidencias=0;
		actual.append(previo,0,coincidencias);
		previo=unRegistro.getString();
		actual.append(previo);
		unRegistro.setString(actual);
		bloqueDescomprimido->agregarRegistro(unRegistro);
		previo= actual;
		itRegistros++;
	}
	return bloqueDescomprimido;
}

float ArbolBMasAlfabetico::obtenerOcupacionBloque(Bloque* unBloque){
	Bloque* bloqueAChequear= aplicarFrontCoding(unBloque);
	float resultado= this->archivoNodos->getOcupacionBloque(bloqueAChequear);
	delete bloqueAChequear;
	return resultado;
}

void ArbolBMasAlfabetico::guardarBloque(Bloque* unBloque, unsigned int nroBloque){
	Bloque* bloqueAGrabar= aplicarFrontCoding(unBloque);
	try{
		this->archivoNodos->grabarBloque(bloqueAGrabar, nroBloque);
	}
	catch (ExceptionBloque& e){
		delete bloqueAGrabar;
		throw e;
	};
	delete bloqueAGrabar;
}

Bloque* ArbolBMasAlfabetico::obtenerBloque(unsigned int nroBloque){
	Bloque* bloqueLevantado= this->archivoNodos->recuperarBloque(nroBloque);
	Bloque* bloqueARetornar= deshacerFrontCoding(bloqueLevantado);
	delete bloqueLevantado;
	return bloqueARetornar;
}

ArbolBMasAlfabetico::~ArbolBMasAlfabetico() {

}
