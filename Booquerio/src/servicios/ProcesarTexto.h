/*
 * ProcesarTexto.h
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#ifndef PROCESARTEXTO_H_
#define PROCESARTEXTO_H_

#include <string>
#include "../parser/Parser.h"
#include "../archivos/Libro.h"
#include "../servicios/SinIndice.h"
using namespace std;

class ProcesarTexto {
public:
	static void procesarTexto(string ruta);
};

#endif /* PROCESARTEXTO_H_ */
