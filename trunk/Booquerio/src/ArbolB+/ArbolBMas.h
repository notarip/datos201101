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
	resultadoOperacion* insertar(Registro registroAInsertar);
	resultadoOperacion* buscar(Registro registroABuscar);
	resultadoOperacion* eliminar(Registro registroAEliminar);
	resultadoOperacion* siguiente(Registro registroAInsertar);
	void exportar();

private:
	int ultimaHojaVisitada, raiz;
	string pathArchivoBloques;
	float ocupacionNodo;
	int tamanioNodo;
};

#endif /* ARBOLBMAS_H_ */
