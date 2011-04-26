/*
 * PruebaArbolMasAlfabetico.cpp
 *
 *  Created on: 22/04/2011
 *      Author: rjc
 */

#include "../ArbolB+/ArbolBMasAlfabetico.h"
#include "string.h"

using namespace std;

int main(){
	string unPath = "ArbolBMas";

	ArbolBMasAlfabetico* miArbol = new ArbolBMasAlfabetico(unPath,96);

	resultadoOperacion* unResultado = miArbol->insertar("pedro",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("gabriel",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("juan",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("agustina",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("gaston",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("maria",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("mercedes",0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("pablo",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("diego",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("martin",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("lucas",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("juana",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("jose",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("victoria",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("amalia",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("viviana",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("facundo",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("horacio",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("mariela",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("lucia",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("santiago",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("natalia",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("julia",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("julieta",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("juliana",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("julio",0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertar("julian",0);
	miArbol->exportar("prueba");

	unResultado= miArbol->eliminar("julian", 0);
	miArbol->exportar("prueba");

	delete miArbol;
}
