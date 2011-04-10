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

	return 0;
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
			unsigned int bytes = tamanioLista*sizeof(elemLista2);
			buff = new char[bytes];
			this->tabla = new elemLista2[tamanioLista];
			archivo.read(buff,bytes);
			archivo.close();

			int count = 0;
			while (bytes > 0)
			{
				memcpy(&tabla[count],buff, sizeof(elemLista2));
				bytes -= sizeof(elemLista2);
				count ++;
			}
			delete [] buff;
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
		Bloque *bloque = archivo->recuperarBloque(this->tabla[nroElem].nroBloque);
		try{
			bloque->agregarRegistro(*registro);
			//ok
		}catch (ExceptionBloque &e)
		{
			elemLista nroNuevoBloque;
			//pedir nuevo bloque
			//ver aca por que el archivo de bloques deveria manejar bloques vacios
			//osea que tendria que darme el bloque nuevo con el nro de bloque asignado

			if (this->tabla[nroElem].TD < this->tamanioLista)
			{
				//pedir al archivo de bloques un bloque nuevo
				//buscar la mitad de las referencias de bloque viejo y apuntarlas al nuevo
				//duplicar el tamaño de TD del bloque viejo
				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo
				//reintentar la insersion de registro


				unsigned int aBuscar = (this->tamanioLista/this->tabla[nroElem].TD)/2;
				unsigned int i = this->tamanioLista-1;
				while (i >= 0)
				{
					if (this->tabla[i].nroBloque == this->tabla[nroElem].nroBloque )
					{
						this->tabla[i].TD = this->tabla[nroElem].TD*2;
						if (aBuscar > 0)
						{
							this->tabla[i].nroBloque = nroNuevoBloque;
							aBuscar--;
						}
					}
				}

				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo
				//reintentar la insersion de registro

			}else{

				//duplicar el tamaño de la tabla espejando los elementos
				//apuntar el elemento original al bloque nuevo
				//duplicar el TD del bloque viejo
				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo
				//reintentar la insersion de registro
			}
		}

	}


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

	elemLista nroBloque = this->tabla[nroElemento].nroBloque;

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
