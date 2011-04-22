/*
 * Servicios.cpp
 *
 *  Created on: 22/04/2011
 *      Author: pablo
 */

#include "Servicios.h"


void Servicios::tomarTexto(string ruta)
{

	//parsear el libro
	Parser *unParser = new Parser();

	if (unParser->parsear(ruta) != EXITOSO)
	{
		string rutaLibros = Parametros().getParametro(CARPETA_LIBROS);
		ruta = rutaLibros + ruta;
		unParser->parsear(ruta);
	}

	Libro *unLibro = unParser->getLibro();

	unParser->~Parser();


	//meter el libro en el arhivo de registros variables
	string rutaArcLibros = Parametros().getParametro(RUTA_ARC_LIBROS);

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);


	archivo->agregarLibro(unLibro); //tambien lo agrega al arbol primario

	archivo->~ArchivoLibros();


	//agregar el libro a las listas
	SinIndice *listas = SinIndice().getInstancia();

	listas->agregarLibroPendienteATodasLasListas(unLibro->getId());


}

void Servicios::procesarAutores()
{
	procesarLibro(INDICE_AUTORES);
}


void Servicios::procesarTitulos()
{
	procesarLibro(INDICE_TITULOS);
}


void Servicios::procesarEditoriales()
{
	procesarLibro(INDICE_EDITORIALES);
}



void Servicios::procesarPalabras()
{
	procesarLibro(INDICE_PALABRAS);
}


void Servicios::procesarLibro(int indice)
{
	SinIndice *listas = SinIndice().getInstancia();

	list<unsigned int> *lista = listas->getPendientesAutores();

	string rutaArcLibros = Parametros().getParametro(RUTA_ARC_LIBROS);

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	Libro *libro;

	for (list<unsigned int>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		libro = archivo->recuperarLibro(*it);
		switch(indice)
		{
			case INDICE_AUTORES :agregarIndiceAutores(libro);break;
			case INDICE_EDITORIALES:agregarIndiceEditoriales(libro);break;
			case INDICE_TITULOS:agregarIndiceTitulos(libro);break;
			case INDICE_PALABRAS:agregarIndicePalabras(libro);break;
		}

		libro->~Libro();
	}

	listas->limpiarListaAutores();

	archivo->~ArchivoLibros();
}

void Servicios::agregarIndiceAutores(Libro *unLibro)
{
	//TODO agregar al indice de autores
}

void Servicios::agregarIndiceEditoriales(Libro *unLibro)
{
	//TODO agregar al indice de editoriales
}

void Servicios::agregarIndiceTitulos(Libro *unLibro)
{
	//TODO agregar al indice de titulos
}

void Servicios::agregarIndicePalabras(Libro *unLibro)
{
	Parser *unParser = new Parser();

	set<string> *palabras = unParser->obtenerPalabras(*unLibro);

	for (set<string>::iterator it = palabras->begin(); it != palabras->end(); it++)
	{
		//TODO agregar al indice de palabras
	}

	unParser->~Parser();

}
