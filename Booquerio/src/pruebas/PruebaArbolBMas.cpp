#include "../ArbolB+/ArbolBMasNumerico.h"
#include "string.h"

using namespace std;

int main4(){
	string unPath = "ArbolBMas";

	ArbolBMasNumerico* miArbol = new ArbolBMasNumerico(unPath,96);

	resultadoOperacion* unResultado = miArbol->insertarNumerico(9,0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(8,0);
	delete unResultado;
	miArbol->exportar("prueba");


	unResultado = miArbol->insertarNumerico(122,0);
	delete unResultado;
	miArbol->exportar("prueba");


	unResultado = miArbol->insertarNumerico(12,0);
	delete unResultado;
	miArbol->exportar("prueba");


	unResultado = miArbol->insertarNumerico(22,0);
	delete unResultado;
	miArbol->exportar("prueba");


	unResultado = miArbol->insertarNumerico(35,0);
	delete unResultado;
	miArbol->exportar("prueba");


	unResultado = miArbol->insertarNumerico(84,0);
	delete unResultado;
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(25,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(44,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(59,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(90,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(120,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(10,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(11,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(114,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(110,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(13,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(121,0);
	miArbol->exportar("prueba");



	delete miArbol;
}
