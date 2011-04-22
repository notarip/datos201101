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
	this->pathHash += nombre + EXTENSION_DATOS;

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

	ArchivoBloques manejadorHash(this->pathHash,TAMANIO_BLOQUE);

	Registro *regTabla = new Registro();
	regTabla->agregarAtribEntero(0);

	Bloque *bloqueTabla = new Bloque();
	bloqueTabla->agregarRegistro(*regTabla);




	try{
		manejadorHash.grabarBloque(bloqueTabla,0);
	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}

	//regTabla->~Registro();  no es necesario.
	//bloqueTabla->~Bloque(); el mismo bloque llama a un destructor.

	delete bloqueTabla;

	/*
	 * Se crea el primer bloque de datos vacio
	 * TODO: este se deberia crear al primer insert
	 */

	Bloque *bloqueUno = new Bloque();
	unsigned int posLibreUno = manejadorHash.getBloqueLibre();

	try{
		manejadorHash.grabarBloque(bloqueUno,posLibreUno);
	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}

	delete bloqueUno;

	return 0;
}



int Hash::abrir(){

		ArchivoBloques archivoLista(this->pathHash,TAMANIO_BLOQUE_TABLA);

		Bloque* bloqueTabla = archivoLista.recuperarBloque(0);
		Registro* regTabla = bloqueTabla->recuperarRegistro(0);

		list<unsigned int> *listaDeBloques = regTabla->getAtributosEnteros();

		list<unsigned int> *referencia = regTabla->getReferencias();

		while (regTabla->getReferencias()!= NULL)
		{

			unsigned int bloqueSiguiente = regTabla->getReferencias()->front();
			bloqueTabla = archivoLista.recuperarBloque(bloqueSiguiente);
			regTabla = bloqueTabla->recuperarRegistro(0);
			listaDeBloques->assign(regTabla->getAtributosEnteros()->begin(), regTabla->getAtributosEnteros()->end());
		}

		this->tamanioTabla = listaDeBloques->size();

		this->tabla = new unsigned int[this->tamanioTabla];

		unsigned int i = 0;

		/*funciona esto? -ver despues */
		for (list<unsigned int>::iterator it = listaDeBloques->begin(); it != listaDeBloques->end(); it++ )
		{
			this->tabla[i] = (*it);
			i++;
		}

		return 0;
}




void Hash::insertar(Registro *registro)
{

	//VALIDACION DE UNICIDAD //
	Registro *registroOld = this->buscar(registro->getString());

	if (!registroOld)
	{
		elemLista posTabla = this->hasheo(registro->getString());

		ArchivoBloques archivo(this->pathHash, TAMANIO_BLOQUE);

		unsigned int nroBloque = this->tabla[posTabla]; //hacerlo mejor esto despues//

		Bloque *bloque = archivo.recuperarBloque(nroBloque);

		try{
			bloque->agregarRegistro(*registro);

		}
		catch (ExceptionBloque &e)
		{

			unsigned int tamanioDispersion= this->tamDispersion(nroBloque);

			//obtiene un bloque libre, si habia uno de antes te devuelve un nro de bloque que ya existia, sino un nuevo//

			unsigned int primerBloqueLibre= archivo.getBloqueLibre();

			unsigned int pos_Desbordado = posTabla;


			//escribe un bloque vacio en la posicion libre que encontramos. Si era una vieja pisa toda esa info con espacio en blanco.//
			//No es necesario tratar aparte los bloques vacios, porque de eso se encarga el manejador de arch de bloques.//

			this->actualizarTabla_insercion(tamanioDispersion,pos_Desbordado,primerBloqueLibre);

			unsigned int bloque_desbordado = this->tabla[pos_Desbordado];

			this->reestructurar_archivo(archivo,bloque_desbordado,primerBloqueLibre,registro);

		}
	}
	cout<<"registro duplicado."<<endl;
}




void Hash::borrar(string que)
{
	//this->buscar(registro->que);
	//analizar si esta o no
	Registro *registroOld = this->buscar(que);

	//si esta
	if (registroOld!=NULL){

		ArchivoBloques archivo(this->pathHash, TAMANIO_BLOQUE);

		unsigned int posTabla= this->hasheo(registroOld->getString());

		unsigned int numBloque= this->tabla[posTabla];

		Bloque* bloqueRecuperado= archivo.recuperarBloque(numBloque);

		/*intento eliminar*/
		list<Registro>::iterator itRegistros= bloqueRecuperado->obtenerRegistros()->begin();

		while(itRegistros->getString()!=registroOld->getString())	itRegistros++;

		list<Registro>* listaRegistros= bloqueRecuperado->obtenerRegistros();

		listaRegistros->erase(itRegistros);
		/*fin intento eliminar*/

		if (listaRegistros->empty()){

			unsigned int tamTablaVieja= this->tamanioTabla;

			unsigned int tamDispersionBloque= this->tamDispersion(numBloque);

			actualizarTabla_eliminacion(tamDispersionBloque, posTabla);

			if (tamTablaVieja > this->tamanioTabla) archivo.eliminarBloque(numBloque);

		}

		else archivo.grabarBloque(bloqueRecuperado,numBloque);

	}
	cout<<"el registro no se encontro."<<endl;
}




