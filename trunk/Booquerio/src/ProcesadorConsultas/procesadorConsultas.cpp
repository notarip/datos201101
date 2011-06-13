/*
 * procesadorConsultas.cpp
 *
 *  Created on: 09/06/2011
 *      Author: rjc
 */

#include "procesadorConsultas.h"

ProcesadorConsultas::ProcesadorConsultas() {
	string carpetaRaiz = Parametros().getParametro(CARPETA_DATOS);
}

ProcesadorConsultas::~ProcesadorConsultas() {

}

float ProcesadorConsultas::calcularPesoxProximidad(list<string> terminos, unsigned int id){

}


float ProcesadorConsultas::calcularPeso(string termino, unsigned int documento,
		float pesoGlobal) {

	//Calculo la norma infinito del documento
	fstream archNormas;
	string pathArchNormas = carpetaRaiz + "archivoNormas"; //OJO!! FIJARSE CUAL ES EL PATH LUEGO
	archNormas.open(pathArchNormas.c_str(), ios::in);
	archNormas.seekg(documento * sizeof(int));
	unsigned int norma;
	archNormas >> norma;

	//Obtengo la cantidad de apariciones del termino

	ArbolBMasAlfabetico* arbolDeTerminos = new ArbolBMasAlfabetico(
			carpetaRaiz + "arbolDocJ", TAMANIO_BLOQUE); //OJOOOO!! FIJARSE CUAL PUSO PABLO
	resultadoOperacion result(OK);
	Registro* regEncontrado = arbolDeTerminos->buscarRegistro(termino, &result);
	unsigned int aij = regEncontrado->getAtributosEnteros()->size(); //OJOOOO, VER DONDE SE GUARDAN LAS POSICIONES
	delete arbolDeTerminos;

	return (aij * pesoGlobal / norma);
}

void ProcesadorConsultas::consultaUnitaria(string termino) {
	float pesoGlobal;
	string pathHash = carpetaRaiz + "hash_palabras";
	Hash hashTerminos(pathHash);
	Registro* regPalabra = hashTerminos.buscar(termino);
	if (regPalabra == NULL) {
		cout << "La Busqueda no Obtuvo Resultados" << endl;
	} else {
		string pathArbolPrim = carpetaRaiz + "bmas_primario";
		ArbolBMasNumerico* arbolPrimario = new ArbolBMasNumerico(pathArbolPrim,
				TAMANIO_BLOQUE_BMAS_NUMERICO);
		resultadoOperacion result(OK);
		Registro* regActual = arbolPrimario->buscarRegistroNumerico(0, &result);
		unsigned int N = 1;
		while (regActual != NULL) {
			regActual = arbolPrimario->siguiente();
			N++;
		}
		delete arbolPrimario;

		unsigned int offset = regPalabra->getAtributosEnteros()->front();
		list<unsigned int> listaIdsConsulta;
		ListasIds().obtenerListaIds(offset, &listaIdsConsulta);
		unsigned int ni = listaIdsConsulta.size();
		list<unsigned int>::iterator it = listaIdsConsulta.begin();

		list<unsigned int> listaDocs;
		list<float> listaPesos;
		float unPeso;
		list<unsigned int>::iterator itDocs;
		list<float>::iterator itPesos;

		pesoGlobal = log10(N / ni);
		while (it != listaIdsConsulta.end()) {
			unPeso = calcularPeso(termino, *it, pesoGlobal);

			//CHEKEAR SI NO HACE FALTA HACER OTRA MULTIPLICACION EN LA FORMULA

			itDocs = listaDocs.begin();
			itPesos = listaPesos.begin();

			while (unPeso < *itPesos) {
				itPesos++;
				itDocs++;
			}
			listaDocs.insert(itDocs, *it);
			listaPesos.insert(itPesos, unPeso);

			it++;

		}
		//aca tengo 2 listas con los contenidos ordenados por pesos, una de pesos y otras de doc

		/*TERMINAR DE REVISAR COMO IMPRIMIOS LA INFO*/

	}
}

