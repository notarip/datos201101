/*
 * ArbolBMas.h
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#ifndef ARBOLBMAS_H_
#define ARBOLBMAS_H_

#include "../archivos/Registro.h"
#include "../archivos/ArchivoBloques.h"
#include "../archivos/Bloque.h"
#include "../archivos/ExceptionBloque.h"
#include "../util/Parametros.h"
#include "resultadoOperacion.h"

using namespace std;

class ArbolBMas {
public:
	ArbolBMas(string path, int tipoClave);
	virtual ~ArbolBMas();
	resultadoOperacion* insertar(string clave, unsigned int valor);
	resultadoOperacion* buscar(string clave, Registro* regEncontrado);
	resultadoOperacion* eliminar(string clave, unsigned int valor);
	resultadoOperacion* siguiente(Registro* regSiguiente);
	void exportar();

private:
	int ultimaHojaVisitada, ultimoValorBuscado;
	Bloque* raiz;
	string pathArchivoBloques;
	float ocupacionNodo;
	int tamanioNodo;
};

#endif /* ARBOLBMAS_H_ */
