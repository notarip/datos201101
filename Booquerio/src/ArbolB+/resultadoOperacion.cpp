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

resultadoOperacion::~resultadoOperacion() {
	// TODO Auto-generated destructor stub
}
int resultadoOperacion::getCodigo() {
	return codigo;
}
string resultadoOperacion::getDescripcion() {
	switch (codigo) {
	case 1:
		return "modificacion";
		break;
	case 2:
		return "encontrado";
		break;
	default:
		return "ok";
		break;
	}


}
