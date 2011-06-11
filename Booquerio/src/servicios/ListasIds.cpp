/*
 * ListasIds.cpp
 *
 *  Created on: 01/05/2011
 *      Author: pablo
 */

#include "ListasIds.h"

int ListasIds::agregarIdDeLibro(unsigned int *offset, unsigned int id, bool listaNueva)
{
	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);
	Bloque *unBloque, *unBloqueNuevo;
	Registro *unRegistro;
	if (listaNueva)
	{
		*offset = archivo->getBloqueLibre();
	//cout << "pido bloque nuevo para la lista de ids, me devolvieron: " << *offset  <<endl;
		unBloque = new Bloque();
		unRegistro = new Registro();
		unRegistro->agregarAtribEntero(id);
		unBloque->agregarRegistro(*unRegistro);
		archivo->grabarBloque(unBloque,*offset);

	}else
	{
		//cout << "ya existe, en que bloque la meto?" << *offset << endl;
		unBloque = archivo->recuperarBloque(*offset);
		unRegistro = new Registro();
		*unRegistro = unBloque->obtenerRegistros()->back();
		unRegistro->agregarAtribEntero(id);
		unBloqueNuevo = new Bloque();
		unBloqueNuevo->agregarRegistro(*unRegistro);
		archivo->grabarBloque(unBloqueNuevo,*offset);
		delete unBloqueNuevo;

	}

	delete unRegistro;
	delete archivo;
	delete unBloque;

	return 0;
}

int ListasIds::agregarPosPalabra(unsigned int *offset, unsigned int pos, bool listaNueva)

{
	return agregarIdDeLibro(offset, pos, listaNueva);
}


int ListasIds::sacarIdDelLibro(unsigned int *offset, unsigned int id)
{
	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);
	Bloque *unBloque, *unBloqueNuevo;
	Registro *unRegistro, *unRegistroNuevo;

	unBloque = archivo->recuperarBloque(*offset);
	unRegistro = new Registro();
	*unRegistro = unBloque->obtenerRegistros()->back();
	unRegistroNuevo = new Registro();

	list<unsigned int> *lista =  unRegistro->getAtributosEnteros();

	for (list<unsigned int>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		if (*it != id)	{
			//cout<<"C"<<*it<< endl;
			unRegistroNuevo->agregarAtribEntero(*it);
		}
	}



	if (unRegistroNuevo->getAtributosEnteros()->size() > 0)
	{//cout<<"A"<<endl;
		unBloqueNuevo = new Bloque();
		unBloqueNuevo->agregarRegistro(*unRegistroNuevo);
		archivo->grabarBloque(unBloqueNuevo,*offset);
		delete archivo;
		return 0;
	}else
	{//cout<<"B"<<endl;cout<<*offset<<endl;
		archivo->eliminarBloque(*offset);
		delete archivo;
		return LISTA_VACIA;
	}

	return 0;
}

int ListasIds::obtenerListaIds(unsigned int offset, list<unsigned int>* listaIds){
	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);
	Bloque *unBloque;
	Registro unRegistro;

	unBloque = archivo->recuperarBloque(offset);
	unRegistro = unBloque->obtenerRegistros()->back();
	list<unsigned int> listaEnteros= *(unRegistro.getAtributosEnteros());
	list<unsigned int>::iterator it=listaEnteros.begin();

	while (it!=listaEnteros.end()){
		listaIds->push_back(*it);
		it++;
	}

	if (listaIds->size() == 0){
		delete unBloque;
		delete archivo;
		return LISTA_VACIA;
	}
	delete unBloque;
	delete archivo;
	return 0;
}
