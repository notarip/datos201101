#include "../ArbolB+/ArbolBMasNumerico.h"
#include "string.h"

using namespace std;

int main(){
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

	unResultado = miArbol->eliminarNumerico(44,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(84,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(90,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(22,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(12,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(110,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(59,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(122,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(114,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->eliminarNumerico(35,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(150,0);
	miArbol->exportar("prueba");

	unResultado = miArbol->insertarNumerico(140,0);
	miArbol->exportar("prueba");

	resultadoOperacion resultadoOp(OK);
	Registro* regEncontrado= miArbol->buscarRegistroNumerico(120, &resultadoOp);
	cout<<resultadoOp.getDescripcion()<<endl;
	cout<<regEncontrado->getAtributosEnteros()->front()<<endl;

	regEncontrado= miArbol->buscarRegistroNumerico(12, &resultadoOp);
	cout<<resultadoOp.getDescripcion()<<endl;
	cout<<regEncontrado->getAtributosEnteros()->front()<<endl;

	regEncontrado= miArbol->buscarRegistroNumerico(10, &resultadoOp);
	cout<<resultadoOp.getDescripcion()<<endl;
	cout<<regEncontrado->getAtributosEnteros()->front()<<endl;

	regEncontrado= miArbol->buscarRegistroNumerico(8,&resultadoOp);

	while (regEncontrado!=NULL){
		cout<<regEncontrado->getAtributosEnteros()->front()<<"|";
		regEncontrado= miArbol->siguiente();
	}

	delete miArbol;
	return 0;
}
