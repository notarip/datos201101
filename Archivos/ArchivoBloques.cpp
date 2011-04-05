/*
 * ArchivoBloques.cpp
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#include "ArchivoBloques.h"

ArchivoBloques::ArchivoBloques(string path, unsigned int tamanio) {
	miPath = path;
	tamanioBloque = tamanio;
}

ArchivoBloques::~ArchivoBloques() {
	// TODO Auto-generated destructor stub
}

void ArchivoBloques::grabarBloque(Bloque* unBloque, unsigned int nroBloque){

	/*faltaria verificacion si entra en donde la quiero poner(usar getOcupacionBloque)*/

	//comienzo armado de la tira de bytes

	char tiraBytes[this->tamanioBloque];
	int i = 0; //representa la posicion en la tira de bytes

	//inserto tamaño libre del bloque;
	tiraBytes[0] = getBytesLibres(unBloque);
	i= i+4;
	//file.write((char*)getBytesLibres(unBloque),4);

	list<Registro>* registrosDelBloque = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = registrosDelBloque->begin();

	//inserto todos los registros del bloque
	while (itRegistros != registrosDelBloque->end() ){
		Registro* unRegistro = &(*itRegistros);
		//escribo la long del registro

		tiraBytes[i] = getLongBytes(unRegistro);
		i = i+4;
		//file.write((char*)this->getLongBytes(unRegistro),4);

		//escribo el registro estrictamente
		//primero el string
		unsigned int tamanioString = unRegistro->getString().size();

		tiraBytes[i] = tamanioString;
		i=i+4;
		tiraBytes[12] = *unRegistro->getString().c_str();
		i=i+tamanioString;
		//file.write((char*)tamanioString,4);
		//file.write((char*)unRegistro->getString().c_str(),tamanioString);

		//despues los identificadores
		list<unsigned int>* Ids = unRegistro->getIdentificadores();
		unsigned int tamanioIds = Ids->size() * 4;

		tiraBytes[i] = tamanioIds;
		i = i+4;
		//file.write((char*)tamanioIds,4);

		list<unsigned int>::iterator itIds = Ids->begin();
		while(itIds != Ids->end()){

			tiraBytes[i] = *itIds;
			i = i+4;
			//file.write((char*)*itIds,4);

			itIds++;
		}

		//por ultimo las referencias
		list<unsigned short int>* referencias = unRegistro->getReferencias();
		unsigned int tamanioReferencias = referencias->size() * 2;

		tiraBytes[i] = tamanioReferencias;
		i = i+4;
		//file.write((char*)tamanioReferencias,4);

		list<unsigned short int>::iterator itReferencias = referencias->begin();
		while(itReferencias != referencias->end()){

			tiraBytes[i] = *itReferencias;
			i= i+2;
			//file.write((char*)*itReferencias,2);
			itReferencias++;
		}

		itRegistros++;
	}



	fstream file(miPath.c_str(),ios::binary);

	//posiciono el puntero del archivo al principio del bloque que necesito (primer bloque = bloque0)
	file.seekp(nroBloque*tamanioBloque);

	//plancho la tira de bytes:
	file.write(tiraBytes,tamanioBloque);

	file.close();

}

Bloque* ArchivoBloques::recuperarBloque(unsigned int nroBloque){
	fstream file(miPath.c_str(),ios::binary);

	/******************************IMPLEMENTAR*************************************/


	file.close();


}

float ArchivoBloques::getOcupacionBloque(Bloque* unBloque){
	unsigned int bytesOcupados = this->getBytesOcupados(unBloque);

	return bytesOcupados / this->tamanioBloque;

}

unsigned int ArchivoBloques::getBytesOcupados(Bloque* unBloque){

	unsigned int tamanioRegistros= 20;
	// 20 bytes corespondientes a caracteres de control
	// 4  para espacio libre en bloque
	// 16 corresponden a los prefijos de long(int) (registro,string,identificadores,referencias)

	list<Registro>* registrosDelBloque = unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros = registrosDelBloque->begin();


	while (itRegistros != registrosDelBloque->end() ){
		Registro* unRegistro = &(*itRegistros);
		tamanioRegistros += this->getLongBytes( unRegistro);
		itRegistros++;
	}

	return tamanioRegistros;

}


unsigned int ArchivoBloques::getLongBytes(Registro* unRegistro){
	return (((unRegistro->getIdentificadores()->size())*4)
					+ ((unRegistro->getReferencias()->size())*2)
					+ unRegistro->getString().size() );
}

unsigned int ArchivoBloques::getBytesLibres(Bloque* unBloque){
	return tamanioBloque - this->getBytesOcupados(unBloque);

}

