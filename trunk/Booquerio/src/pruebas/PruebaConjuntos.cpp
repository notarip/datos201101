/*
 * PruebaConjuntos.cpp
 *
 *  Created on: 17/06/2011
 *      Author: rjc
 */

#include "../ProcesadorConsultas/procesadorConsultas.h"
using namespace std;

int main33(){
	list<unsigned int> lista1, lista2,resultado;

	lista1.push_back(1);
	lista1.push_back(5);
	lista1.push_back(12);
	lista1.push_back(32);
	lista1.push_back(66);
	lista1.push_back(89);
	lista1.push_back(13);
	lista1.push_back(45);
	lista1.push_back(9);
	lista1.push_back(8);

	lista2.push_back(8);
	lista2.push_back(89);
	lista2.push_back(50);
	lista2.push_back(24);
	lista2.push_back(32);
	lista2.push_back(17);
	lista2.push_back(13);
	lista2.push_back(0);

	resultado =ProcesadorConsultas().resolverResta(lista1,lista2);

	list<unsigned int>::iterator it = resultado.begin();

	while (it != resultado.end()){
		cout << *it << endl;
		it++;
	}



}
