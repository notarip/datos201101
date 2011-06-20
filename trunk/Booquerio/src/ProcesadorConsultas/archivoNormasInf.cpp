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

	fstream archNormas;
	string ruta= Parametros().getParametro(CARPETA_DATOS);
	ruta+= RUTA_ARCHIVO_NORMAS;
	archNormas.open(ruta.c_str(),ios::out | ios::in);

	if (!archNormas.good()){
		archNormas.open(ruta.c_str(), ios::out);
		archNormas.seekp(0,ios::beg);
		char c='D';
		archNormas.write(&c,sizeof(char));
	}


}

void archivoNormasInf::escribirNorma(double normaInfinito){
	this->escribir(normaInfinito, -1);

}

void archivoNormasInf::actualizarNorma(double normainfinitoArch,unsigned int nroDoc){
	this->escribir(normainfinitoArch, nroDoc);

}

void archivoNormasInf::escribir(double normaInfinitoArch, int nro){

	this->normaInf= normaInfinitoArch;
	fstream normasInfinito;

	string ruta= Parametros().getParametro(CARPETA_DATOS);
	ruta+= RUTA_ARCHIVO_NORMAS;

	if (nro==-1) normasInfinito.open(ruta.c_str(),ios::out | ios::app | ios::in);

	else{
		normasInfinito.open(ruta.c_str(),ios::out | ios::in);
		normasInfinito.seekp(sizeof(double)*(nro)+1);
	}

	char* valor=new char[sizeof(double)];

	memcpy(valor,&this->normaInf,sizeof(double));

	normasInfinito.write(valor,sizeof(double));
	normasInfinito.close();

	this->estadoActualizacion('D');

	delete []valor;
}

double archivoNormasInf::obtenerNorma(unsigned int id){

	fstream archNormas;
	string pathArchNormas = Parametros().getParametro(CARPETA_DATOS);
	pathArchNormas+= RUTA_ARCHIVO_NORMAS; //OJO!! FIJARSE CUAL ES EL PATH LUEGO
	archNormas.open(pathArchNormas.c_str(), ios::in);
	archNormas.seekg(id * sizeof(double)+sizeof(char));
	double norma;
	archNormas.read((char*)&norma, sizeof(double));
	archNormas.close();
	return norma;
}

void archivoNormasInf::imprimirNormasInf(string path, list<unsigned int> idLibros){
	fstream archivo (path.c_str(), ios::in | ios::out | ios::app);
	list<unsigned int>::iterator it= idLibros.begin();
	archivo<<endl;
	archivo<<"ID Libro"<<"		";
	while(it!=idLibros.end()){
		archivo<<*it<<"		";
		it++;
	}
	archivo<<endl;
	archivo<<"Norma Infinito"<<"		";
	it= idLibros.begin();
	while(it!=idLibros.end()){
		archivo<<this->obtenerNorma(*it)<<"		";
		it++;
	}
	archivo<<endl<<endl;
	for(int i= 0; i< idLibros.size(); i++){
		archivo<<"*********************";
	}
	archivo<<endl;
}

void archivoNormasInf::estadoActualizacion(char c){
	fstream archNormas;
	string pathArchNormas = Parametros().getParametro(CARPETA_DATOS);
	pathArchNormas+= RUTA_ARCHIVO_NORMAS;

	archNormas.open(pathArchNormas.c_str(),ios::out | ios::in);
	if (archNormas.good()){

		char leido;

		archNormas.seekg(0,ios::beg);
		archNormas.read(&leido,sizeof(char));

		if ( ((leido == 'A') && (c == 'D')) || ((leido == 'D') && (c == 'A')) ){
			archNormas.seekp(0,ios::beg);
			archNormas.write(&c, sizeof(char));
			if (c== 'A') cout <<"Normas Infinito actualizadas."<<endl;
		}

	}

}

bool archivoNormasInf::estaActualizado(){
	fstream archNormas;
	string pathArchNormas = Parametros().getParametro(CARPETA_DATOS);
	pathArchNormas+= RUTA_ARCHIVO_NORMAS;

	archNormas.open(pathArchNormas.c_str(),ios::out | ios::in);

	if ( archNormas.good()){

		char leido;

		archNormas.seekg(0,ios::beg);
		archNormas.read(&leido,sizeof(char));

		if (leido == 'A') return true;

		return false;
	}

	else return false;

}

archivoNormasInf::~archivoNormasInf() {
	// TODO Auto-generated destructor stub
}
