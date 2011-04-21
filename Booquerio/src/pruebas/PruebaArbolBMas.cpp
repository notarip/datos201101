#include "../ArbolB+/ArbolBMasNumerico.h"
#include "string.h"

using namespace std;

int main(){
	string unPath = "ArbolBMas";

	ArbolBMasNumerico* miArbol = new ArbolBMasNumerico(unPath,128);

	resultadoOperacion* unResultado = miArbol->insertarNumerico(9,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "1" << endl;

	unResultado = miArbol->insertarNumerico(8,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "2" << endl;


	unResultado = miArbol->insertarNumerico(122,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "3" << endl;


	unResultado = miArbol->insertarNumerico(12,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "4" << endl;


	unResultado = miArbol->insertarNumerico(22,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "5" << endl;


	unResultado = miArbol->insertarNumerico(35,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "6" << endl;


	unResultado = miArbol->insertarNumerico(84,0);
	delete unResultado;
	miArbol->exportar("prueba");
	cout<< "7" << endl;

	delete miArbol;

}
