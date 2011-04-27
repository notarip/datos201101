/*
 * Servicios.cpp
 *
 *  Created on: 22/04/2011
 *      Author: pablo
 */
#define TAMANIO_BLOQUE 4096
#include "Servicios.h"
#include "../hash/Hash.h"
#include "../util/Util.h"
int Servicios::tomarTexto(string ruta)
{

//parsear el libro
	Parser *unParser = new Parser();

	if (unParser->parsear(ruta) != EXITOSO)
	{
		string rutaLibros = Parametros().getParametro(CARPETA_LIBROS);
		ruta = rutaLibros + ruta;
		if (unParser->parsear(ruta) != EXITOSO) return ERROR_RUTA_INVALIDA;
	}

	Libro *unLibro = unParser->getLibro();

	//cout << unLibro->toString() << endl; //BORRAR SOLO PARA PROBAR LA IMPRESION

	unParser->~Parser();


//meter el libro en el archivo de registros variables
	string rutaArcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (rutaArcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	archivo->agregarLibro(unLibro); //tambien lo agrega al arbol primario

	archivo->~ArchivoLibros();


//agregar el libro a las listas
	SinIndice *listas = SinIndice().getInstancia();

	return listas->agregarLibroPendienteATodasLasListas(unLibro->getId());


}

int Servicios::procesarAutores()
{
	return procesarLibro(INDICE_AUTORES);
}


int Servicios::procesarTitulos()
{
	return procesarLibro(INDICE_TITULOS);
}


int Servicios::procesarEditoriales()
{
	return procesarLibro(INDICE_EDITORIALES);
}



int Servicios::procesarPalabras()
{
	return procesarLibro(INDICE_PALABRAS);
}


int Servicios::listarLibros()
{

	string rutaArcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (rutaArcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	list<Libro> *lista = archivo->recuperacionComprensiva();

	cout << "Listado del libros: " << endl;

	for (list<Libro>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		cout << (*it).toString() << endl;
	}

	cout << "Fin listado del libros: " << endl;

	return 0;
}

int Servicios::obtenerLibro(string unId)
{
	unsigned int id = atoi(unId.c_str());
	fstream libro;
	string rutaSalida = Parametros().getParametro(CARPETA_SALIDA);
	string arcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (arcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

 	ArchivoLibros *archivo = new ArchivoLibros(arcLibros);

	Libro *unLibro = archivo->recuperarLibro(id);

// BORRAR
//	Parser *unParser = new Parser();
//	unParser->parsear("./archivos/libros/Justin Somper - Vampiratas 3 - Emboscada en el Océano.txt");
//	Libro *unLibro = unParser->getLibro();
//BORRAR

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
//editorial
	linea += "Editorial: ";
	linea += unLibro->getEditorial();
	linea += "\n";
//titulo
	linea += "Titutlo: ";
	linea += unLibro->getTitulo();
	linea += "\n";
//texto
	linea += "\n\n...\n";
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

	return 0;
}

int Servicios::quitarArchivo(string unId)
{
	unsigned int id = atoi(unId.c_str());

//TODO quitar de los indices

//quitar de la lista de libros sin procesar
	SinIndice *listas  = SinIndice().getInstancia();
	return listas->sacarLibroDeTodasLasListas(id);

//quitar de el archivo de registros variables
	string arcLibros = Parametros().getParametro(ARCHIVO_LIBROS);
	ArchivoLibros *archivo = new ArchivoLibros(arcLibros);
	archivo->suprimirLibro(id);


	return 0;
}

int Servicios::verEstructuraAutor()
{
	//TODO llamar al metodo de arbol de autores que lista la estructura
	return 0;
}

int Servicios::verEstructuraEditorial()
{
	//TODO llamar al metodo de arbol de editoriales que lista la estructura
	return 0;
}

int Servicios::verEstructuraTitulos()
{
	//TODO llamar al metodo de hash de titulos que lista la estructura
	return 0;
}

int Servicios::verEstructuraPalabras()
{
	//TODO llamar al metodo de hash de palabras que lista la estructura
	return 0;
}



int Servicios::procesarLibro(int indice)
{
	int error = 0;
	SinIndice *listas = SinIndice().getInstancia();

	list<unsigned int> *lista;


	switch(indice)
	{
		case INDICE_AUTORES :lista = listas->getPendientesAutores();break;
		case INDICE_EDITORIALES:lista = listas->getPendientesEditoriales();break;
		case INDICE_TITULOS:lista = listas->getPendientesTitulos();break;
		case INDICE_PALABRAS:lista = listas->getPendientesPalabras();break;
	}

	string rutaArcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (rutaArcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	Libro *libro;

	for (list<unsigned int>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		libro = archivo->recuperarLibro(*it);
		switch(indice)
		{
			case INDICE_AUTORES :error = agregarIndiceAutores(libro);break;
			case INDICE_EDITORIALES:error = agregarIndiceEditoriales(libro);break;
			case INDICE_TITULOS:error = agregarIndiceTitulos(libro);break;
			case INDICE_PALABRAS:error = agregarIndicePalabras(libro);break;
		}

		libro->~Libro();
	}


	switch(indice)
	{
		case INDICE_AUTORES :error = listas->limpiarListaAutores();break;
		case INDICE_EDITORIALES:error = listas->limpiarListaEditoriales();break;
		case INDICE_TITULOS:error = listas->limpiarListaTitulos();break;
		case INDICE_PALABRAS:error = listas->limpiarListaPalabras();break;
	}

	archivo->~ArchivoLibros();

	return error;
}


void Servicios::modificarListaIds(string pathHash, string pathListaIds, string clavePasada, unsigned int idNueva){
	Hash hash(parametro);
	ArchivoBloques archivo(parametro2,TAMANIO_BLOQUE);
	Registro* registro= hash.buscar(clavePasada);
	Bloque* bloque=NULL;
	unsigned int referencia=0;
	if (registro){
		referencia= registro->getAtributosEnteros()->back();
		bloque= archivo.recuperarBloque(referencia);
		Registro listaIds= bloque->obtenerRegistros()->front();
		listaIds.agregarAtribEntero(idNueva);

	}
	else{
		referencia= archivo.getBloqueLibre();
		bloque= new Bloque();
		Registro listaIds;
		listaIds.agregarAtribEntero(idNueva);
		bloque->agregarRegistro(listaIds);
	}
	archivo.grabarBloque(bloque,referencia);
}


int Servicios::agregarIndiceAutores(Libro *unLibro)
{
	//logica pertenece al arbol.

	return 0;
}

int Servicios::agregarIndiceEditoriales(Libro *unLibro)
{
	//logica pertenece al arbol.
	return 0;
}

int Servicios::agregarIndiceTitulos(Libro *unLibro)
{
//	string path1= Parametros().getParametro(CARPETA_HASH_TITULOS);
//	string path2= Parametros().getParametro(CARPETA_LISTA_TITULOS);
//	modificarListaIds(path1, path2, unLibro->getAutor(),unLibro->getId());
	return 0;
}

int Servicios::agregarIndicePalabras(Libro *unLibro)
{

	set<string> *palabras = unLibro->getListaPalabras();
	string path1= Parametros().getParametro(CARPETA_HASH_PALABRAS);
	string path2= Parametros().getParametro(CARPETA_LISTA_PALABRAS);

	for (set<string>::iterator it = palabras->begin(); it != palabras->end(); it++)
	{
		modificarListaIds(path1, path2, *it,unLibro->getId());
	}

	return 0;
}
