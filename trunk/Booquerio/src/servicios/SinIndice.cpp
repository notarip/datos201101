/*
 * SinIndice.cpp
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#include "SinIndice.h"


SinIndice::SinIndice(void)
{
	rutaListas = Parametros().getParametro(RUTA_LISTAS);
	if (!Util().existeArchivo(rutaListas))
	{
		ArchivoBloques *manejador = new ArchivoBloques(rutaListas, TAMANIO_B_LISTAS);
		unsigned int nroBloque = 0;
		Bloque *unBloque = new Bloque();

		Registro *listaA = new Registro();
		Registro *listaE = new Registro();
		Registro *listaT = new Registro();
		Registro *listaP = new Registro();

		listaA->agregarAtribEntero(LISTA_AUTORES);
		unBloque->agregarRegistro(*listaA); //autores

		listaE->agregarAtribEntero(LISTA_EDITORIALES);
		unBloque->agregarRegistro(*listaE); //editoriales

		listaT->agregarAtribEntero(LISTA_TITULOS);
		unBloque->agregarRegistro(*listaT); //titulos

		listaP->agregarAtribEntero(LISTA_PALABRAS);
		unBloque->agregarRegistro(*listaP); //palabras

		manejador->grabarBloque(unBloque,nroBloque);
		listaA->~Registro();
		listaE->~Registro();
		listaT->~Registro();
		listaP->~Registro();

		unBloque->~Bloque();
	}

}

SinIndice* SinIndice::pInstancia = 0;
SinIndice* SinIndice::getInstancia()
{
	if (!pInstancia)
			pInstancia = new SinIndice();

	return pInstancia;
}

void SinIndice::agregarLibroPendienteTitulos(unsigned int unId)
{
	this->agregarLibro(unId, LISTA_TITULOS);
}

void SinIndice::agregarLibroPendienteEditoriales(unsigned int unId)
{
	this->agregarLibro(unId, LISTA_EDITORIALES);
}

void SinIndice::agregarLibroPendienteAutores(unsigned int unId)
{
	this->agregarLibro(unId, LISTA_AUTORES);
}

void SinIndice::agregarLibroPendientePalabras(unsigned int unId)
{
	this->agregarLibro(unId, LISTA_PALABRAS);
}



list<unsigned int>* SinIndice::getPendientesTitulos()
{
	return this->getLista(LISTA_TITULOS);

}


list<unsigned int>* SinIndice::getPendientesAutores()
{
	return this->getLista(LISTA_AUTORES);
}

list<unsigned int>* SinIndice::getPendientesEditoriales()
{
	return this->getLista(LISTA_EDITORIALES);
}

list<unsigned int>* SinIndice::getPendientesPalabras()
{
	return this->getLista(LISTA_PALABRAS);
}


void SinIndice::sacarLibroListaAutores(unsigned int unId)
{
	this->borrarLibro(unId, LISTA_AUTORES);

}


void SinIndice::sacarLibroListaEditorial(unsigned int unId)
{
	this->borrarLibro(unId, LISTA_EDITORIALES);

}

void SinIndice::sacarLibroListaTitulos(unsigned int unId)
{
	this->borrarLibro(unId, LISTA_TITULOS);
}

void SinIndice::sacarLibroListaPalabras(unsigned int unId)
{
	this->borrarLibro(unId, LISTA_PALABRAS);
}

void SinIndice::limpiarListaEditoriales()
{
	this->vaciarLista(LISTA_EDITORIALES);
}

void SinIndice::limpiarListaTitulos()
{
	this->vaciarLista(LISTA_TITULOS);
}

void SinIndice::limpiarListaAutores()
{
	this->vaciarLista(LISTA_AUTORES);
}

void SinIndice::limpiarListaPalabras()
{
	this->vaciarLista(LISTA_PALABRAS);
}



void SinIndice::agregarLibro(unsigned int unId, int lista)
{
	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Registro *unRegistro = unBloque->recuperarRegistro(lista);

	unRegistro->agregarReferencia(unId);

	Bloque *nuevoBloque = new Bloque();

	nuevoBloque->agregarRegistro(*unRegistro);

	this->copiarBloque(lista, nuevoBloque, unBloque);

	manejador->grabarBloque(nuevoBloque,0);

	unBloque->~Bloque();

	nuevoBloque->~Bloque();

}


list<unsigned int> *SinIndice::getLista(int lista)
{
	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Registro *unRegistro = unBloque->recuperarRegistro(lista);

	return unRegistro->getReferencias();

}



void SinIndice::borrarLibro(unsigned int unId, int lista)
{
	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Bloque *nuevoBloque = new Bloque();
	Registro *unRegistro = unBloque->recuperarRegistro(lista);
	Registro *nuevoRegistro = new Registro();
	nuevoRegistro->agregarAtribEntero(unRegistro->getAtributosEnteros()->front());
	list<unsigned int> *refs = unRegistro->getReferencias();


	for (list<unsigned int>::iterator it = refs->begin();it != refs->end(); it++ )
		if (*it != unId)
			nuevoRegistro->agregarReferencia(*it);

	nuevoBloque->agregarRegistro(*nuevoRegistro);

	this->copiarBloque(lista, nuevoBloque, unBloque);

	manejador->grabarBloque(nuevoBloque, 0);

	//unRegistro->~Registro();
	//nuevoRegistro->~Registro();
	unBloque->~Bloque();
	nuevoBloque->~Bloque();

	manejador->~ArchivoBloques();
}



void SinIndice::vaciarLista(int lista)
{

	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Bloque *nuevoBloque = new Bloque();
	Registro *unRegistro = unBloque->recuperarRegistro(lista);
	Registro *nuevoRegistro = new Registro();

	nuevoRegistro->agregarAtribEntero(unRegistro->getAtributosEnteros()->front());
	nuevoBloque->agregarRegistro(*nuevoRegistro);

	this->copiarBloque(lista, nuevoBloque, unBloque);

	manejador->grabarBloque(nuevoBloque, 0);

	//unRegistro->~Registro();
	//nuevoRegistro->~Registro();
	unBloque->~Bloque();
	nuevoBloque->~Bloque();

	manejador->~ArchivoBloques();
}

void SinIndice::copiarBloque(int lista, Bloque *nuevoBloque, Bloque *viejoBloque)
{

	if (lista != LISTA_AUTORES)
		nuevoBloque->agregarRegistro(*(viejoBloque->recuperarRegistro(LISTA_AUTORES)));

	if (lista != LISTA_EDITORIALES)
		nuevoBloque->agregarRegistro(*(viejoBloque->recuperarRegistro(LISTA_EDITORIALES)));

	if (lista != LISTA_TITULOS)
		nuevoBloque->agregarRegistro(*(viejoBloque->recuperarRegistro(LISTA_TITULOS)));

	if (lista != LISTA_PALABRAS)
		nuevoBloque->agregarRegistro(*(viejoBloque->recuperarRegistro(LISTA_PALABRAS)));

}


