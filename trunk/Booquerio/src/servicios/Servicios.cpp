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
	cout << "Procesando Libro...." << endl;
	if (unParser->parsear(ruta) != EXITOSO)
	{
		string rutaLibros = Parametros().getParametro(CARPETA_LIBROS);
		ruta = rutaLibros + ruta;
		if (unParser->parsear(ruta) != EXITOSO) return ERROR_RUTA_INVALIDA;
	}

	Libro *unLibro = unParser->getLibro();

	//cout << unLibro->toString() << endl; //BORRAR SOLO PARA PROBAR LA IMPRESION

	unParser->~Parser();

	cout << "parsie el libro" << endl;


//meter el libro en el archivo de registros variables
	string rutaArcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (rutaArcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	cout << "creo archivo de libros" << endl;



	unsigned int offset = 0;
	offset = archivo->agregarLibro(unLibro);
	archivo->~ArchivoLibros();

	cout << "agrego el libro" << endl;



	string pathArbolPrimario = Parametros().getParametro(CARPETA_DATOS);
	pathArbolPrimario += NOMBRE_BMAS_PRIMARIO;

	if (pathArbolPrimario == "") return ERROR_RUTA_BMAS_PRIMARIO;

	ArbolBMasNumerico *arbolP = new ArbolBMasNumerico(pathArbolPrimario, TAMANIO_BLOQUE_BMAS_NUMERICO);
	cout << "id: "<< unLibro->getId() << endl;
	arbolP->insertarNumerico(unLibro->getId(),offset);

	delete arbolP;

	cout << "agrego al indice primario" << endl;


//agregar el libro a las listas
	SinIndice *listas = SinIndice().getInstancia();

	return listas->agregarLibroPendienteATodasLasListas(unLibro->getId());

	cout << "agrego a las listas no procesadas" << endl;


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
	//list<Libro> *lista = archivo->recuperacionComprensiva(); //en vez de usar el metodo de hernan
	//uso el metodo siguiente del primario

	list<Libro> *lista = new list<Libro>;
	string pathArbolPrimario = Parametros().getParametro(CARPETA_DATOS);
	pathArbolPrimario += NOMBRE_BMAS_PRIMARIO;

	ArbolBMasNumerico* arbolPrimario = new ArbolBMasNumerico(pathArbolPrimario,TAMANIO_BLOQUE_BMAS_NUMERICO);
	//para iniciar la recuperasion secuencial busco primero el id 0 = el primero
	resultadoOperacion resultadoBus(OK);
	Registro* registroObtenido = arbolPrimario->buscarRegistroNumerico(0,&resultadoBus);

	unsigned int unId;
	Libro* unLibro = NULL;

	//armo las lista de libros con el metodo siguiente del arbol
	while (	registroObtenido != NULL){
		unId = registroObtenido->getAtributosEnteros()->front();
		recuperarLibro(unId, &unLibro);
		//agrego un libro a la lista de libros
		lista->push_back(*unLibro);
		registroObtenido = arbolPrimario->siguiente();
	}

	delete arbolPrimario;

	cout << "Listado del libros: " << endl;
	unsigned int i = 1;

	for (list<Libro>::iterator it = lista->begin(); it != lista->end(); it++)
	{
		cout<<"**************************  "<< i << "  **************************" << endl;
		cout << (*it).toString() << endl;
		i++;
	}
	cout << "Fin listado del libros: " << endl;

	delete lista;
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
	Libro *unLibro = NULL;
	//intento recuperar el libro
	int resultadoRecuperacion = 0;
	resultadoRecuperacion = recuperarLibro(id,&unLibro);
	if( resultadoRecuperacion == ERROR_LIBRO_INEXISTENTE){
		return resultadoRecuperacion;
	};
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
	linea += "Titulo: ";
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

	resultadoOperacion resultado(OK);

	Registro* registro = arbolP->buscarRegistroNumerico(id, &resultado);

	//TODO ver si esta recuperando bien el offset
	if (registro && resultado.getDescripcion() == "ENCONTRADO")
		offset = registro->getReferencias()->front();
	else
		return ERROR_LIBRO_INEXISTENTE;


//quitar de el archivo de registros variables

	string arcLibros = Parametros().getParametro(ARCHIVO_LIBROS);
	ArchivoLibros *archivo = new ArchivoLibros(arcLibros);
	Libro *libro = archivo->recuperarLibro(offset);
	cout <<"Autor: "<< libro->getAutor() <<endl;
	cout <<"Editorial: "<< libro->getEditorial() <<endl;
	cout <<"Titulo: "<< libro->getTitulo() <<endl;
	cout <<"Id: "<< libro->getId() <<endl;
	cout <<"Cant.palabras: " << libro->getCantPalabras() << endl;
	cout << "Presione ENTER para continuar...." << endl;
	cin.get();

	cout << "SACO EL LIBRO DEL ARCHIVO DE REGISTROS VARIABLES" << endl;
	archivo->suprimirLibro(offset);


//quitar del primario

	cout << "QUITO DEL PRIMARIO EL ID" << endl;

	arbolP->eliminarNumerico(libro->getId());

	delete arbolP;

//quitar de los indices

	cout << "SACO DE LOS INDICES" << endl;

	cout << "SACO DE AUTORES" << endl;
	sacarDelArbol(NOMBRE_BMAS_AUTORES,libro->getAutor(),libro->getId());
	cout << "Presione ENTER para continuar...." << endl;
	cin.get();

	cout << "SACO DE EDITORIALES" << endl;
	sacarDelArbol(NOMBRE_BMAS_EDITORIALES,libro->getEditorial(),libro->getId());
	cout << "Presione ENTER para continuar...." << endl;
	cin.get();

	cout << "SACO DE TITULOS" << endl;
	sacarDelHash(NOMBRE_HASH_TITULOS,libro->getTitulo(),libro->getId());
	cout << "Presione ENTER para continuar...." << endl;
	cin.get();

	cout << "SACO PALABRAS" << endl;
	cout << "Procesando la quita del indice..." << endl;
	set<string> *palabras = libro->getListaPalabras();

	for (set<string>::iterator it = palabras->begin(); it != palabras->end(); it++)
	{
		sacarDelHash(NOMBRE_HASH_PALABRAS, *it ,libro->getId());
	}


//quitar de la lista de libros sin procesar
	cout << "SACO DE LOS INDICES SIN PROCESAR" <<endl;
	SinIndice *listas  = SinIndice().getInstancia();
	return listas->sacarLibroDeTodasLasListas(id);


	delete archivo;
	return 0;
}

