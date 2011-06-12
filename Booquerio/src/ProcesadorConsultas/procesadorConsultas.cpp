/*
 * procesadorConsultas.cpp
 *
 *  Created on: 09/06/2011
 *      Author: rjc
 */

#include "procesadorConsultas.h"

ProcesadorConsultas::ProcesadorConsultas(){
	string carpetaRaiz = Parametros().getParametro(CARPETA_DATOS);
}

ProcesadorConsultas::~ProcesadorConsultas(){

}

float ProcesadorConsultas::calcularPeso(string termino, unsigned int documento, float pesoGlobal){

	//Calculo la norma infinito del documento
	fstream archNormas;
	string pathArchNormas= carpetaRaiz+"archivoNormas"; //OJO!! FIJARSE CUAL ES EL PATH LUEGO
	archNormas.open(pathArchNormas.c_str(), ios::in);
	archNormas.seekg(documento*sizeof(int));
	unsigned int norma;
	archNormas>>norma;

	//Obtengo la cantidad de apariciones del termino

	ArbolBMasAlfabetico* arbolDeTerminos= new ArbolBMasAlfabetico(carpetaRaiz+"arbolDocJ", TAMANIO_BLOQUE); //OJOOOO!! FIJARSE CUAL PUSO PABLO
	resultadoOperacion result(OK);
	Registro* regEncontrado= arbolDeTerminos->buscarRegistro(termino, &result);
	unsigned int aij= regEncontrado->getAtributosEnteros()->size(); //OJOOOO, VER DONDE SE GUARDAN LAS POSICIONES
	delete arbolDeTerminos;

	return (aij*pesoGlobal/norma);
}

void ProcesadorConsultas::consultaUnitaria(string termino){
	float pesoGlobal;
	string pathHash = carpetaRaiz+"hash_palabras";
	Hash hashTerminos(pathHash);
	Registro* regPalabra= hashTerminos.buscar(termino);
	if(regPalabra==NULL){
		cout<<"La Busqueda no Obtuvo Resultados"<<endl;
	}
	else{
		//Calculo la cant total de documentos

		string pathArbolPrim= carpetaRaiz+"bmas_primario";
		ArbolBMasNumerico* arbolPrimario= new ArbolBMasNumerico(pathArbolPrim, TAMANIO_BLOQUE_BMAS_NUMERICO);
		resultadoOperacion result(OK);
		Registro* regActual= arbolPrimario->buscarRegistroNumerico(0,&result);
		unsigned int N= 1;
		while(regActual!=NULL){
			regActual=arbolPrimario->siguiente();
			N++;
		}
		delete arbolPrimario;

		unsigned int offset= regPalabra->getAtributosEnteros()->front();
		list<unsigned int> listaIdsConsulta;
		ListasIds().obtenerListaIds(offset,&listaIdsConsulta);
		unsigned int ni= listaIdsConsulta.size();
		list<unsigned int>::iterator it= listaIdsConsulta.begin();
		while(it!=listaIdsConsulta.end()){
			pesoGlobal= log10(N/ni);
			calcularPeso(termino, *it, pesoGlobal);
			it++;
		}
	}
}
