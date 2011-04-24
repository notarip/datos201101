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
	virtual string consultarClave(Registro* unRegistro);
	virtual void setearClave(Registro* registroAModificar, string clave);

	resultadoOperacion* insertarNumerico(unsigned int clave, unsigned int valor);

	resultadoOperacion* buscarBloqueNumerico(unsigned int clave, Bloque* bloqueEncontrado);

	resultadoOperacion* eliminarNumerico(unsigned int clave, unsigned int valor);

	resultadoOperacion* siguienteNumerico(Registro* regSiguiente);
};

#endif /* ARBOLBMASNUMERICO_H_ */
