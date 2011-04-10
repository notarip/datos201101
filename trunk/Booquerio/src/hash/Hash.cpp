/*
 * Hash.cpp
 *
 *  Created on: 09/04/2011
 *      Author: pablo
 */

//TODO revisar como devolver los errores
//TODO conexion con manejador de archivos

#include "Hash.h"

Hash::Hash()
{
	// TODO Auto-generated constructor stub
}

Hash::~Hash()
{
	delete [] tabla;
}

int Hash::crear(string nombre)
{
	//crear el archivo de la tabla de dispersion
	string pathTabla = Parametros().getParametro(CARPETA_DATOS);
	pathTabla += nombre + EXTENCION_TABLA;

	if (Util().existeArchivo(pathTabla))
		return 1; //error de creacion

	fstream archivo;
	archivo.open(pathTabla.c_str(), ios::out | ios::binary);

	if (archivo.is_open())
	{
		elemLista tamanio = 0;
		char *buff = new char[sizeof(elemLista)];
		memcpy(buff,&tamanio,sizeof(elemLista));
		archivo.write(buff,sizeof(elemLista));
		archivo.close();
		delete []buff;
		return 0; //creacion correcta
	}
		return 1; //error al crear

	//llamada a la creacion de un nuevo archivo de bloques
}


int Hash::abrir(string nombre)
{
	//cargar el achivo de la tabla de dispersion en memoria
	string carpeta  = Parametros().getParametro(CARPETA_DATOS);
	string arcTabla = carpeta + nombre + EXTENCION_TABLA;

	if (!Util().existeArchivo(arcTabla.c_str()))
		return 2; //error al quere abrir el hash

	fstream archivo;
	archivo.open(arcTabla.c_str(), ios::in | ios::binary);
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

void Hash::insertar(void *registro)
{
	//buscar en el hash
	//this->buscar(registro->que);
	//analizar si esta o no


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

int Hash::buscar(string que)
{

	//logica de busqueda
	int nroElemento = this->hasheo(que);
	if (nroElemento < 0)
		return -1;

	elemLista nroBloque = this->tabla[nroElemento];

	//llamda al archivo de bloques con nroBloque

	//busqueda del registro dentro del bloque
	//si lo encuentra
		//asignar a this->offsetUltimaBusqueda en que byte empieza dentro del bloque
	//si no lo encuentra asignar a this->offsetUltimaBusqueda -1

	return 0;

}

int Hash::hasheo(string key)
{

	elemLista keyNumerica;
	elemLista nroElemento;
	elemLista nroBloque;

	if (key.length() > 0)
	{
		keyNumerica += (elemLista)(elemLista)key[0];
		if (key.length() > 1)
			keyNumerica += (elemLista)(elemLista)key[1];
			if (key.length() > 2)
				keyNumerica += (elemLista)(elemLista)key[2];
	}else
		return -1;

	return (keyNumerica % (this->tamanioLista));
}