int Servicios::verEstructuraAutor(string path)
{
	string pathExport;
	if (path == "")
	{
		pathExport = Parametros().getParametro(CARPETA_SALIDA);
		pathExport += NOMBRE_BMAS_AUTORES;
	}
	else
	{
		pathExport = path;
	}

	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += NOMBRE_BMAS_AUTORES;

	ArbolBMasAlfabetico* arbolA = new ArbolBMasAlfabetico(pathArbol,TAMANIO_BLOQUE_BMAS);
	arbolA->exportar(pathExport);
	delete arbolA;
	return 0;
}

int Servicios::verEstructuraEditorial(string path)
{
	string pathExport;
	if (path == "")
	{
		pathExport = Parametros().getParametro(CARPETA_SALIDA);
		pathExport += NOMBRE_BMAS_EDITORIALES;
	}
	else
	{
		pathExport = path;
	}
	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += NOMBRE_BMAS_EDITORIALES;

	ArbolBMasAlfabetico* arbolE = new ArbolBMasAlfabetico(pathArbol,TAMANIO_BLOQUE_BMAS);
	arbolE->exportar(pathExport);
	delete arbolE;
	return 0;

}

int Servicios::verEstructuraPrimario(string path)
{
	string pathExport;
	if (path == "")
	{
		pathExport = Parametros().getParametro(CARPETA_SALIDA);
		pathExport += NOMBRE_BMAS_PRIMARIO;
	}
	else
	{
		pathExport = path;
	}
	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += NOMBRE_BMAS_PRIMARIO;

	ArbolBMasNumerico* arbolE = new ArbolBMasNumerico(pathArbol,TAMANIO_BLOQUE_BMAS);
	arbolE->exportar(pathExport);
	delete arbolE;
	return 0;


}

