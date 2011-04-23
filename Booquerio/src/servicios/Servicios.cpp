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

	//cout << unLibro->toString() << endl; //BORRAR SOLO PARA PROBAR LA IMPRESION

	unParser->~Parser();


//meter el libro en el archivo de registros variables
	string rutaArcLibros = Parametros().getParametro(CARPETA_ARC_LIBROS);

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


void Servicios::listarLibros()
{
	string rutaArcLibros = Parametros().getParametro(CARPETA_ARC_LIBROS);

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	list<Libro> *lista = archivo->recuperacionComprensiva();

	cout << "Listado del libros: " << endl;

	for (list<Libro>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		cout << (*it).toString() << endl;
	}

	cout << "Fin listado del libros: " << endl;
}

void Servicios::obtenerLibro(string unId)
{
	unsigned int id = atoi(unId.c_str());
	fstream libro;
	string rutaSalida = Parametros().getParametro(CARPETA_SALIDA);
	string arcLibros = Parametros().getParametro(CARPETA_ARC_LIBROS);

	ArchivoLibros *archivo = new ArchivoLibros(arcLibros);

	Libro *unLibro = archivo->recuperarLibro(id);

	string nombreArchivo = rutaSalida + unLibro->getTitulo();
	nombreArchivo += ".txt";

	libro.open(nombreArchivo.c_str(), ios::binary | ios::out);

	unsigned int tamanio;
	char* buff;
	string linea;

//autor
	linea = "Autor: ";
	linea += unLibro->getAutor();
	linea += "\n";
	tamanio = linea.size();
	buff= new char[tamanio];
	memcpy(buff,linea.c_str(),tamanio);
	libro.write(buff,tamanio);
	delete [] buff;

//editorial
	linea = "Editorial: ";
	linea += unLibro->getEditorial();
	linea += "\n";
	tamanio = linea.size();
	buff= new char[tamanio];
	memcpy(buff,linea.c_str(),tamanio);
	libro.write(buff,tamanio);
	delete [] buff;

//titulo
	linea = "Titutlo: ";
	linea += unLibro->getTitulo();
	linea += "\n";
	tamanio = linea.size();
	buff= new char[tamanio];
	memcpy(buff,linea.c_str(),tamanio);
	libro.write(buff,tamanio);
	delete [] buff;


//texto
	linea = "\n\n\n\n...";
	linea += unLibro->getTexto();
	linea += "\n";
	tamanio = linea.size();
	buff= new char[tamanio];
	memcpy(buff,linea.c_str(),tamanio);
	libro.write(buff,tamanio);
	delete [] buff;



	libro.close();

	archivo->~ArchivoLibros();
	unLibro->~Libro();

}

void Servicios::procesarLibro(int indice)
{
	SinIndice *listas = SinIndice().getInstancia();

	list<unsigned int> *lista = listas->getPendientesAutores();

	string rutaArcLibros = Parametros().getParametro(CARPETA_ARC_LIBROS);

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
