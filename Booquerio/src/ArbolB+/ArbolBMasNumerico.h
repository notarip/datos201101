/*
 * ArbolBMasNumerico.h
 *
 *  Created on: 15/04/2011
 *      Author: martin
 */

#ifndef ARBOLBMASNUMERICO_H_
#define ARBOLBMASNUMERICO_H_

#include "ArbolBMas.h"
#include <sstream>


class ArbolBMasNumerico : public ArbolBMas {
public:
	ArbolBMasNumerico(string path, unsigned int tamanioBloque);
	virtual ~ArbolBMasNumerico();
	virtual char compareRegistros(string clave, Registro* unRegistro );
	virtual Registro* crearRegistroClave(string clave);
	virtual Registro* crearRegistroClave(string clave, unsigned int valor);
	virtual string consultarClave(Registro* unRegistro);
	virtual void setearClave(Registro* registroAModificar, string clave);

	resultadoOperacion insertarNumerico(unsigned int clave, unsigned int valor);

	Registro* buscarRegistroNumerico(unsigned int clave, resultadoOperacion* resultOperacion);

	resultadoOperacion eliminarNumerico(unsigned int clave, unsigned int valor);
};

#endif /* ARBOLBMASNUMERICO_H_ */
