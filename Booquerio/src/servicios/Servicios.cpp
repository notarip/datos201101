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

//TODO falta que devuelva el offset
	unsigned int offset = 0;
	archivo->agregarLibro(unLibro);
	archivo->~ArchivoLibros();

//TODO agregar al arbol primario, para lo cual falta el offset
	string pathArbolPrimario = Parametros().getParametro(CARPETA_DATOS);
	pathArbolPrimario += NOMBRE_BMAS_PRIMARIO;

	if (pathArbolPrimario == "") return ERROR_RUTA_BMAS_PRIMARIO;

	ArbolBMasNumerico *arbolP = new ArbolBMasNumerico(pathArbolPrimario, TAMANIO_BLOQUE_BMAS_NUMERICO);
	arbolP->insertarNumerico(unLibro->getId(),offset);

	arbolP->~ArbolBMasNumerico();



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
	unsigned int offset = 0;

//recuperar el offset del primario

	string pathArbolPrimario = Parametros().getParametro(CARPETA_DATOS);
	pathArbolPrimario += NOMBRE_BMAS_PRIMARIO;

	if (pathArbolPrimario == "") return ERROR_RUTA_BMAS_PRIMARIO;

	ArbolBMasNumerico *arbolP = new ArbolBMasNumerico(pathArbolPrimario, TAMANIO_BLOQUE_BMAS_NUMERICO);

	resultadoOperacion *resultado = 0;

	Registro* registro = arbolP->buscarRegistroNumerico(id, resultado);

	//TODO ver si esta recuperando bien el offset
	if (registro && resultado->getDescripcion() == "ENCONTRADO")
		offset = registro->getReferencias()->front();
	else
		return ERROR_LIBRO_INEXISTENTE;


//quitar de el archivo de registros variables
	string arcLibros = Parametros().getParametro(ARCHIVO_LIBROS);
	ArchivoLibros *archivo = new ArchivoLibros(arcLibros);
	Libro *libro = archivo->recuperarLibro(offset);
	archivo->suprimirLibro(offset);


//quitar del primario


	arbolP->eliminarNumerico(libro->getId(), offset);

	arbolP->~ArbolBMasNumerico();

//quitar de los indices

	sacarDelArbol(NOMBRE_BMAS_AUTORES,libro->getAutor(),libro->getId());

	sacarDelArbol(NOMBRE_BMAS_EDITORIALES,libro->getEditorial(),libro->getId());

	sacarDelHash(NOMBRE_HASH_TITULOS,libro->getTitulo(),libro->getId());

	set<string> *palabras = libro->getListaPalabras();

	for (set<string>::iterator it = palabras->begin(); it != palabras->end(); it++)
	{
		sacarDelHash(NOMBRE_HASH_PALABRAS, *it ,libro->getId());
	}


//quitar de la lista de libros sin procesar
	SinIndice *listas  = SinIndice().getInstancia();
	return listas->sacarLibroDeTodasLasListas(id);


	archivo->~ArchivoLibros();
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

	for (list<unsigned int>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		Libro *libro = 0;
		int error = recuperarLibro((*it), libro);

		switch(indice)
		{
			case INDICE_AUTORES :error = agregarIndiceAutores(libro);break;
			case INDICE_EDITORIALES:error = agregarIndiceEditoriales(libro);break;
			case INDICE_TITULOS:error = agregarIndiceTitulos(libro);break;
			case INDICE_PALABRAS:error = agregarIndicePalabras(libro);break;
		}

		libro->~Libro();
	}

//limpia la lista de ids pendientes de procesar
	switch(indice)
	{
		case INDICE_AUTORES :error = listas->limpiarListaAutores();break;
		case INDICE_EDITORIALES:error = listas->limpiarListaEditoriales();break;
		case INDICE_TITULOS:error = listas->limpiarListaTitulos();break;
		case INDICE_PALABRAS:error = listas->limpiarListaPalabras();break;
	}

	return error;
}


int Servicios::agregarIndiceAutores(Libro *unLibro)
{
	//logica pertenece al arbol.
	agregarAlArbol(NOMBRE_BMAS_AUTORES, unLibro->getAutor(), unLibro->getId());

	return 0;
}

int Servicios::agregarIndiceEditoriales(Libro *unLibro)
{
	//logica pertenece al arbol.
	agregarAlArbol(NOMBRE_BMAS_EDITORIALES, unLibro->getAutor(), unLibro->getId());

	return 0;
}

