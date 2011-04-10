/*
 * Hash.cpp
 *
 *  Created on: 09/04/2011
 *      Author: pablo
 */

//TODO revisar como devolver los errores
//TODO conexion con manejador de archivos

#include "Hash.h"

Hash::Hash(string nombre)
{
	this->pathTabla = Parametros().getParametro(CARPETA_DATOS);
	this->pathDatos = pathTabla;
	this->pathTabla += nombre + EXTENCION_TABLA;
	this->pathDatos += nombre + EXTENCION_DATOS;

	if (Util().existeArchivo(this->pathTabla))
	{
		this->abrir();
	}else{
		this->crear();
		this->abrir();
	}
}

Hash::~Hash()
{
	delete [] tabla;
}

int Hash::crear()
{

	fstream archivo;
	archivo.open(this->pathTabla.c_str(), ios::out | ios::binary);

	if (archivo.is_open())
	{
		elemLista tamanio = 0;
		char *buff = new char[sizeof(elemLista)];
		memcpy(buff,&tamanio,sizeof(elemLista));
		archivo.write(buff,sizeof(elemLista));
		archivo.close();
		delete []buff;
	}


	//llamada a la creacion de un nuevo archivo de bloques
	ArchivoBloques *archivoBloque = new ArchivoBloques(this->pathDatos,TAMANIO_BLOQUE);
	Bloque *bloque0 = new Bloque();

	try{
		archivoBloque->grabarBloque(bloque0,0);
		//bloques = true;
	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}


	archivoBloque->~ArchivoBloques();
	bloque0->~Bloque();

}


int Hash::abrir()
{
	fstream archivo;
	archivo.open(this->pathTabla.c_str(), ios::in | ios::binary);
	if (archivo.is_open())
	{
		char *buff = new char[sizeof(elemLista)];
		archivo.read(buff,sizeof(elemLista));
		memcpy(&tamanioLista, buff, sizeof(elemLista));
		delete [] buff;

		//si tiene elementos
		if (tamanioLista > 0)
		{
			unsigned int bytes = tamanioLista*sizeof(elemLista);
			buff = new char[bytes];
			this->tabla = new elemLista[tamanioLista];
			archivo.read(buff,bytes);
			delete [] buff;
			archivo.close();

			int count = 0;
			while (bytes > 0)
			{
				memcpy(&tabla[count],buff, sizeof(elemLista));
				bytes -= sizeof(elemLista);
				count ++;
			}
		}
		return 0;

	}else
		return 2; //error al quere abrir el hash
}

void Hash::insertar(Registro *registro)
{

	Registro *registroOld = this->buscar(registro->getString());

	if (!registro)
	{
		elemLista nroElem = this->hasheo(registro->getString());
		ArchivoBloques *archivo = new ArchivoBloques(this->pathDatos, TAMANIO_BLOQUE);
		Bloque *bloque = archivo->recuperarBloque(this->tabla[nroElem]);
		try{
			bloque->agregarRegistro(*registro);
			//ok
		}catch (ExceptionBloque &e)
		{
			//Bloque *bloqueNuevo = new Bloque();
			//archivo->grabarBloque(bloqueNuevo,1);
			//ver aca por que el archivo de bloques deveria manejar bloques vacios
			//osea que tendria que darme el bloque nuevo con el nro de bloque asignado

		}
	}else{

	}


	//si no esta
		//elemLista nroBloque = this->hasheo(registro->que);
		//obtener bloque del archivo de bloques
		//agregar el registro al final
		//grabar bloque en el archivo de bloques
		//si se grabo
			//devolver ok
		//sino se grabo por exceso del tamaño del bloque
			//si TD < tamanio de la tabla
				//pedir al archivo de bloques un bloque nuevo
				//buscar la mitad de las referencias de bloque viejo y apuntarlas al nuevo
				//duplicar el tamaño de TD del bloque viejo
				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo
				//reintentar la insersion de registro
			//si TD > tamanio de la tabla
				//pedir al archivo de bloques un bloque nuevo
				//duplicar el tamaño de la tabla espejando los elementos
				//apuntar el elemento original al bloque nuevo
				//duplicar el TD del bloque viejo
				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo
				//reintentar la insersion de registro
		//fin si no se grabo
	//si esta
	//devolver error



	//logica de insersion

	//llamada al archivo de bloques

}

void Hash::borrar(string que)
{
	//this->buscar(registro->que);
	//analizar si esta o no

	//si esta
		//reestructurar el bloque borrando (usar this->offsetUltimaBusqueda para ubicar el registro)
		//si tiene mas registros
			//guardar el bloque
			//retornar OK
		//si no tiene mas registros
			//se busca un elemento con distancia (TD/2) para adelante y (TD/2) para atras osea equidistante
			//si el elemento apunta al mismo bloque
				//los elementos que apuntaban al bloque viejo apuntan al bloque que apuntan los equidistantes
				//el TD del bloque divide a la mitad
				//se informa al archivo de bloques que elimine el bloque

			//no en el mismo bloque (adelante != atras)
				//no se hace nada
}

Registro *Hash::buscar(string que)
{

	//logica de busqueda
	int nroElemento = this->hasheo(que);
	if (nroElemento < 0)
		return NULL;

	elemLista nroBloque = this->tabla[nroElemento];

	ArchivoBloques *archivoBloq = new ArchivoBloques(this->pathDatos, TAMANIO_BLOQUE);

	Bloque *bloque = archivoBloq->recuperarBloque(nroBloque);


	Registro *registro = bloque->recuperarRegistro(que);
	bloque->~Bloque();
	archivoBloq->~ArchivoBloques();

	return registro;

}

int Hash::hasheo(string key)
{

	elemLista keyNumerica=0;

	if (key.length() > 0)
	{
		keyNumerica += (elemLista)key[0];
		if (key.length() > 1)
			keyNumerica += (elemLista)(elemLista)key[1];
			if (key.length() > 2)
				keyNumerica += (elemLista)(elemLista)key[2];
	}else
		return -1;

	return (keyNumerica % (this->tamanioLista));
}
