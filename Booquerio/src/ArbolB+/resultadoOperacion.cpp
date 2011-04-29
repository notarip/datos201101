/*
 * resultadoOperacion.cpp
 *
 *  Created on: 14/04/2011
 *      Author: martin
 */

#include "resultadoOperacion.h"

resultadoOperacion::resultadoOperacion(int codigo) {
	this->codigo = codigo;
}

void resultadoOperacion::setCodigo(int codigo){
	this->codigo= codigo;
}

resultadoOperacion::~resultadoOperacion() {
	// TODO Auto-generated destructor stub
}
int resultadoOperacion::getCodigo() {
	return codigo;
}
string resultadoOperacion::getDescripcion() {
	switch (codigo) {
	case 1:
		return "HUBO MODIFICACION";
		break;
	case 2:
		return "ENCONTRADO";
		break;
	case 3:
		return "NO ENCONTRADO";
		break;
	case 4:
		return "CLAVE REPETIDA";
		break;
	default:
		return "OK";
		break;
	}


}