int Servicios::verEstructuraTitulos(string path)
{
	//TODO llamar al metodo de hash de titulos que lista la estructura

	/* LLEEEEEER*/
	//me tome el atrevimiento de usar la interfaz para pruebas, este metodo por ahora va a exportar arbol primario
	string pathExport;
	if (path == "")
	{
		pathExport = Parametros().getParametro(CARPETA_SALIDA);
		pathExport += NOMBRE_HASH_TITULOS;
	}
	else
	{
		pathExport = path;
	}
	string pathHash = Parametros().getParametro(CARPETA_DATOS);
	pathHash += NOMBRE_HASH_TITULOS;

	//ArbolBMasNumerico* arbolP = new ArbolBMasNumerico(pathArbol,TAMANIO_BLOQUE_BMAS);
	//arbolP->exportar(pathExport);
	//delete arbolP;
	//return 0;
	Hash * hash_titulos=new Hash(pathHash);

	hash_titulos->mostrar2(pathExport);
	delete hash_titulos;

	return 0;
}

int Servicios::verEstructuraPalabras(string path)
{
	string pathExport;
	if (path == "")
	{
		pathExport = Parametros().getParametro(CARPETA_SALIDA);
		pathExport += NOMBRE_HASH_PALABRAS;
	}
	else
	{
		pathExport = path;
	}
	string pathHash = Parametros().getParametro(CARPETA_DATOS);
	pathHash += NOMBRE_HASH_PALABRAS;

	Hash * hash_palabras=new Hash(pathHash);

	hash_palabras->mostrar2(pathExport);
	delete hash_palabras;

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
		int error = recuperarLibro((*it), &libro);
		cout <<"Autor: "<< libro->getAutor() <<endl;
		cout <<"Editorial: "<< libro->getEditorial() <<endl;
		cout <<"Titulo: "<< libro->getTitulo() <<endl;
		cout <<"Id: "<< libro->getId() <<endl;
		cout <<"Cant.palabras: " << libro->getCantPalabras() << endl;
		cout << "Presione ENTER para continuar...." << endl;
		cin.get();

		switch(indice)
		{
			case INDICE_AUTORES :error = agregarIndiceAutores(libro);break;
			case INDICE_EDITORIALES:error = agregarIndiceEditoriales(libro);break;
			case INDICE_TITULOS:error = agregarIndiceTitulos(libro);break;
			case INDICE_PALABRAS:error = agregarIndicePalabras(libro);break;
		}

		delete libro;
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
	agregarAlArbol(NOMBRE_BMAS_EDITORIALES, unLibro->getEditorial(), unLibro->getId());

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
	set<string>::iterator it = palabras->begin();

	for ( ; it != palabras->end(); it++)
	{
		cout << "PALABRA A INDEXAR =" << *it <<endl;
		agregarAlHash(NOMBRE_HASH_PALABRAS, *it ,unLibro->getId());
	}

	delete palabras;
	return 0;
}

void Servicios::agregarAlHash(string nombreHash, string clavePasada, unsigned int idNueva)
{
	string pathHash = Parametros().getParametro(CARPETA_DATOS);
	pathHash += nombreHash;
	Hash *hash  = new Hash(pathHash);

	Registro* registro= hash->buscar(clavePasada);
	unsigned int offset;

	//si ya existe la lista//
	if (registro){
		//cout << "lista ya existente" << endl;
		offset = registro->getAtributosEnteros()->front();
		ListasIds().agregarIdDeLibro(&offset,idNueva,false);
	}
	//no existe la lista de ids
	else{
		//cout << "lista nueva" << endl;
		ListasIds().agregarIdDeLibro(&offset, idNueva,true);
		registro = new Registro();
		registro->setString(clavePasada);
		registro->agregarAtribEntero(offset);
		hash->insertar(registro);
	}

	delete registro;
	delete hash;
}

