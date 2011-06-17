/*
 * archivoNormasInf.cpp
 *
 *  Created on: 15/06/2011
 *      Author: rjc
 */

#include "archivoNormasInf.h"
#include "procesadorConsultas.h"

archivoNormasInf::archivoNormasInf() {
	this->normaInf=0;
}

void archivoNormasInf::escribirNorma(double normaInfinito){
	this->escribir(normaInfinito, 0);

}

void archivoNormasInf::actualizarNorma(double normainfinitoArch,unsigned int nroDoc){
	this->escribir(normainfinitoArch, nroDoc);

}

void archivoNormasInf::escribir(double normaInfinitoArch,unsigned int nro){

	this->normaInf= normaInfinitoArch;
	fstream normasInfinito;

	string ruta= Parametros().getParametro(CARPETA_DATOS);
	ruta+= RUTA_ARCHIVO_NORMAS;

	if (nro==0) normasInfinito.open(ruta.c_str(),ios::out | ios::app | ios::in);

	else{
		normasInfinito.open(ruta.c_str(),ios::out | ios::in);
		normasInfinito.seekp(sizeof(unsigned int)*nro);
	}

	char* valor=new char[4];

	memcpy(valor,&this->normaInf,sizeof(unsigned int));

	normasInfinito.write(valor,sizeof(unsigned int));
	normasInfinito.close();

	delete []valor;
}

unsigned int archivoNormasInf::obtenerNorma(unsigned int id){

	fstream archNormas;
	string pathArchNormas = Parametros().getParametro(CARPETA_DATOS);
	pathArchNormas=+ RUTA_ARCHIVO_NORMAS; //OJO!! FIJARSE CUAL ES EL PATH LUEGO
	archNormas.open(pathArchNormas.c_str(), ios::in);
	archNormas.seekg(id * sizeof(int));
	unsigned int norma;
	archNormas >> norma;
	archNormas.close();
	return norma;
}

archivoNormasInf::~archivoNormasInf() {
	// TODO Auto-generated destructor stub
}
