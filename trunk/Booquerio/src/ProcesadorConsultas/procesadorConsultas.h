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
	//devuelve la lista de documentos ordenadas por relevancia decreciente
	void consultaUnitaria(string termino);
	float calcularPeso(string termino, unsigned int documento, float pesoGlobal);
	float calcularPesoxProximidad(list<string> terminos, unsigned int id);

	void consultaPorTerminosCercanos(list<string> listaTerminos);
	list<unsigned int> resolverInterseccion(list<unsigned int> vieja,list<unsigned int> nueva);

};

#endif /* PROCESADORCONSULTAS_H_ */
