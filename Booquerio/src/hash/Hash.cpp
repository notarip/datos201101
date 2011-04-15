/*
 * Hash.cpp
 *
 *  Created on: 09/04/2011
 *      Author: pablo
 */

//TODO revisar como devolver los errores
//TODO implementar borrar
//TODO agregar metodo para persistir la tabla teniendo en cuenta si viene
//de varios registros
//TODO en crear pasar la creacion del 1er bloque de datos a la 1er insercion
//TODO Hernan dejame el insertar que termino de tocarlo yo

#include "Hash.h"

Hash::Hash(string nombre)
{
	this->pathHash = Parametros().getParametro(CARPETA_DATOS);
	this->pathHash += nombre + EXTENCION_DATOS;

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

	/*
	 * Se crea el registro para la lista de bloques
	 * y se crea el bloque que va a contener la lista
	 */

	ArchivoBloques manejadorTabla(this->pathHash,TAMANIO_BLOQUE_TABLA);
	ArchivoBloques manejadorDatos(this->pathHash,TAMANIO_BLOQUE);

	Registro *regTabla = new Registro();

	for (int i = 0; i < TAMANIO_TABLA; i++)
		regTabla->agregarAtribEntero(0);



	Bloque *bloqueTabla = new Bloque();
	bloqueTabla->agregarRegistro(*regTabla);




	try{
		manejadorTabla.grabarBloque(bloqueTabla,0);
	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}

	regTabla->~Registro();
	bloqueTabla->~Bloque();


	/*
	 * Se crea el primer bloque de datos vacio
	 * TODO: este se deberia crear al primer insert
	 */
	Bloque *bloque1 = new Bloque();
	unsigned int libre1= manejadorDatos.getBloqueLibre();

	try{
		manejadorDatos.grabarBloque(bloque1,libre1);
	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}

	bloque1->~Bloque();

	return 0;
}


int Hash::abrir(){

		bool hayMasElementos = true;
		ArchivoBloques archivoLista(this->pathHash,TAMANIO_BLOQUE_TABLA);

		Bloque* bloqueTabla = archivoLista.recuperarBloque(0);
		Registro* regTabla = bloqueTabla->recuperarRegistro(0);

		list<unsigned int> *listaDeBloques = regTabla->getAtributosEnteros();

		while (hayMasElementos)
		{
			if (regTabla->getReferencias()->size() > 0)
			{
				unsigned int bloque = regTabla->getReferencias()->front();
				bloqueTabla = archivoLista.recuperarBloque(bloque);
				regTabla = bloqueTabla->recuperarRegistro(0);
				listaDeBloques->assign(regTabla->getAtributosEnteros()->begin(), regTabla->getAtributosEnteros()->end());
			}else
				hayMasElementos = false;
		}
		this->tamanioLista = listaDeBloques->size();
		this->tabla = new unsigned int[this->tamanioLista];

		unsigned int i = 0;
		for (list<unsigned int>::iterator it = listaDeBloques->begin(); it != listaDeBloques->end(); it++ )
		{
			this->tabla[i] = (*it);
			i++;
		}


		bloqueTabla->~Bloque();
		regTabla->~Registro();

		return 0;
}

void Hash::insertar(Registro *registro)
{

	Registro *registroOld = this->buscar(registro->getString());

	if (!registroOld)
	{
		elemLista nroElem = this->hasheo(registro->getString());
		ArchivoBloques archivo(this->pathHash, TAMANIO_BLOQUE);
		unsigned int nroBloque = this->tabla[nroElem];
		Bloque *bloque = archivo.recuperarBloque(nroBloque);
		try{
			bloque->agregarRegistro(*registro);
			//ok
		}catch (ExceptionBloque &e)
		{
			elemLista nroNuevoBloque = archivo.getBloqueLibre();
			Bloque *nuevoBloque = archivo.recuperarBloque(nroNuevoBloque);

			if (bloque->getAtributoBloque() < this->tamanioLista)
			{
				//buscar la mitad de las referencias de bloque viejo y apuntarlas al nuevo -ok
				//duplicar el tamaño de TD del bloque viejo -ok
				//agarrar todos los registros del bloque viejo y dispersarlos de nuevo -ok
				//reintentar la insersion de registro -ok

				bloque->setAtributoBloque(bloque->getAtributoBloque()*2);
				nuevoBloque->setAtributoBloque(bloque->getAtributoBloque());

				unsigned int aBuscar = (this->tamanioLista/bloque->getAtributoBloque())/2;
				unsigned int i = this->tamanioLista-1;
				while (i >= 0)
				{
					if (this->tabla[i] == nroBloque )
					{
						if (aBuscar > 0)
						{
							this->tabla[i] = nroNuevoBloque;
							aBuscar--;
						}
					}
				}

			}else{

				bloque->setAtributoBloque(bloque->getAtributoBloque()*2); //duplica el TD del bloque viejo
				nuevoBloque->setAtributoBloque(bloque->getAtributoBloque());

				//duplica el tamaño de la tabla
				elemLista *nuevaTabla = new elemLista[this->tamanioLista*2];

				// espejo los elementos
				for (unsigned int j = 0; j < this->tamanioLista;j++)
				{
					nuevaTabla[j] = this->tabla[j];
					nuevaTabla[this->tamanioLista + j] = this->tabla[j];
				}

				//apunta el elemento viejo al bloque nuevo
				this->tabla[nroElem] = nroNuevoBloque;

				delete [] this->tabla;
				this->tabla = nuevaTabla;
			}

			//TODO metodo que persista la tabla

			//agarra todos los registros del bloque viejo incluso el registro nuevo y los dispersa de nuevo
			list<Registro> *lista =  bloque->obtenerRegistros();
			archivo.grabarBloque(new Bloque(),nroBloque); //graba un bloque vacio en lugar el viejo
			for (list<Registro>::iterator it = lista->begin(); it != lista->end(); it++)
			{
				this->insertar(&(*it));
			}
			bloque->~Bloque();
			nuevoBloque->~Bloque();
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

	elemLista nroBloque = this->tabla[nroElemento];

	ArchivoBloques *archivoBloq = new ArchivoBloques(this->pathHash, TAMANIO_BLOQUE);

	Bloque *bloque = archivoBloq->recuperarBloque(nroBloque);

	Registro *registro = bloque->recuperarRegistro(que);

	bloque->~Bloque();
	archivoBloq->~ArchivoBloques();

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
