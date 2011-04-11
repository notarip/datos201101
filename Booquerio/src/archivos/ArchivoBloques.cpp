/*
 * ArchivoBloques.cpp
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#include "ArchivoBloques.h"
#include "string.h"
#include <iostream>


using namespace std;

ArchivoBloques::ArchivoBloques(string path, unsigned int tamanio) {
	miPath = path;
	tamanioBloque = tamanio;
}

ArchivoBloques::~ArchivoBloques() {
}

void ArchivoBloques::grabarBloque(Bloque* unBloque, unsigned int nroBloque) {

	// Evitor grabar bloque + grandes del tamanio
	if (this-> getBytesOcupados(unBloque) > this->tamanioBloque)
		throw ExceptionBloque();

	//comienzo armado de la tira de bytes

	char tiraBytes[this->tamanioBloque];
	int i = 0; //representa la posicion en la tira de bytes

	//inserto tamaño ocupado del bloque;
	unsigned int bytesOcupados = this->getBytesOcupados(unBloque);

	memcpy(tiraBytes + i, &bytesOcupados, 4);
	i = i + 4;

	list<Registro>* registrosDelBloque = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = registrosDelBloque->begin();

	//inserto todos los registros del bloque
	while (itRegistros != registrosDelBloque->end()) {
		Registro* unRegistro = &(*itRegistros);
		//escribo la long del registro

		unsigned int longReg = getLongBytes(unRegistro);
		memcpy(tiraBytes + i, &longReg, 4);
		i = i + 4;

		//escribo el registro estrictamente
		//primero el string
		unsigned int tamanioString = unRegistro->getString().size();
		memcpy(tiraBytes + i, &tamanioString, 4);
		i = i + 4;
		string unString = (*unRegistro).getString();
		char cstr[unString.size()];
		strcpy(cstr, unString.c_str());
		memcpy(tiraBytes + i, cstr, strlen(cstr));
		i = i + tamanioString;

		//despues los identificadores
		list<unsigned int>* Ids = unRegistro->getIdentificadores();
		unsigned int tamanioIds = Ids->size() * 4;

		memcpy(tiraBytes + i, &tamanioIds, 4);
		i = i + 4;

		list<unsigned int>::iterator itIds = Ids->begin();
		while (itIds != Ids->end()) {

			unsigned int id = *itIds;
			memcpy(tiraBytes + i, &id, 4);
			i = i + 4;
			itIds++;
		}

		//por ultimo las referencias
		list<unsigned short int>* referencias = unRegistro->getReferencias();
		unsigned int tamanioReferencias = referencias->size() * 2;

		memcpy(tiraBytes + i, &tamanioReferencias, 4);
		i = i + 4;

		list<unsigned short int>::iterator itReferencias = referencias->begin();
		while (itReferencias != referencias->end()) {

			unsigned int ref = *itReferencias;
			memcpy(tiraBytes + i, &ref, 2);
			i = i + 2;
			itReferencias++;
		}

		itRegistros++;
	}

	fstream archivo;
	archivo.open(miPath.c_str(),ios::binary| ios::out | ios::in | ios::ate);
	if( !archivo.good()) {archivo.open(miPath.c_str(),ios::binary| ios::out | ios::ate);}

	//posiciono el puntero del archivo al principio del bloque que necesito (primer bloque = bloque0)

	archivo.seekp(nroBloque * tamanioBloque, ios::beg);

	//plancho la tira de bytes:

	archivo.write(tiraBytes, tamanioBloque);
	/*OJO QUE SE PLANCHA CON BASURA ASI COMO ESTA AHORA   => IGUAL NO AFECTA
	 **/

	archivo.close();

}

Bloque* ArchivoBloques::recuperarBloque(unsigned int nroBloque) {
	fstream file(miPath.c_str(), ios::binary | ios::in);
	char tiraBytes[this->tamanioBloque];
	int i = 0; //representa la posicion en la tira de bytes

	file.seekg(tamanioBloque * nroBloque, ios::beg);

	//bajo la tira de bytes a memoria
	file.read(tiraBytes, tamanioBloque);
	file.close();


	// comienzo interpretacion de los bytes
	unsigned int bytesOcupados;

	//
	memcpy(&bytesOcupados, tiraBytes + i, 4);
	i = i + 4;
	Bloque* unBloque = new Bloque();

	while ((bytesOcupados - i) > 0) {
		i = i + 4; //salteo long reg

		//levanto el string
		unsigned int tamanioString;
		memcpy(&tamanioString, tiraBytes + i, 4);
		i = i + 4;
		char cadena[tamanioString];
		memcpy(cadena, tiraBytes + i, tamanioString);
		cadena[tamanioString] = '\0';
		i = i + tamanioString;
		string unString = string(cadena);
		Registro unRegistro(unString);

		//levanto identificadores
		unsigned int tamanioIds;
		memcpy(&tamanioIds, tiraBytes + i, 4);
		i = i + 4;
		for (unsigned int j = 0; j < tamanioIds / 4; j++) {
			unsigned int unId;
			memcpy(&unId, tiraBytes + i, 4);
			i = i + 4;
			unRegistro.agregarId(unId);
		}

		//levanto referencias
		unsigned int tamanioRef;
		memcpy(&tamanioRef, tiraBytes + i, 4);
		i = i + 4;
		for (unsigned int j = 0; j < tamanioRef / 2; j++) {
			unsigned short int unaRef;
			memcpy(&unaRef, tiraBytes + i, 2);
			i = i + 2;
			unRegistro.agregarReferencia(unaRef);
		}
		unBloque->agregarRegistro(unRegistro);
	}

	return unBloque;
}

float ArchivoBloques::getOcupacionBloque(Bloque* unBloque) {
	unsigned int bytesOcupados = this->getBytesOcupados(unBloque);

	return (float) bytesOcupados / this->tamanioBloque;

}

unsigned int ArchivoBloques::getBytesOcupados(Bloque* unBloque) {

	unsigned int tamanioRegistros = 4;
	// 4  bytes para espacio libre en bloque

	list<Registro>* registrosDelBloque = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = registrosDelBloque->begin();

	while (itRegistros != registrosDelBloque->end()) {
		Registro* unRegistro = &(*itRegistros);
		//16 bytes corresponden a los prefijos de long(int) (registro,string,identificadores,referencias)
		tamanioRegistros += 16;
		tamanioRegistros += this->getLongBytes(unRegistro);
		itRegistros++;
	}

	return tamanioRegistros;
}

unsigned int ArchivoBloques::getLongBytes(Registro* unRegistro) {

	return (((unRegistro->getIdentificadores()->size()) * 4)
			+ ((unRegistro->getReferencias()->size()) * 2)
			+ unRegistro->getString().size());
}

unsigned int ArchivoBloques::getBytesLibres(Bloque* unBloque) {
	return tamanioBloque - this->getBytesOcupados(unBloque);
}

