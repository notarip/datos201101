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

	// Evito grabar bloque + grandes del tamanio
	if (this-> getBytesOcupados(unBloque) > this->tamanioBloque)
		throw ExceptionBloque();

	//comienzo armado de la tira de bytes

	char tiraBytes[this->tamanioBloque];
	int i = 0; //representa la posicion en la tira de bytes

	//inserto tamaño ocupado del bloque;
	unsigned int bytesOcupados = this->getBytesOcupados(unBloque);

	memcpy(tiraBytes + i, &bytesOcupados, sizeof(int));
	i = i + sizeof(int);

	//inserto el atributo del bloque
	unsigned int atributoBloque= unBloque->getAtributoBloque();

	memcpy(tiraBytes + i, &atributoBloque, sizeof(int));
	i = i + sizeof(int);

	//inserto puntero a bloque siguiente
	unsigned int siguiente= unBloque->getSiguiente();

	memcpy(tiraBytes + i, &siguiente, sizeof(int));
	i = i + sizeof(int);


	list<Registro>* registrosDelBloque = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = registrosDelBloque->begin();

	//inserto todos los registros del bloque
	while (itRegistros != registrosDelBloque->end()) {
		Registro* unRegistro = &(*itRegistros);
		//escribo la long del registro

		unsigned int longReg = getLongBytes(unRegistro);
		memcpy(tiraBytes + i, &longReg, sizeof(int));
		i = i + sizeof(int);

		//escribo el registro estrictamente
		//primero el string
		unsigned int tamanioString = unRegistro->getString().size();
		memcpy(tiraBytes + i, &tamanioString, sizeof(int));
		i = i + sizeof(int);
		string unString = (*unRegistro).getString();
		char cstr[unString.size()];
		strcpy(cstr, unString.c_str());
		memcpy(tiraBytes + i, cstr, strlen(cstr));
		i = i + tamanioString;

		//despues los identificadores
		list<unsigned int>* Ids = unRegistro->getIdentificadores();
		unsigned int tamanioIds = Ids->size() * sizeof(int);

		memcpy(tiraBytes + i, &tamanioIds, sizeof(int));
		i = i + sizeof(int);

		list<unsigned int>::iterator itIds = Ids->begin();
		while (itIds != Ids->end()) {

			unsigned int id = *itIds;
			memcpy(tiraBytes + i, &id, sizeof(int));
			i = i + sizeof(int);
			itIds++;
		}

		//por ultimo las referencias
		list<unsigned int>* referencias = unRegistro->getReferencias();
		unsigned int tamanioReferencias = referencias->size() * sizeof(int);

		memcpy(tiraBytes + i, &tamanioReferencias, sizeof(int));
		i = i + sizeof(int);

		list<unsigned int>::iterator itReferencias = referencias->begin();
		while (itReferencias != referencias->end()) {

			unsigned int ref = *itReferencias;
			memcpy(tiraBytes + i, &ref, sizeof(int));
			i = i + sizeof(int);
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
	memcpy(&bytesOcupados, tiraBytes + i, sizeof(int));
	i = i + sizeof(int);
	Bloque* unBloque = new Bloque();

	//levanto el atributo del bloque
	unsigned int atributoBloque;
	memcpy(&atributoBloque, tiraBytes + i, sizeof(int));
	i = i + sizeof(int);
	unBloque->setAtributoBloque(atributoBloque);

	//levanto el puntero al siguiente bloque

	unsigned int siguiente;
	memcpy(&siguiente, tiraBytes + i, sizeof(int));
	i = i + sizeof(int);
	unBloque->setSiguiente(siguiente);

	while ((bytesOcupados - i) > 0) {
		i = i + sizeof(int); //salteo long reg

		//levanto el string
		unsigned int tamanioString;
		memcpy(&tamanioString, tiraBytes + i, sizeof(int));
		i = i + sizeof(int);
		char cadena[tamanioString];
		memcpy(cadena, tiraBytes + i, tamanioString);
		cadena[tamanioString] = '\0';
		i = i + tamanioString;
		string unString = string(cadena);
		Registro unRegistro(unString);

		//levanto identificadores
		unsigned int tamanioIds;
		memcpy(&tamanioIds, tiraBytes + i, sizeof(int));
		i = i + sizeof(int);
		for (unsigned int j = 0; j < tamanioIds / sizeof(int); j++) {
			unsigned int unId;
			memcpy(&unId, tiraBytes + i, sizeof(int));
			i = i + sizeof(int);
			unRegistro.agregarId(unId);
		}

		//levanto referencias
		unsigned int tamanioRef;
		memcpy(&tamanioRef, tiraBytes + i, sizeof(int));
		i = i + sizeof(int);
		for (unsigned int j = 0; j < tamanioRef / sizeof(int); j++) {
			unsigned int unaRef;
			memcpy(&unaRef, tiraBytes + i, sizeof(int));
			i = i + sizeof(int);
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

	unsigned int tamanioRegistros = 3*sizeof(int);
	// 4  bytes para espacio ocupado en bloque
	// 4  bytes para atributo del bloque
	// 4  bytes para puntero al siguiente bloque

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

	return (((unRegistro->getIdentificadores()->size()) * sizeof(int))
			+ ((unRegistro->getReferencias()->size()) * sizeof(int))
			+ unRegistro->getString().size());
}

unsigned int ArchivoBloques::getBytesLibres(Bloque* unBloque) {
	return tamanioBloque - this->getBytesOcupados(unBloque);
}

void ArchivoBloques::eliminarBloque(unsigned int nroBloque){
	string pathLibres = miPath;
	pathLibres += "-libres";
	fstream archivoLibres;

	cout << pathLibres << endl;

	archivoLibres.open(pathLibres.c_str(),ios::binary | ios::in | ios::out | ios::ate);

	cout << boolalpha << archivoLibres.good() << endl;

	if (!archivoLibres.good()){
		archivoLibres.open(pathLibres.c_str(), ios::binary | ios::out | ios::ate );}

	archivoLibres.write((char*)&nroBloque , sizeof(int));
}

unsigned int ArchivoBloques::getBloqueLibre(){
	string pathLibres = miPath;
	pathLibres += "-libres";
	fstream archivoLibres, archivoBloques;
	unsigned int unBloqueLibre;

	archivoLibres.open(pathLibres.c_str(),ios::binary | ios::in | ios::out );

	if (!archivoLibres.eof() && archivoLibres.good() ){
		//leo primer numero
		archivoLibres.read((char*)&unBloqueLibre,sizeof(int));

		unsigned int posInicial, posFinal, longitud;
		posInicial = archivoLibres.tellg();
		archivoLibres.seekg(0, ios::end);
		posFinal = archivoLibres.tellg();
		longitud = posFinal - posInicial;

		archivoLibres.seekg(posInicial,ios::beg);

		char libresRestantes[longitud];
		archivoLibres.read(libresRestantes, longitud);
		archivoLibres.close();

		archivoLibres.open(pathLibres.c_str(), ios::binary | ios::out);
		archivoLibres.write(libresRestantes,longitud);
	}

	else { //el archivo no existe o esta vacio
		archivoBloques.open(miPath.c_str(), ios::binary | ios::in | ios::ate);

		if (archivoBloques.good()){
			unBloqueLibre = archivoBloques.tellp() / this->tamanioBloque;
		}

		archivoBloques.close();
	}

	archivoLibres.close();

	return unBloqueLibre;

}
