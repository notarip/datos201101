/*
 * pruebaMap.cpp
 *
 *  Created on: 11/06/2011
 *      Author: pablo
 */

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "../archivos/Registro.h"
#include "../archivos/Bloque.h"
#include "../archivos/ArchivoBloques.h"
#include "../servicios/ListasIds.h"


using namespace std;

int main_pepepe(int argc, char** argv)
{

//	map<string, list<int> > mapa;

	cout << "aááááááááááááááááááá" << endl;

//	mapa["par1"].push_back(1);
//	mapa["par1"].push_back(2);
//	mapa["par1"].push_back(3);
//
//	cout << mapa.find("par1")->second.front();
//	cout << mapa.find("par1")->second.back();

	unsigned int offset;

	ListasIds().agregarPosPalabra(&offset,1,true);

	for (int i = 2; i < 100; i++)
		ListasIds().agregarPosPalabra(&offset,i,false);


	list<unsigned int> *lista = new list<unsigned int>();
	ListasIds().obtenerListaIds(offset,lista);

	for (list<unsigned int>::iterator it = lista->begin(); it != lista->end() ; it ++)
		cout << *it << endl;


	ListasIds().sacarIdDelLibro(offset,13);

	cout << "--------------------" << endl;

	lista->clear();
	ListasIds().obtenerListaIds(offset,lista);

	for (list<unsigned int>::iterator it = lista->begin(); it != lista->end() ; it ++)
		cout << *it << endl;

	lista->clear();
	delete lista;
	return 0;

}

