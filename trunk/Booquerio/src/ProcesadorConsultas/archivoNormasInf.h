/*
 * archivoNormasInf.h
 *
 *  Created on: 15/06/2011
 *      Author: rjc
 */

#ifndef ARCHIVONORMASINF_H_
#define ARCHIVONORMASINF_H_
#include <fstream>
#include "../util/Util.h"
#define RUTA_ARCHIVO_NORMAS "normas_infinito.dat"
using namespace std;

class archivoNormasInf {
public:
	archivoNormasInf();
	void escribirNorma(unsigned int normaInfinito);
	unsigned int obtenerNorma(unsigned int id);
	virtual ~archivoNormasInf();

private:
	unsigned int normaInf;
};

#endif /* ARCHIVONORMASINF_H_ */
