/*
 * PruebaIds.cpp
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#include <iostream>
#include "../servicios/SinIndice.h"
#include "../util/Util.h"

void listar(SinIndice *listas)
{
	list<unsigned int> *la = listas->getPendientesAutores();

	for (list<unsigned int>::iterator ita = la->begin(); ita != la->end(); ita++)
		cout << "Autores -> " << *ita << endl;

	list<unsigned int> *le = listas->getPendientesEditoriales();

	for (list<unsigned int>::iterator ite = le->begin(); ite != le->end(); ite++)
		cout << "Editoriales -> " << *ite << endl;

	list<unsigned int> *lt = listas->getPendientesTitulos();

	for (list<unsigned int>::iterator itt = lt->begin(); itt != lt->end(); itt++)
		cout << "Titulos -> " << *itt << endl;

	list<unsigned int> *lp = listas->getPendientesPalabras();

	for (list<unsigned int>::iterator itp = lp->begin(); itp != lp->end(); itp++)
		cout << "Palabras -> " << *itp << endl;


}

int main_90(int argc, char** argv)
{
	SinIndice *listas = SinIndice().getInstancia();

	listas->agregarLibroPendienteAutores(1);
	listas->agregarLibroPendienteAutores(2);
	listas->agregarLibroPendienteAutores(3);

	listas->agregarLibroPendienteEditoriales(4);
	listas->agregarLibroPendienteEditoriales(5);
	listas->agregarLibroPendienteEditoriales(6);

	listas->agregarLibroPendienteTitulos(7);
	listas->agregarLibroPendienteTitulos(8);
	listas->agregarLibroPendienteTitulos(9);

	listas->agregarLibroPendientePalabras(10);
	listas->agregarLibroPendientePalabras(11);
	listas->agregarLibroPendientePalabras(12);

	listar(listas);

	listas->sacarLibroListaAutores(2);

	listas->sacarLibroListaEditorial(4);

	listas->sacarLibroListaTitulos(9);

	listas->sacarLibroListaPalabras(10);

	cout << "eliminados 2, 4, 9, 10" << endl;

	listar(listas);

	listas->limpiarListaAutores();

	cout << "eliminados autores" << endl;

	listar(listas);




	return 0;
}
