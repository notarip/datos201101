/*
 * Hash.cpp
 *
 *  Created on: 09/04/2011
 *      Author: pablo
 */

//TODO revisar como devolver los errores
//TODO implementar borrar

#include "Hash.h"

Hash::Hash(string nombre)
{
	this->pathHash = Parametros().getParametro(CARPETA_DATOS);

	if (Util().existeArchivo(this->pathHash))
		this->abrir();

	else {

		this->crear();
		this->abrir();
	}
}

Hash::~Hash()
{
	delete [] tabla;
}

int Hash::crear(){

	ArchivoBloques archivo(this->pathHash,TAMANIO_BLOQUE);

	if (archivo.is_open())
	{
		Registro regTabla(0,0);
		Bloque bloqueTabla(regTabla);

		try{
			archivo.grabarBloque(bloqueTabla,0);
		}catch (ExceptionBloque &e)
		{
			cout << e.what() << endl;
		}

	}



	Bloque *bloque1 = new Bloque();
	unsigned int libre1= archivo.getBloqueLibre();

	try{
		archivo.grabarBloque(bloque1,libre1);
	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}


	return 0;
}


int Hash::abrir(){  //TODO nose exactamente q es lo que haces, le saque el is_open ese
					// porque el archivo se abre si no existe cuando creas el objeto.
					//
		ArchivoBloques archivoHash(this->pathHash,TAMANIO_BLOQUE);
		Bloque* bloqueTabla = archivoHash.recuperarBloque(0);
		Registro* regTabla = (bloqueTabla)->recuperarRegistro(0);
		unsigned int tamanioLista = regTabla->getIdentificadores().size();

		if (tamanioLista > 0)
		{
		List<unsigned int>* tabla= regTabla->getIdentificadores();
		return 0;
	}
	else
		return 2; //error al quere abrir el hash
}

void Hash::insertar(Registro *registro)
{

	Registro *registroOld = this->buscar(registro->getString());

	if (!registro)
	{
		elemLista nroElem = this->hasheo(registro->getString());
		ArchivoBloques *archivo = new ArchivoBloques(this->pathDatos, TAMANIO_BLOQUE);
		unsigned int nroBloque = this->tabla[nroElem].nroBloque;
		Bloque *bloque = archivo->recuperarBloque(nroBloque);
		try{
			bloque->agregarRegistro(*registro);
			//ok
		}catch (ExceptionBloque &e)
		{
			elemLista nroNuevoBloque; //TODO asignarle el nro del bloque nuevo
			//TODO pedir nuevo bloque
			//TODO ver aca por que el archivo de bloques deberia manejar bloques vacios
			//TODO osea que tendria que darme el bloque nuevo con el nro de bloque asignado

			if (this->tabla[nroElem].TD < this->tamanioLista)
			{
				//buscar la mitad de las referencias de bloque viejo y apuntarlas al nuevo
				//duplicar el tamaño de TD del bloque viejo
				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo
				//reintentar la insersion de registro


				unsigned int aBuscar = (this->tamanioLista/this->tabla[nroElem].TD)/2;
				unsigned int i = this->tamanioLista-1;
				while (i >= 0)
				{
					if (this->tabla[i].nroBloque == nroBloque )
					{
						this->tabla[i].TD = this->tabla[nroElem].TD*2;
						if (aBuscar > 0)
						{
							this->tabla[i].nroBloque = nroNuevoBloque;
							aBuscar--;
						}
					}
				}

			}else{
				//duplica el TD del bloque viejo
				//hago esto aca para que el elemento nuevo ya quede con el TD duplicado
				//todavia no apunto al bloque nuevo para que el elemento espejado apunte al bloque viejo
				this->tabla[nroElem].TD *= 2;

				//duplica el tamaño de la tabla
				elemLista2 *nuevaTabla = new elemLista2[this->tamanioLista*2];

				// espejo los elementos
				for (unsigned int j = 0; j < this->tamanioLista;j++)
				{
					nuevaTabla[j] = this->tabla[j];
					nuevaTabla[this->tamanioLista + j] = this->tabla[j];
				}

				//apunta el elemento viejo al bloque nuevo
				this->tabla[nroElem].nroBloque = nroNuevoBloque;

				delete [] this->tabla;
				this->tabla = nuevaTabla;

			}

			//agarra todos los registros del bloque viejo y los dispersa de nuevo
			//reintenta la insersion de registro
			list<Registro> *lista =  bloque->obtenerRegistros();
			archivo->grabarBloque(new Bloque(),nroBloque); //grabo vacio el viejo
			for (list<Registro>::iterator it = lista->begin(); it != lista->end(); it++)
			{
				this->insertar(&(*it));
			}
			bloque->~Bloque();
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

	delete bloque;
	delete archivoBloq;

	return registro;

}

int Hash::hasheo(string key)
{

	elemLista keyNumerica=0;
	unsigned int i=0;
	while(i<key.length() && i< 3){
		keyNumerica += (elemLista)key[i];
		i++;
		if (i<3) keyNumerica= keyNumerica<<8;
	}
	if (key.length() > 0)
		return (keyNumerica % (this->tamanioLista));
	return -1;
}
