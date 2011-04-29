/*
 * ArbolBMasAlfabetico.h
 *
 *  Created on: 15/04/2011
 *      Author: martin
 */

#ifndef ARBOLBMASALFABETICO_H_
#define ARBOLBMASALFABETICO_H_

#include "ArbolBMas.h"


class ArbolBMasAlfabetico : public ArbolBMas{
public:
	ArbolBMasAlfabetico(string path, unsigned int tamanioBloque);
	virtual ~ArbolBMasAlfabetico();
	virtual char compareRegistros(string clave, Registro* unRegistro );
	virtual Registro* crearRegistroClave(string clave);
	virtual Registro* crearRegistroClave(string clave, unsigned int valor);
	virtual string consultarClave(Registro* unRegistro);
	virtual void setearClave(Registro* registroAModificar, string clave);
};

#endif /* ARBOLBMASALFABETICO_H_ */
