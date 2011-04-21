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
	ArbolBMasAlfabetico(string path, float ocupacion, unsigned int tamanioBloque);
	virtual ~ArbolBMasAlfabetico();
	virtual char compareRegistros(string clave, Registro* unRegistro );
	virtual Registro* crearRegistroClave(string clave);
	virtual string consultarClave(Registro* unRegistro);
};

#endif /* ARBOLBMASALFABETICO_H_ */
