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
	void escribirNorma(double normaInfinito);
	void actualizarNorma(double normaInfinito,unsigned int nrodoc);
	double obtenerNorma(unsigned int id);
	void estadoActualizacion(char c);
	virtual ~archivoNormasInf();
	bool estaActualizado();

private:
	double normaInf;
	void escribir(double normaInfinito,int nrodoc);
};

#endif /* ARCHIVONORMASINF_H_ */
