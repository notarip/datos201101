/*
 * SinIndice.cpp
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#include "SinIndice.h"


SinIndice::SinIndice(void)
{
	rutaListas = "";
	rutaListas = Parametros().getParametro(ARCHIVO_LISTAS);

	if (rutaListas != "" && !Util().existeArchivo(rutaListas))

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

		delete listaA;
		delete listaE;
		delete listaT;
		delete listaP;

		delete unBloque;
		delete manejador;
	}

}

SinIndice* SinIndice::pInstancia = 0;
SinIndice* SinIndice::getInstancia()
{
	if (!pInstancia)
			pInstancia = new SinIndice();

	return pInstancia;
}

int SinIndice::agregarLibroPendienteTitulos(unsigned int unId)
{
	return this->agregarLibro(unId, LISTA_TITULOS);
}

int SinIndice::agregarLibroPendienteEditoriales(unsigned int unId)
{
	return this->agregarLibro(unId, LISTA_EDITORIALES);
}

int SinIndice::agregarLibroPendienteAutores(unsigned int unId)
{
	return this->agregarLibro(unId, LISTA_AUTORES);
}

int SinIndice::agregarLibroPendientePalabras(unsigned int unId)
{
	return this->agregarLibro(unId, LISTA_PALABRAS);
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


int SinIndice::sacarLibroListaAutores(unsigned int unId)
{
	return this->borrarLibro(unId, LISTA_AUTORES);

}


int SinIndice::sacarLibroListaEditorial(unsigned int unId)
{
	return this->borrarLibro(unId, LISTA_EDITORIALES);

}

int SinIndice::sacarLibroListaTitulos(unsigned int unId)
{
	return this->borrarLibro(unId, LISTA_TITULOS);
}

int SinIndice::sacarLibroListaPalabras(unsigned int unId)
{
	return this->borrarLibro(unId, LISTA_PALABRAS);
}

int SinIndice::limpiarListaEditoriales()
{
 	return this->vaciarLista(LISTA_EDITORIALES);
}

int SinIndice::limpiarListaTitulos()
{
	return this->vaciarLista(LISTA_TITULOS);
}

int SinIndice::limpiarListaAutores()
{
	return this->vaciarLista(LISTA_AUTORES);
}

int SinIndice::limpiarListaPalabras()
{
	return this->vaciarLista(LISTA_PALABRAS);
}



int SinIndice::agregarLibro(unsigned int unId, int lista)
{
	if (this->rutaListas == "") return ERROR_ARCHIVO_LISTAS;

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

	manejador->~ArchivoBloques();

	return 0;

}

int SinIndice::agregarLibroPendienteATodasLasListas(unsigned int unId)
{

	if (this->rutaListas == "") return ERROR_ARCHIVO_LISTAS;

	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Registro *unRegistroA = unBloque->recuperarRegistro(LISTA_AUTORES);
	Registro *unRegistroE = unBloque->recuperarRegistro(LISTA_EDITORIALES);
	Registro *unRegistroT = unBloque->recuperarRegistro(LISTA_TITULOS);
	Registro *unRegistroP = unBloque->recuperarRegistro(LISTA_PALABRAS);

	unRegistroA->agregarReferencia(unId);
	unRegistroE->agregarReferencia(unId);
	unRegistroT->agregarReferencia(unId);
	unRegistroP->agregarReferencia(unId);

	Bloque *nuevoBloque = new Bloque();

	nuevoBloque->agregarRegistro(*unRegistroA);
	nuevoBloque->agregarRegistro(*unRegistroE);
	nuevoBloque->agregarRegistro(*unRegistroT);
	nuevoBloque->agregarRegistro(*unRegistroP);

	manejador->grabarBloque(nuevoBloque,0);

	unBloque->~Bloque();
	nuevoBloque->~Bloque();
	manejador->~ArchivoBloques();

	return 0;

}

int SinIndice::sacarLibroDeTodasLasListas(unsigned int unId)
{

	if (this->rutaListas == "") return ERROR_ARCHIVO_LISTAS;

//recuperar el bloque
	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);

//recuperar las litas
	Registro *unRegistroA = unBloque->recuperarRegistro(LISTA_AUTORES);
	Registro *unRegistroE = unBloque->recuperarRegistro(LISTA_EDITORIALES);
	Registro *unRegistroT = unBloque->recuperarRegistro(LISTA_TITULOS);
	Registro *unRegistroP = unBloque->recuperarRegistro(LISTA_PALABRAS);

//armar un bloque nuevo
	Bloque *nuevoBloque = new Bloque();
	Registro *nuevoRegistroA = new Registro();
	Registro *nuevoRegistroE = new Registro();
	Registro *nuevoRegistroT = new Registro();
	Registro *nuevoRegistroP = new Registro();

	nuevoRegistroA->agregarAtribEntero(unRegistroA->getAtributosEnteros()->front());
	nuevoRegistroE->agregarAtribEntero(unRegistroE->getAtributosEnteros()->front());
	nuevoRegistroT->agregarAtribEntero(unRegistroT->getAtributosEnteros()->front());
	nuevoRegistroP->agregarAtribEntero(unRegistroP->getAtributosEnteros()->front());


	list<unsigned int> *refs;

	refs = unRegistroA->getReferencias();
	for (list<unsigned int>::iterator it = refs->begin();it != refs->end(); it++ )
		if (*it != unId)
			nuevoRegistroA->agregarReferencia(*it);

	refs = unRegistroE->getReferencias();
	for (list<unsigned int>::iterator it = refs->begin();it != refs->end(); it++ )
		if (*it != unId)
			nuevoRegistroE->agregarReferencia(*it);

	refs = unRegistroT->getReferencias();
	for (list<unsigned int>::iterator it = refs->begin();it != refs->end(); it++ )
		if (*it != unId)
			nuevoRegistroT->agregarReferencia(*it);

	refs = unRegistroP->getReferencias();
	for (list<unsigned int>::iterator it = refs->begin();it != refs->end(); it++ )
		if (*it != unId)
			nuevoRegistroP->agregarReferencia(*it);


	nuevoBloque->agregarRegistro(*nuevoRegistroA);
	nuevoBloque->agregarRegistro(*nuevoRegistroE);
	nuevoBloque->agregarRegistro(*nuevoRegistroT);
	nuevoBloque->agregarRegistro(*nuevoRegistroP);

	manejador->grabarBloque(nuevoBloque, 0);

	unBloque->~Bloque();
	nuevoBloque->~Bloque();
	manejador->~ArchivoBloques();

	return 0;
}


list<unsigned int> *SinIndice::getLista(int lista)
{
	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Registro *unRegistro = unBloque->recuperarRegistro(lista);

	delete manejador;
	//delete unBloque;

	return unRegistro->getReferencias();

}



int SinIndice::borrarLibro(unsigned int unId, int lista)
{
	if (this->rutaListas == "") return ERROR_ARCHIVO_LISTAS;

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


	unBloque->~Bloque();
	nuevoBloque->~Bloque();

	manejador->~ArchivoBloques();

	return 0;
}



int SinIndice::vaciarLista(int lista)
{

	if (this->rutaListas == "") return ERROR_ARCHIVO_LISTAS;

	ArchivoBloques *manejador = new ArchivoBloques(this->rutaListas, TAMANIO_B_LISTAS);
	Bloque *unBloque = manejador->recuperarBloque(0);
	Bloque *nuevoBloque = new Bloque();
	Registro *unRegistro = unBloque->recuperarRegistro(lista);
	Registro *nuevoRegistro = new Registro();

	nuevoRegistro->agregarAtribEntero(unRegistro->getAtributosEnteros()->front());
	nuevoBloque->agregarRegistro(*nuevoRegistro);

	this->copiarBloque(lista, nuevoBloque, unBloque);

	manejador->grabarBloque(nuevoBloque, 0);

	delete manejador;
	delete unBloque;
	delete nuevoBloque;
	//delete unRegistro;
	//delete nuevoRegistro;




	return 0;
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