Registro* Hash::buscar(string que){

	//logica de busqueda
	int nroElemento = this->hasheo(que);

	if (nroElemento < 0)	return NULL;

	elemLista nroBloque = this->tabla[nroElemento];

	ArchivoBloques archivoBloq = ArchivoBloques(this->pathHash, TAMANIO_BLOQUE);

	Bloque *bloque = archivoBloq.recuperarBloque(nroBloque);

	Registro *registro = bloque->recuperarRegistro(que);

	return registro;

}





//probar funcion de dispersion, (que sea buena)//

unsigned int Hash::hasheo(string key){
	int i=0;
	unsigned int parcial=0;
	unsigned int final=0;
	while(i<4){
		parcial=0;
		parcial=(unsigned int)key[i];
		final+=parcial;
		if (i<3) final=final<<8;
		i++;
	}
	final%=this->tamanioTabla;
	return final;
}




void Hash::reestructurar_archivo(ArchivoBloques archivo,unsigned int nro_desbordado,unsigned int nro_libre, Registro* registro_a_ins){
	// posicion del bucket desbordado //
	Bloque* bloque_desbordado =archivo.recuperarBloque(nro_desbordado);
	Bloque* bloque_libre =new Bloque();


	list<Registro>* lista_desbordado = bloque_desbordado->obtenerRegistros();
	list<Registro>* lista_libres = bloque_desbordado->obtenerRegistros();
	list<Registro>::iterator iteradorReg_desbordado = bloque_desbordado->obtenerRegistros()->begin();
	list<Registro>::iterator iteradorReg_libre = bloque_libre->obtenerRegistros()->begin();

	while( iteradorReg_desbordado !=  bloque_desbordado->obtenerRegistros()->end() ){

		unsigned int posTabla= this->hasheo(iteradorReg_desbordado->getString());

		if (nro_libre == this->tabla[posTabla]){
			Registro reg_a_mover= *iteradorReg_desbordado;
			lista_libres->insert(iteradorReg_libre,reg_a_mover);
			lista_desbordado->erase(iteradorReg_desbordado);

			iteradorReg_libre= lista_libres->end();
		}
		else iteradorReg_desbordado++;
	}
	archivo.grabarBloque(bloque_desbordado,nro_desbordado);
	archivo.grabarBloque(bloque_libre,nro_libre);

}




unsigned int Hash::tamDispersion(unsigned int x){
	unsigned int cont=0;
	for (unsigned int i=0; i<this->tamanioTabla ;i++){
		if (this->tabla[i]==x) cont++;
	}
	cont=this->tamanioTabla/cont;
	return cont;
}




void Hash::actualizarTabla_insercion(unsigned int tamDispersion,unsigned int pos_desbordado,unsigned int bl_nuevo){
	if (tamDispersion==tamanioTabla) {
		unsigned int* viejaTabla= this->tabla;
		this->tabla= new unsigned int[tamanioTabla*2];
		memcpy(tabla,viejaTabla,this->tamanioTabla*sizeof(unsigned int));
		this->tabla=tabla+4;
		memcpy(tabla,viejaTabla,this->tamanioTabla*sizeof(unsigned int));
		this->tabla=tabla-4;
		this->tamanioTabla*=2;
		delete viejaTabla;
	}
	tabla[pos_desbordado]=bl_nuevo;
}





void Hash::actualizarTabla_eliminacion(unsigned int tamDispersion,unsigned int pos_eliminado){
	int tamTabla=tamanioTabla;
	int salto1= pos_eliminado +tamDispersion/2 ;
	int salto2= pos_eliminado -tamDispersion/2 ;
	if (salto1 >= tamTabla) salto1=salto1-tamanioTabla;
	else if(salto1 < 0) salto1=salto1+tamanioTabla;
	if (salto2 >= tamTabla) salto2=salto2-tamanioTabla;
	else if (salto2 < 0) salto2=salto2+tamanioTabla;
		bool sonIguales= ( tabla[salto1] == tabla[salto2] );

	int pos1=0;
	int pos2=0;
	int comparaciones=0;

	if (sonIguales) {
		this->tabla[pos_eliminado]=this->tabla[salto1];
		pos2=tamanioTabla/2;
	}

	while(sonIguales && (comparaciones <(tamanioTabla/2)) ){
		sonIguales= (this->tabla[pos1]==this->tabla[pos2]);
		pos1++;
		pos2++;
		comparaciones++;
	}

	if (sonIguales){
		unsigned int* viejaTabla= tabla;
		this->tabla= new unsigned int[this->tamanioTabla/2];
		memcpy(this->tabla,viejaTabla,(this->tamanioTabla/2)*sizeof(unsigned int));
		this->tamanioTabla/=2;
		delete viejaTabla;
	}
}

