/*
 * procesadorConsultas.h
 *
 *  Created on: 09/06/2011
 *      Author: rjc
 */

#ifndef PROCESADORCONSULTAS_H_
#define PROCESADORCONSULTAS_H_

#include "../hash/Hash.h"
#include "../ArbolB+/ArbolBMasAlfabetico.h"
#include "../ArbolB+/ArbolBMasNumerico.h"
#include "../servicios/ListasIds.h"
#include <cmath>

#define TAMANIO_BLOQUE_BMAS_NUMERICO 1024

using namespace std;

class ProcesadorConsultas {
public:
	ProcesadorConsultas();

	~ProcesadorConsultas();

private:
	string carpetaRaiz;
	void consultaUnitaria(string termino);
	float calcularPeso(string termino, unsigned int documento, float pesoGlobal);
};

#endif /* PROCESADORCONSULTAS_H_ */
