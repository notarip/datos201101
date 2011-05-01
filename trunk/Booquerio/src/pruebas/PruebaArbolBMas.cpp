#include "../ArbolB+/ArbolBMasNumerico.h"
#include "string.h"

using namespace std;

int main3(){
	string unPath = "ArbolBMas";

	ArbolBMasNumerico* miArbol = new ArbolBMasNumerico(unPath,200);

	cout << "insercion 9" << endl;
	resultadoOperacion unResultado = miArbol->insertarNumerico(9,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 8" << endl;
	unResultado = miArbol->insertarNumerico(8,0);
	miArbol->exportar("prueba");
	cout << unResultado.getDescripcion() << endl;

	cout << "insercion 122" << endl;
	unResultado = miArbol->insertarNumerico(122,0);
	miArbol->exportar("prueba");
	cout << unResultado.getDescripcion() << endl;

	cout << "insercion 8" << endl;
	unResultado = miArbol->insertarNumerico(8,0);
	miArbol->exportar("prueba");
	cout << unResultado.getDescripcion() << endl;

	cout << "insercion 12" << endl;
	unResultado = miArbol->insertarNumerico(12,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 22" << endl;
	unResultado = miArbol->insertarNumerico(22,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 35" << endl;
	unResultado = miArbol->insertarNumerico(35,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 84" << endl;
	unResultado = miArbol->insertarNumerico(84,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 25" << endl;
	unResultado = miArbol->insertarNumerico(25,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 44" << endl;
	unResultado = miArbol->insertarNumerico(44,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 59" << endl;
	unResultado = miArbol->insertarNumerico(59,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 90" << endl;
	unResultado = miArbol->insertarNumerico(90,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 120" << endl;
	unResultado = miArbol->insertarNumerico(120,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 10" << endl;
	unResultado = miArbol->insertarNumerico(10,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 11" << endl;
	unResultado = miArbol->insertarNumerico(11,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 114" << endl;
	unResultado = miArbol->insertarNumerico(114,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 110" << endl;
	unResultado = miArbol->insertarNumerico(110,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 13" << endl;
	unResultado = miArbol->insertarNumerico(13,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 11" << endl;
	unResultado = miArbol->insertarNumerico(11,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 121" << endl;
	unResultado = miArbol->insertarNumerico(121,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 44" << endl;
	unResultado = miArbol->eliminarNumerico(44);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 84" << endl;
	unResultado = miArbol->eliminarNumerico(84);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 90" << endl;
	unResultado = miArbol->eliminarNumerico(90);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 22" << endl;
	unResultado = miArbol->eliminarNumerico(22);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 12" << endl;
	unResultado = miArbol->eliminarNumerico(12);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 110" << endl;
	unResultado = miArbol->eliminarNumerico(110);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 59" << endl;
	unResultado = miArbol->eliminarNumerico(59);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 122" << endl;
	unResultado = miArbol->eliminarNumerico(122);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 114" << endl;
	unResultado = miArbol->eliminarNumerico(114);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 35" << endl;
	unResultado = miArbol->eliminarNumerico(35);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 150" << endl;
	unResultado = miArbol->insertarNumerico(150,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "insercion 140" << endl;
	unResultado = miArbol->insertarNumerico(140,0);
	cout << unResultado.getDescripcion() << endl;
	miArbol->exportar("prueba");

	cout << "eliminacion 35" << endl;
	unResultado = miArbol->eliminarNumerico(35);
	cout<<unResultado.getDescripcion()<<endl;

	resultadoOperacion resultadoOp(OK);

	cout << "busqueda 120" << endl;
	Registro* regEncontrado= miArbol->buscarRegistroNumerico(120, &resultadoOp);
	cout<<resultadoOp.getDescripcion()<<endl;
	cout<<regEncontrado->getAtributosEnteros()->front()<<endl;

	cout << "busqueda 12" << endl;
	regEncontrado= miArbol->buscarRegistroNumerico(12, &resultadoOp);
	cout<<resultadoOp.getDescripcion()<<endl;
	cout<<regEncontrado->getAtributosEnteros()->front()<<endl;

	cout << "busqueda 10" << endl;
	regEncontrado= miArbol->buscarRegistroNumerico(10, &resultadoOp);
	cout<<resultadoOp.getDescripcion()<<endl;
	cout<<regEncontrado->getAtributosEnteros()->front()<<endl;

	regEncontrado= miArbol->buscarRegistroNumerico(8,&resultadoOp);

	cout << "recuperacion secuencial" <<endl;
	while (regEncontrado!=NULL){
		cout<<regEncontrado->getAtributosEnteros()->front()<<"|";
		regEncontrado= miArbol->siguiente();
	}

	delete miArbol;
	return 0;
}
