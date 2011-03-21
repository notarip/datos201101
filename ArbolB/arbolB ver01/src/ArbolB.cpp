/*
 * ArbolB.cpp
 *
 *  Created on: 18/03/2011
 *      Author: pablo
 */

#include "ArbolB.h"

ArbolB::ArbolB() {
	CI 			= (TN-2-sizeof(TRefB))/(sizeof(Elemento) + sizeof(TRefB));
	MitadDerCI 	= CI / 2;
	MitadIzqCI 	= (MitadDerCI + CI) % 2  ;
	CH 			= (TN -2) / sizeof(Elemento);
	MitadDerCH 	= 0;
	MitadIzqCH	= (MitadDerCH + CH) % 2;
	CR 			= (TN - 2) % sizeof(Elemento);
}

ArbolB::~ArbolB() {
	// TODO Auto-generated destructor stub
}


void ArbolB::Crear(string nombre)
{
	fstream arbolArc;
	string rutaArbol = nombre + ".b";
	fstream nodosArc;
	string rutaNodos = nombre + ".el";
	NodoH raiz;


	arbolArc.open(rutaArbol.c_str(),ios::binary | ios::out);
	//aca hay que guardar el nodo en el archivo
	//ojo con los tamaños

	arbolArc.close();

	nodosArc.open(rutaNodos.c_str(),ios::binary | ios::out);
	nodosArc.close();

}


void ArbolB::Abrir()
{
}

void ArbolB::Visualizar()
{
}



void ArbolB::Exportar()
{
}



void ArbolB::Suprimir(TClave clave, string resultado)
{
}



void ArbolB::Cerrar()
{
}



void ArbolB::Insertar(Elemento elemento, string resultado)
{
}



void ArbolB::Buscar(TClave clave, Elemento elemento, string resultado)
{
}