int Servicios::agregarIndiceTitulos(Libro *unLibro)
{
	agregarAlHash(NOMBRE_HASH_TITULOS,unLibro->getTitulo(),unLibro->getId());

	return 0;
}

int Servicios::agregarIndicePalabras(Libro *unLibro)
{

	set<string> *palabras = unLibro->getListaPalabras();

	for (set<string>::iterator it = palabras->begin(); it != palabras->end(); it++)
	{
		agregarAlHash(NOMBRE_HASH_PALABRAS, *it ,unLibro->getId());
	}

	return 0;
}

void Servicios::agregarAlHash(string nombreHash, string clavePasada, unsigned int idNueva)
{
	string pathHash = Parametros().getParametro(CARPETA_DATOS);
	pathHash += nombreHash;
	Hash *hash  = new Hash(pathHash);

	Registro* registro= hash->buscar(clavePasada);
	unsigned int offset;

	if (registro){

		offset = registro->getAtributosEnteros()->front();
		ListasIds().agregarIdDeLibro(&offset,idNueva,false);
	}
	else{
		ListasIds().agregarIdDeLibro(&offset, idNueva,true);
		registro = new Registro();
		registro->setString(clavePasada);
		registro->agregarAtribEntero(offset);
		hash->insertar(registro);
	}

	hash->~Hash();
}

void Servicios::agregarAlArbol(string nombreArbol, string clavePasada, unsigned int idNueva)
{
	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += nombreArbol;
	ArbolBMasAlfabetico *arbol  = new ArbolBMasAlfabetico(pathArbol, TAMANIO_BLOQUE_BMAS);

	resultadoOperacion *resultado = 0;

	Registro* registro= arbol->buscarRegistro(clavePasada, resultado);
	unsigned int offset;


	if (registro && resultado->getDescripcion() == "ENCONTRADO")
	{
		offset = registro->getAtributosEnteros()->front();

		ListasIds().agregarIdDeLibro(&offset,idNueva,false);
	}
	else{
		ListasIds().agregarIdDeLibro(&offset, idNueva,true);
		arbol->insertar(clavePasada,offset);
	}

	arbol->~ArbolBMasAlfabetico();
}

void Servicios::sacarDelHash(string nombreHash, string clave, unsigned int idLibro)
{

	string pathHash = Parametros().getParametro(CARPETA_DATOS);
	pathHash += nombreHash;
	Hash *hash  = new Hash(pathHash);

	Registro* registro= hash->buscar(clave);
	unsigned int offset;

	if (registro){

		offset = registro->getAtributosEnteros()->front();
		if (ListasIds().sacarIdDelLibro(&offset,idLibro) == LISTA_VACIA)
			hash->borrar(clave);
	}

	hash->~Hash();

}


void Servicios::sacarDelArbol(string nombreArbol, string clave, unsigned int idLibro)
{

	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += nombreArbol;
	ArbolBMasAlfabetico *arbol  = new ArbolBMasAlfabetico(pathArbol, TAMANIO_BLOQUE_BMAS);

	resultadoOperacion *resultado = 0;

	Registro* registro= arbol->buscarRegistro(clave, resultado);
	unsigned int offset;


	if (registro && resultado->getDescripcion() == "ENCONTRADO")
	{
		offset = registro->getAtributosEnteros()->front();

		if (ListasIds().sacarIdDelLibro(&offset,idLibro) == LISTA_VACIA)
			//TODO por que recibe el 2do parametro ?
			arbol->eliminar(clave,offset);
	}

	arbol->~ArbolBMasAlfabetico();

}


int Servicios::recuperarLibro(unsigned int idLibro, Libro *libro)
{
	unsigned int offset = 0;

	string pathArbolPrimario = Parametros().getParametro(CARPETA_DATOS);
	pathArbolPrimario += NOMBRE_BMAS_PRIMARIO;

	ArbolBMasNumerico *arbolP = new ArbolBMasNumerico(pathArbolPrimario, TAMANIO_BLOQUE_BMAS_NUMERICO);

	resultadoOperacion *resultado = 0;

	Registro* registro = arbolP->buscarRegistroNumerico(idLibro, resultado);

	//TODO ver si esta recuperando bien el offset
	if (registro && resultado->getDescripcion() == "ENCONTRADO")
		offset = registro->getReferencias()->front();
	else
		return ERROR_LIBRO_INEXISTENTE;


	string rutaArcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (rutaArcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	libro = archivo->recuperarLibro(offset);

	archivo->~ArchivoLibros();

	arbolP->~ArbolBMasNumerico();

	return 0;

}