void Servicios::agregarAlArbol(string nombreArbol, string clavePasada, unsigned int idNueva)
{
	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += nombreArbol;
	ArbolBMasAlfabetico *arbol  = new ArbolBMasAlfabetico(pathArbol, TAMANIO_BLOQUE_BMAS);

	resultadoOperacion resultado(OK);

	Registro* registro= arbol->buscarRegistro(clavePasada, &resultado);
	unsigned int offset;

	if (registro && resultado.getDescripcion() == "ENCONTRADO")
	{
		//lista ya existe
		offset = registro->getReferenciai(1);
		ListasIds().agregarIdDeLibro(&offset,idNueva,false);
	}
	else{
		//debo crear la lista nueva
		ListasIds().agregarIdDeLibro(&offset, idNueva,true);
		arbol->insertar(clavePasada,offset);
	}
	delete arbol;

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
		bool listaVacia = ListasIds().sacarIdDelLibro(&offset,idLibro);
		if (listaVacia == LISTA_VACIA){
			hash->borrar(clave);
		}
	}
	delete hash;
}


void Servicios::sacarDelArbol(string nombreArbol, string clave, unsigned int idLibro)
{

	string pathArbol = Parametros().getParametro(CARPETA_DATOS);
	pathArbol += nombreArbol;
	ArbolBMasAlfabetico *arbol  = new ArbolBMasAlfabetico(pathArbol, TAMANIO_BLOQUE_BMAS);
	resultadoOperacion resultado(OK) ;

	Registro* registro= arbol->buscarRegistro(clave, &resultado);
	unsigned int offset;


	if (registro && resultado.getDescripcion() == "ENCONTRADO")
	{
		offset = registro->getReferenciai(1);

		if (ListasIds().sacarIdDelLibro(&offset,idLibro) == LISTA_VACIA){
			resultadoOperacion resultado = arbol->eliminar(clave);
		}
	}

	delete arbol;

}


int Servicios::recuperarLibro(unsigned int idLibro, Libro **libro)
{
	unsigned int offset = 0;

	string pathArbolPrimario = Parametros().getParametro(CARPETA_DATOS);
	pathArbolPrimario += NOMBRE_BMAS_PRIMARIO;

	ArbolBMasNumerico *arbolP = new ArbolBMasNumerico(pathArbolPrimario, TAMANIO_BLOQUE_BMAS_NUMERICO);

	resultadoOperacion resultado(OK) ;

	Registro* registro = arbolP->buscarRegistroNumerico(idLibro, &resultado);

	//TODO ver si esta recuperando bien el offset
	if (registro && resultado.getDescripcion() == "ENCONTRADO"){
		offset = registro->getReferencias()->front();
	}
	else{
		return ERROR_LIBRO_INEXISTENTE;
	}


	string rutaArcLibros = Parametros().getParametro(ARCHIVO_LIBROS);

	if (rutaArcLibros == "") return ERROR_RUTA_ARCHIVO_LIBROS;

	ArchivoLibros *archivo = new ArchivoLibros(rutaArcLibros);

	*libro = (archivo->recuperarLibro(offset));

	delete archivo;

	delete arbolP;

	return 0;
}

int Servicios::agregarVarios(string cuantos)
{
	int corte = atoi(cuantos.c_str());
	string rutaLibros = Parametros().getParametro(CARPETA_LIBROS);
	list<string> *archivos = Util().getArchivos(rutaLibros);
	int cont = 0;

	for (list<string>::iterator it = archivos->begin(); it != archivos->end(); it++)
	{
		cont++;
		string pathAbs = rutaLibros + *it;
		cout << "Agregando Texto: " << *it << "..." <<endl;
		Servicios().tomarTexto(pathAbs);

		if (cont >= corte) break;
	}

	return 0;

}