void ProcesadorConsultas::consultaPorTerminosCercanos(
		list<string> listaTerminos) {

	//itero por todos los terminos buscando los documentos que los contengan
	list<string>::iterator itTerminos = listaTerminos.begin();
	list<unsigned int> idsTodos;
	unsigned int unOffset;
	list<unsigned int> unaListaIds;
	string unTermino = *itTerminos;
	bool primeraVez = true;

	while (itTerminos != listaTerminos.end()) {
		string pathHash = carpetaRaiz + "hash_palabras";
		Hash hashTerminos(pathHash);
		Registro* regPalabra = hashTerminos.buscar(unTermino);
		//si un termino no tiene documentos asociados entonces la busqueda es imposible
		if (regPalabra == NULL) {
			cout << "La Busqueda no Obtuvo Resultados con todas esas palabras"<< endl;
			return;
		} else {
			unOffset = regPalabra->getAtributosEnteros()->front();
			ListasIds().obtenerListaIds(unOffset, &unaListaIds);
			if (primeraVez) {
				//si es la primera vez entonces la lista de todos los ids es la que acabo de levantar
				idsTodos = unaListaIds;
			}
			// la nueva lista de todos los ids que cumplen tener todos los terminos es la interseccion
			// del actual(unaListaIds) con el que venia siendo la interseccion previa(idsTodos)
			idsTodos = resolverInterseccion(idsTodos, unaListaIds);
		}

		itTerminos++;
		primeraVez = false;
	}
	//a la salida de este while tengo en idsTodos los terminos documentos que contienen a todos los terminos
	if ( idsTodos.size() == 0) {
		//busqueda no satisfactoria
		cout << "La Busqueda no Obtuvo Resultados con todas esas palabras" << endl;
		return;
	}

	//Ahora debo iterar por estos documentos y asignar un peso a cada uno de ellos
	list<unsigned int>::iterator itDocsARankear = idsTodos.begin();

	unsigned int unPeso;
	list<unsigned int> listaDocsRankeados;
	list<float> listaPesos;
	list<unsigned int>::iterator itDocs;
	list<float>::iterator itPesos;

	while ( itDocsARankear != idsTodos.end()){
		unPeso = this->calcularPesoxProximidad(listaTerminos,*itDocs);

		itDocs = listaDocsRankeados.begin();
		itPesos = listaPesos.begin();

		while (unPeso < *itPesos) {
			itPesos++;
			itDocs++;
		}
		listaDocsRankeados.insert(itDocs, *itDocsARankear);
		listaPesos.insert(itPesos, unPeso);

		itDocsARankear++;
	}

	//aca tengo 2 listas con los contenidos ordenados por pesos, una de pesos y otras de doc

	/*TERMINAR DE REVISAR COMO IMPRIMIOS LA INFO*/


}

list<unsigned int> ProcesadorConsultas::resolverInterseccion(list<unsigned int> vieja, list<unsigned int> nueva) {

	list<unsigned int> resultado;
	vieja.sort();
	nueva.sort();

	list<unsigned int>::iterator itNuevo = nueva.begin();
	list<unsigned int>::iterator itViejo = vieja.begin();
	bool encontrado = false;
	bool imposible = false;

	unsigned int aVerificar;

	while (itViejo != vieja.end()) {
		encontrado = false;
		imposible = false;
		aVerificar = *itViejo;
		cout << "verifico" << aVerificar << endl;

		while (!encontrado && !imposible) {
			if (*itNuevo == aVerificar)
				encontrado = true;
			else {
				if (*itNuevo > aVerificar)
					imposible = true;
				else
					itNuevo++;

			}
		}

		if (encontrado) {
			resultado.push_back(*itViejo);
		}
		itViejo++;
	}

	return resultado;

}

