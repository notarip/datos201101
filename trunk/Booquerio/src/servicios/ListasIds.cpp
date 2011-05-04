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
		cout << "pido bloque nuevo para la lista de ids, me devolvieron: " << *offset  <<endl;
		unBloque = new Bloque();
		unRegistro = new Registro();
		unRegistro->agregarAtribEntero(id);
		unBloque->agregarRegistro(*unRegistro);
		archivo->grabarBloque(unBloque,*offset);

	}else
	{
		unBloque = archivo->recuperarBloque(*offset);
		unRegistro = new Registro();
		*unRegistro = unBloque->obtenerRegistros()->back();
		unRegistro->agregarAtribEntero(id);
		unBloqueNuevo = new Bloque();
		unBloqueNuevo->agregarRegistro(*unRegistro);
		archivo->grabarBloque(unBloqueNuevo,*offset);
	}

	delete archivo;
	delete unBloque;

	return 0;
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

	for (list<unsigned int>::iterator it; it != lista->end(); it++)
	{
		if (*it != id)
			unRegistroNuevo->agregarAtribEntero(*it);
	}

	if (unRegistroNuevo->getAtributosEnteros()->size() > 0)
	{
		unBloqueNuevo = new Bloque();
		unBloqueNuevo->agregarRegistro(*unRegistroNuevo);
		archivo->grabarBloque(unBloqueNuevo,*offset);
		archivo->~ArchivoBloques();
		return 0;
	}else
	{
		archivo->eliminarBloque(*offset);
		archivo->~ArchivoBloques();
		return LISTA_VACIA;
	}


	return 0;
}
