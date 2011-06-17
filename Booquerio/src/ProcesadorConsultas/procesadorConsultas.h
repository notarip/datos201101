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
#include "../ProcesadorConsultas/archivoNormasInf.h"
#include "../servicios/ListasIds.h"
#include <cmath>
#include "limits.h"
#include <map>

#define TAMANIO_BLOQUE_BMAS_NUMERICO 1024
#define PREFIJO_ARBOL_TERMINOS "bmas_palabras_"

using namespace std;

class ProcesadorConsultas {
public:
	ProcesadorConsultas();
	void procesar(list<string> terminos);
	~ProcesadorConsultas();

private:
	string carpetaRaiz;
	//devuelve la lista de documentos ordenadas por relevancia decreciente
	void consultaUnitaria(string termino);
	float calcularPeso(string termino, unsigned int documento, float pesoGlobal);
	float calcularPesoxProximidad(list<string> terminos, unsigned int id);
	void ArmarSecuenciaOptima(list<unsigned int> secuencia, list<unsigned int>* max , unsigned int cantListas, list<unsigned int>[]);
	unsigned int obtenerSeparacion(list<unsigned int> unaSecuencia);
	void consultaPorTerminosCercanos2(list<string> listaTerminos);
	list<float> calculadorPesos(list<unsigned int> documentos,list<string> terminos, unsigned int agrupacion, unsigned int comienzo);
	void rankearDocumentos(list<unsigned int> documentos, list<float> pesos);
	list<float> pesarSegunCantPalabras(list<float> pesos, unsigned int agrupacion);



	void consultaPorTerminosCercanos(list<string> listaTerminos);
	list<unsigned int> resolverInterseccion(list<unsigned int> vieja,list<unsigned int> nueva);
public:
	list<unsigned int> resolverResta(list<unsigned int> original, list<unsigned int> aRestar);

	void imprimirConsulta(list<unsigned int> docRankeados);
	void imprimirConsulta(map<unsigned int,float> unMapa);
	void imprimirConsulta(list<unsigned int> docsOrdenados,list<float> pesosOrdenados);


};

#endif /* PROCESADORCONSULTAS_H_ */
