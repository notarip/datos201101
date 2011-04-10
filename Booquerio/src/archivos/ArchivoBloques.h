/*
 * ArchivoBloques.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc(prueba)
 */

#ifndef ARCHIVOBLOQUES_H_
#define ARCHIVOBLOQUES_H_

#include <fstream>
#include "Bloque.h"
#include <list>
#include "ExceptionBloque.h"


/*ArchivoBloques
*
*Se encarga de transferir datos organizados en bloques entre el programa y el archivo de bloques
*/

class ArchivoBloques {
public:
	ArchivoBloques(string path, unsigned int tamanio); //tamaño en Bytes;
	virtual ~ArchivoBloques();
	void grabarBloque(Bloque* unBloque, unsigned int nroBloque);
	Bloque* recuperarBloque(unsigned int nroBloque);
	float getOcupacionBloque(Bloque* unBloque);// devuelve el grado de ocupacion de dicho bloque

private:
	string miPath;
	unsigned int tamanioBloque;

	unsigned int getLongBytes(Registro* unRegistro); //devuelve longitud en bytes de un registro
	unsigned int getBytesLibres(Bloque* unBloque);
	unsigned int getBytesOcupados(Bloque* unBloque);
};



#endif /* ARCHIVOBLOQUES_H_ */
