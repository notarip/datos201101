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
		//si y tiene enlazados otros bloques voy por el ultimo
		unsigned int ultimoBloq = recuperarUltimoBloque(*offset);

		unBloque = archivo->recuperarBloque(ultimoBloq);
		unRegistro = new Registro();
		*unRegistro = unBloque->obtenerRegistros()->back();
		unRegistro->agregarAtribEntero(id);
		unBloqueNuevo = new Bloque();
		unBloqueNuevo->agregarRegistro(*unRegistro);

		try{

			archivo->grabarBloque(unBloqueNuevo,ultimoBloq);


		}catch (ExceptionBloque &e)
		{
			//pedir un bloque nuevo
			unsigned int nuevoBloqNro = archivo->getBloqueLibre();
			Registro *nuevoReg = new Registro();

			//agregar al registro viejo el nro del bloque nuevo
			unRegistro->agregarReferencia(nuevoBloqNro);



			unsigned int tercero = unRegistro->getAtributosEnteros()->back();

			unRegistro->getAtributosEnteros()->pop_back();

			unsigned int segundo = unRegistro->getAtributosEnteros()->back();

			unRegistro->getAtributosEnteros()->pop_back();

			unsigned int primer = unRegistro->getAtributosEnteros()->back();

			unRegistro->getAtributosEnteros()->pop_back();

			nuevoReg->agregarAtribEntero(primer);
			nuevoReg->agregarAtribEntero(segundo);
			nuevoReg->agregarAtribEntero(tercero);


			//grabar el bloque  viejo
			delete unBloqueNuevo;
			unBloqueNuevo = new Bloque();
			unBloqueNuevo->agregarRegistro(*unRegistro);
			archivo->grabarBloque(unBloqueNuevo,ultimoBloq);


			//grabar el bloque nuevo
			Bloque *nuevoBloq = new Bloque();
			nuevoBloq->agregarRegistro(*nuevoReg);
			archivo->grabarBloque(nuevoBloq,nuevoBloqNro);
			delete nuevoBloq;
			delete nuevoReg;

		}


		delete unBloqueNuevo;

	}

	delete unRegistro;
	delete archivo;
	delete unBloque;

	return 0;
}

unsigned int ListasIds::recuperarUltimoBloque(unsigned int primero)
{

	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);

	Bloque *primerBloque;

	primerBloque = archivo->recuperarBloque(primero);
	Registro unRegistro;

	unRegistro = primerBloque->obtenerRegistros()->back();
	unsigned int nroBloq;
	int ref = 0;

	bool vacio = unRegistro.getReferencias()->empty();

	if (!vacio)
		ref = unRegistro.getReferenciai(1);
	else
		ref = -1;

	nroBloq = primero;

	Bloque *otroBloque;

	while (ref > 0)
	{

		otroBloque = archivo->recuperarBloque(ref);
		nroBloq = ref;

		unRegistro = otroBloque->obtenerRegistros()->front();
		ref = 0;

		bool vacia = unRegistro.getReferencias()->empty();

		if (!vacia)
			ref = unRegistro.getReferenciai(1);
		else
			ref = -1;

		delete otroBloque;
	}

	delete primerBloque;
	//delete otroBloque;
	delete archivo;


	return nroBloq;
}


int ListasIds::eliminarLista(unsigned int offset)
{

	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);

	Bloque *bloque;

	bloque = archivo->recuperarBloque(offset);
	Registro unRegistro;

	unRegistro = bloque->obtenerRegistros()->back();
	unsigned int nroBloq;
	int ref = 0;

	bool vacio = unRegistro.getReferencias()->empty();

	if (!vacio)
		eliminarLista(unRegistro.getReferenciai(1));

	archivo->eliminarBloque(offset);


	return 0;

}

int ListasIds::agregarPosPalabra(unsigned int *offset, unsigned int pos, bool listaNueva)

{
	return agregarIdDeLibro(offset, pos, listaNueva);
}


int ListasIds::sacarIdDelLibro(unsigned int offset, unsigned int id)
{
	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);
	Bloque *unBloque;
	Registro unRegistro;


	bool encontrado = false;
	bool corte = false;
	int ref;

	while (!encontrado && offset >= 0 && !corte)
	{
		unBloque = archivo->recuperarBloque(offset);
		unRegistro = unBloque->obtenerRegistros()->back();

		list<unsigned int> *lista =  unRegistro.getAtributosEnteros();

		for (list<unsigned int>::iterator it = lista->begin(); it != lista->end(); it++)
		{
			if (*it == id)
			{
				unRegistro.getAtributosEnteros()->erase(it);
				encontrado = true;
				break;
			}

		}

		if (!encontrado)
		{
			bool vacio = unRegistro.getReferencias()->empty();

			if (!vacio)
				ref = unRegistro.getReferenciai(1);
			else
				ref = -1;

			if (ref >= 0)
				offset = ref;
			else
				corte = true; //para que corte igual
		}

		delete unBloque;
	}


	if (encontrado)
	{
		unBloque = new Bloque();
		unBloque->agregarRegistro(unRegistro);
		archivo->grabarBloque(unBloque,offset);
		delete archivo;
		delete unBloque;
		return 0;
	}


	return 0;
}

int ListasIds::obtenerListaIds(unsigned int offset, list<unsigned int>* listaIds)
{

	string archivoListas = Parametros().getParametro(ARCHIVO_LISTAS_IDS);
	ArchivoBloques *archivo = new ArchivoBloques(archivoListas,TAMANIO_B_LISTA_IDS);
	Bloque *unBloque;


	Registro unRegistro;


	int primero = offset;
	list<unsigned int> listaEnteros;

	while (primero >= 0)
	{

		unBloque = archivo->recuperarBloque(primero);
		unRegistro = unBloque->obtenerRegistros()->front();

		listaEnteros= *(unRegistro.getAtributosEnteros());
		list<unsigned int>::iterator it=listaEnteros.begin();

		while (it!=listaEnteros.end())
		{
			listaIds->push_back(*it);
			it++;
		}

		bool vacio = unRegistro.getReferencias()->empty();

		if (!vacio)
			primero = unRegistro.getReferenciai(1);
		else
			primero = -1;

		delete unBloque;

	}


	delete archivo;

	return 0;


/*
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
*/
}
