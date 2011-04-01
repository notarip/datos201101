/*
 * ArchivoBloques.h
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#ifndef ARCHIVOBLOQUES_H_
#define ARCHIVOBLOQUES_H_

#include <fstream>
#include "Bloque.h"

class ArchivoBloques {
public:
	ArchivoBloques(string path, unsigned int tamanoBloque); //tamaño en Bytes;
	virtual ~ArchivoBloques();
	void grabarBloque(Bloque* unBloque, unsigned int nroBloque);
	Bloque* recuperarBloque(unsigned int nroBloque);

};



#endif /* ARCHIVOBLOQUES_H_ */
