/*
 * Hash.cpp
 *
 *  Created on: 29/04/2011
 *      Author: hernan
 */


#include "Hash.h"
#include "math.h"

Hash::Hash(string nombre)
{

	this->pathHash = nombre + ".hash";

	if (Util().existeArchivo(this->pathHash))
		this->abrir();

	else {
		this->crear();
		this->abrir();
	}
}


Hash::~Hash()
{

	this->guardarTabla();

}


int Hash::crear(){

	/*
	 * Se crea el registro para la lista de bloques
	 * y se crea el bloque que va a contener la lista
	 */

	ArchivoBloques manejadorHash(this->pathHash,TAMANIO_BLOQUE);

	Registro regTabla;

	regTabla.agregarAtribEntero(1);
	regTabla.agregarReferencia(0);

	Bloque *bloqueTabla = new Bloque();
	bloqueTabla->agregarRegistro(regTabla);

	try{
		manejadorHash.grabarBloque(bloqueTabla,0);

	}catch (ExceptionBloque &e)
	{
		cout << e.what() << endl;
	}

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

	ArchivoBloques archivoLista(this->pathHash,TAMANIO_BLOQUE);

	unsigned int bloqueSiguiente= 0;

	Bloque* bloqueTabla = archivoLista.recuperarBloque(bloqueSiguiente);

	Registro regTabla = bloqueTabla->obtenerRegistros()->front();

	list<unsigned int> listaDeBloques;

	list<unsigned int>::iterator itBloques= regTabla.getAtributosEnteros()->begin();
	for (itBloques;itBloques!= regTabla.getAtributosEnteros()->end();itBloques++) listaDeBloques.push_back(*itBloques);

	bloqueSiguiente = regTabla.getReferencias()->front();

	//cout<<"obtengo la ref:"<<bloqueSiguiente<<" en el bloque 0"<<endl;

	while (bloqueSiguiente > 0){

		bloqueTabla = archivoLista.recuperarBloque(bloqueSiguiente);

		regTabla = bloqueTabla->obtenerRegistros()->front();

		list<unsigned int>* listaParcial= regTabla.getAtributosEnteros();

		list<unsigned int>::iterator it2= listaDeBloques.end();

		bloqueSiguiente = regTabla.getReferencias()->front();

		//cout<<"obtengo la ref:"<<bloqueSiguiente<<endl;

		listaDeBloques.splice(it2,*listaParcial);

	}

	this->tamanioTabla = listaDeBloques.size();

	this->tabla = new unsigned int[this->tamanioTabla];

	unsigned int i = 0;

	list<unsigned int>::iterator it = listaDeBloques.begin();

	/*funciona esto? -ver despues */

	while(it != listaDeBloques.end()){

		this->tabla[i] = *it;

		it++;

		i++;
	}

	/********************************************************
	cout<<"-------------------------------------------------"<<endl;
	cout<<"tamanio tabla: "<<this->tamanioTabla<<endl;
	cout<<"-------------------------------------------------"<<endl;
	cout<<"tabla: "<<endl;
	for(int j=0;j<this->tamanioTabla;j++){ cout<<this->tabla[j];}
	cout<<endl;
	cout<<"-------------------------------------------------"<<endl;
	/*********************************************************/

	/*libero memoria pedida*/

	delete bloqueTabla;

	return 0;
}




void Hash::insertar(Registro *registro)
{

	//VALIDACION DE UNICIDAD //
	//cout<<"inserción:"<<endl;

	Registro *registroOld = this->buscar(registro->getString());

	if (registroOld == NULL)
	{

		elemLista posTabla = this->hasheo(registro->getString());

		unsigned int nroBloque = this->tabla[posTabla]; //hacerlo mejor esto despues//

		//cout<<"debo guardar en :"<<nroBloque<<endl;

		ArchivoBloques archivo(this->pathHash, TAMANIO_BLOQUE);

		Bloque *bloque = archivo.recuperarBloque(nroBloque);

		try{
			bloque->agregarRegistro(*registro);

			archivo.grabarBloque(bloque,nroBloque);

		}
		catch (ExceptionBloque &e)
		{

			unsigned int tamanioDispersion= this->tamDispersion(nroBloque);

			//obtiene un bloque libre, si habia uno de antes te devuelve un nro de bloque que ya existia, sino un nuevo//

			unsigned int primerBloqueLibre= archivo.getBloqueLibre();

			//cout<<"Bucket "<<nroBloque<<": esta desbordado; ";
			//cout<<"hay que escribir en un nodo nuevo libre, el :"<<primerBloqueLibre<<endl;

			unsigned int pos_Desbordado = posTabla;


			//escribe un bloque vacio en la posicion libre que encontramos. Si era una vieja pisa toda esa info con espacio en blanco.//
			//No es necesario tratar aparte los bloques vacios, porque de eso se encarga el manejador de arch de bloques.//

			unsigned int bloque_desbordado = this->tabla[pos_Desbordado];

			this->actualizarTabla_insercion(tamanioDispersion,pos_Desbordado,primerBloqueLibre);

			this->reestructurar_archivo(archivo,bloque_desbordado,primerBloqueLibre,registro);

		}
		/*libero memoria*/
		delete bloque;
		//cout<<"Registro insertado."<<endl;
	}
	else{
		//cout<<"registro duplicado."<<endl;

	}
}




void Hash::borrar(string que)
{
	//this->buscar(registro->que);
	//analizar si esta o no
	Registro *registroOld = this->buscar(que);

	//VALIDACION DE UNICIDAD//
	//cout<<"valido unicidad para eliminar"<<endl;
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

			if ((numBloque != this->tabla[posTabla]) && (this->tamanioTabla!=1)){
				//cout<<numBloque<<" "<<this->tabla[posTabla]<<endl;
				archivo.eliminarBloque(numBloque);
				Bloque* blvacio=new Bloque();
				archivo.grabarBloque(blvacio,numBloque);
				delete blvacio;
			}
			//if (tamTablaVieja > this->tamanioTabla) archivo.eliminarBloque(numBloque); //esta MAL!

			else archivo.grabarBloque(bloqueRecuperado,numBloque);

		}

		else archivo.grabarBloque(bloqueRecuperado,numBloque);

	}

}




Registro* Hash::buscar(string que){

	//logica de busqueda
	int nroElemento = this->hasheo(que);

	if (nroElemento < 0)	return NULL;

	elemLista nroBloque = this->tabla[nroElemento];

	//cout<<"busco el reg. con ID:'"<<que<<"' en bloque:"<<nroBloque<<endl;

	ArchivoBloques archivoBloq = ArchivoBloques(this->pathHash, TAMANIO_BLOQUE);


	Bloque *bloque = archivoBloq.recuperarBloque(nroBloque);

	Registro *registro = bloque->recuperarRegistro(que);
	Registro *reg_encontrado;

	if (registro==NULL){ //cout<<"no lo encontro. "<<endl;
	}
	else {
		Registro *reg_encontrado= new Registro(registro);
		delete bloque;
		return reg_encontrado;
	}
	return registro;
}





//probar funcion de dispersion, (que sea buena)//
/*
unsigned int Hash::hasheo(string key){
	unsigned int i=0;
	unsigned int parcial=0;
	unsigned int final=0;
	while(i<4 && i<key.length()){
		parcial=0;
		parcial=(unsigned int)key[i];
		final+=parcial;
		if (i<key.length()-1) final=final<<8;
		i++;
	}
	cout<<final<<" "<<endl;
	final%=this->tamanioTabla;

	return final;
}

unsigned int Hash::hasheo(string key){
	unsigned int final=atoi(key.c_str());
	final%=this->tamanioTabla;
	return final;
}*/

 unsigned int Hash::hasheo( string key )
{

	 unsigned int hashVal = 0;
	 for( int i = 0; i < key.length( ); i++ )
		hashVal = 37 * hashVal + key.at(i);
	 hashVal %= this->tamanioTabla;
	 if( hashVal < 0 )
		hashVal += this->tamanioTabla;

	 return hashVal;
}




void Hash::reestructurar_archivo(ArchivoBloques archivo,unsigned int nro_desbordado,unsigned int nro_libre, Registro* registro_a_ins){

	Bloque* bloque_desbordado =archivo.recuperarBloque(nro_desbordado);
	Bloque* bloque_libre =new Bloque();

	list<Registro>* lista_desbordado = bloque_desbordado->obtenerRegistros();
	list<Registro>* lista_libres = bloque_libre->obtenerRegistros();
	list<Registro>::iterator iteradorReg_desbordado = bloque_desbordado->obtenerRegistros()->begin();
	list<Registro>::iterator iteradorReg_libre = bloque_libre->obtenerRegistros()->begin();

	while( iteradorReg_desbordado !=  bloque_desbordado->obtenerRegistros()->end() ){

		unsigned int posTabla= this->hasheo(iteradorReg_desbordado->getString());

		//cout<<"tamanio tabla: "<<this->getTamanioTabla()<<endl;

		//cout<<"inicial: "<<nro_desbordado<<"; nuevo: "<< this->tabla[posTabla] <<" reg: "<<iteradorReg_desbordado->getString()<<"| posicion tabla: "<<posTabla<<endl;

		if (nro_libre == this->tabla[posTabla]){

			Registro reg_a_mover= *iteradorReg_desbordado;

			lista_libres->push_back(reg_a_mover);
			//lista_libres->insert(iteradorReg_libre,reg_a_mover);

			iteradorReg_desbordado=lista_desbordado->erase(iteradorReg_desbordado);

			iteradorReg_libre= lista_libres->end();
		}
		//else if ( iteradorReg_desbordado !=  bloque_desbordado->obtenerRegistros()->end()) advance(iteradorReg_desbordado,1);
		else iteradorReg_desbordado++;
	}

	unsigned posTabla= this->hasheo(registro_a_ins->getString());
	//cout<<posTabla<<endl;
	/*
	if (this->tabla[posTabla]== nro_libre){
		cout<<"nuevo va en libre."<<endl;
		iteradorReg_libre= lista_libres->end();
		lista_libres->insert(iteradorReg_libre,*registro_a_ins);
	}

	else{
		cout<<"nuevo va en desbord."<<this->hasheo(registro_a_ins->getString())<<endl;
		iteradorReg_desbordado= lista_desbordado->end();
		lista_desbordado->insert(iteradorReg_desbordado,*registro_a_ins);
	}*/
	//cout<<"***************desbord :"<<lista_desbordado->size()<<";nuevo :"<<lista_libres->size()<<endl;

	archivo.grabarBloque(bloque_desbordado,nro_desbordado);
	archivo.grabarBloque(bloque_libre,nro_libre);

	this->insertar(registro_a_ins);

	//cout<<"salgo bien"<<endl;
	//this->mostrar();
	delete bloque_libre;
	delete bloque_desbordado;
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

		memcpy(tabla+this->tamanioTabla,viejaTabla,this->tamanioTabla*sizeof(unsigned int));

		this->tamanioTabla*=2;
		delete viejaTabla;

		tabla[pos_desbordado]=bl_nuevo;
	}
	else{
		for(int i=0; i < this->tamanioTabla ; i+=(2*tamDispersion))
			tabla[pos_desbordado + i ]=bl_nuevo;
	}



/********************************************************************
	cout<<"--------------NUEVA TABLA:------------------------"<<endl;
	for(int i=0;i<this->tamanioTabla;i++) cout<<this->tabla[i]<<" ";
	cout<<endl;
	cout<<"--------------------------------------------------"<<endl;
/*********************************************************************/

}



void Hash::actualizarTabla_eliminacion(unsigned int tamDispersion,unsigned int pos_eliminado){

	int tamTabla=tamanioTabla;
	if (tamTabla>1){
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
		for (int i=0; i<this->tamanioTabla; i+=(tamDispersion))
			this->tabla[pos_eliminado + i]=this->tabla[salto1];
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
	/********************************************************************
		cout<<"--------------NUEVA TABLA:------------------------"<<endl;
		for(int i=0;i<this->tamanioTabla;i++) cout<<this->tabla[i]<<" ";
		cout<<endl;
		cout<<"--------------------------------------------------"<<endl;
	/*********************************************************************/

}


void Hash::guardarTabla(){

	list<unsigned int> refASiguientes;
	ArchivoBloques archivoHash(this->pathHash,TAMANIO_BLOQUE);
	Bloque* bloqueTabla= archivoHash.recuperarBloque(0);
	unsigned int refActual= bloqueTabla->obtenerRegistros()->front().getReferencias()->front();

	while (refActual != 0){
		refASiguientes.push_back(refActual);
		delete bloqueTabla;
		bloqueTabla= archivoHash.recuperarBloque(refActual);
		refActual= bloqueTabla->obtenerRegistros()->front().getReferencias()->front();
	}

	/*tengo que ver cuantos bloques voy a necesitar para guardar la lista de IDS  */
	/*para eso establezco un maximo de IDS por bloque, que en principio seran 1000*/
	/*chequeo la cantidad de IDS, y veo si los bloques que estaba usando antes me */
	/*alcanzan si no me alcanzan le pido mas, si me sobran libero.				  */

	float tamTabla_f= (float)this->tamanioTabla;

	unsigned int cantBloquesNecesarios = (unsigned int)(ceil(tamTabla_f / 1000));
	//cout<<"1)cant de bloques que NECESITO para guardar tabla:"<<cantBloquesNecesarios<<endl;

	while ((refASiguientes.size()+1) < cantBloquesNecesarios){

		refActual= archivoHash.getBloqueLibre();
		//cout<<refActual<<endl;
		refASiguientes.push_back(refActual);
		Bloque* bloqueNuevo= new Bloque();
		archivoHash.grabarBloque(bloqueNuevo,refActual);
		delete bloqueNuevo;
	}
	while ((refASiguientes.size()+1) > cantBloquesNecesarios){

		refActual= refASiguientes.back();
		archivoHash.eliminarBloque(refActual);
		refASiguientes.pop_back();
	}

	//******agrego a la lista de referencias el 0, porque el ultimo apunta a nada.*****//
	refASiguientes.push_back(0);

	list<unsigned int>::iterator itReferencias= refASiguientes.begin();

	//** esto que sigue es prueba**//
	for (itReferencias ;itReferencias!=refASiguientes.end();itReferencias++);// cout<<*itReferencias;
	//cout<<endl;
	//** fin de prueba**//

	unsigned int i=0;
	itReferencias=refASiguientes.begin();
	while ( i< this->tamanioTabla ){

		unsigned int cont= 0;
		Registro tablaNueva;

		while(cont<1000 &&  i< this->tamanioTabla){
			tablaNueva.agregarAtribEntero(this->tabla[i]);
			i++;
			cont++;
		}

		/*TODO ver estas 2 lineas que siguen.*/
		tablaNueva.agregarReferencia((*itReferencias));
		//cout<<"agrego:"<<*itReferencias<<endl;
		Bloque* bloqueTabla= new Bloque();

		bloqueTabla->agregarRegistro(tablaNueva);
		if (itReferencias==refASiguientes.begin()){
			archivoHash.grabarBloque(bloqueTabla,0);
			//cout<<"0"<<"|->"<<*(refASiguientes.begin())<<endl;
		}
		else{
			itReferencias--;
			//cout<<*itReferencias <<"|->"<< tablaNueva.getReferencias()->front()<<endl;
			archivoHash.grabarBloque(bloqueTabla,*itReferencias);
			itReferencias++;
		}
		itReferencias++;

		/*libero memoria*/
		delete bloqueTabla;
		delete tabla;
	}

	//tablaNueva.agregarReferencia(1);

	//list<Registro>::iterator iterador= bloqueTabla->obtenerRegistros()->begin();

	//bloqueTabla->obtenerRegistros()->insert(iterador,tablaNueva);

	//ArchivoBloques manejadorHash(this->pathHash,TAMANIO_BLOQUE);

	//manejadorHash.grabarBloque(bloqueTabla,0);

	//ArchivoBloques arch(this->pathHash,TAMANIO_BLOQUE);


}


void Hash::mostrar(){
	ArchivoBloques archivo(this->pathHash,TAMANIO_BLOQUE);
	Bloque* bloque=NULL;
	unsigned int max=0;
	cout<<"******************************************"<<endl;
	cout<<"DATOS:"<<endl;
	cout<<"------------------------------------------"<<endl;
	cout<<"|tamaño de tabla: "<<this->tamanioTabla<<endl;
	cout<<"| ";
	for(unsigned int i=0; i<this->tamanioTabla; i++){
		cout<<this->tabla[i]<<" ";
		if (this->tabla[i] > max) max=this->tabla[i];
	}
	cout<<endl;
	char a;
	cout<<"----------------------------------------- "<<endl;
	for (unsigned int i=1;i<max+1;i++){
		if (i%50==0){ cout<<"presione cualquier tecla para continuar..."<<endl; cin>>a;}
		cout<<"|Bloque "<<i<<"| T.d: "<<this->tamDispersion(i)<<"]  ";
		bloque= archivo.recuperarBloque(i);
		list<Registro>::iterator it= bloque->obtenerRegistros()->begin();
		while(it!=bloque->obtenerRegistros()->end() ){
			cout<<it->getString()<<" ";
			it++;
		}
		cout<<endl;
	}
	delete bloque;
	cout<<"******************************************"<<endl;
}


void Hash::mostrar2(string nombre){
	ArchivoBloques archivo(this->pathHash,TAMANIO_BLOQUE);
	stringstream conversor;
	Bloque* bloque=NULL;
	unsigned int max=0;
	string resultado="";
	resultado+="******************************************";
	resultado+='\n';
	resultado+="DATOS:";
	resultado+="\n";
	resultado+="------------------------------------------";
	resultado+='\n';
	resultado+="|tamaño de tabla: ";
	conversor<<this->tamanioTabla;
	resultado+=conversor.str();
	conversor.str("");
	resultado+='\n';
	resultado+="| ";
	for(unsigned int i=0; i<this->tamanioTabla; i++){
		conversor<<this->tabla[i];
		resultado+=conversor.str();
		resultado+=" ";
		if (this->tabla[i] > max) max=this->tabla[i];
		conversor.str("");
	}
	resultado+='\n';
	resultado+="----------------------------------------- ";
	resultado+='\n';
	for (unsigned int i=1;i<max+1;i++){
		resultado+="|Bloque "; conversor<<i;
		resultado+=conversor.str(); conversor.str(""); resultado+="| T.d: ";
		conversor<<this->tamDispersion(i); resultado+=conversor.str();
		conversor.str(""); resultado+="]  ";
		bloque= archivo.recuperarBloque(i);
		list<Registro>::iterator it= bloque->obtenerRegistros()->begin();
		while(it!=bloque->obtenerRegistros()->end()){
			resultado+=it->getString()+" ";
			it++;
		}
		resultado+='\n';
		resultado+='\n';
	}
	resultado+="******************************************";
	resultado+="\n";
	fstream archtexto;
	archtexto.open(nombre.c_str(), ios::out | ios::app);
	archtexto << resultado;
	archtexto.close();
}

unsigned int Hash::getTamanioTabla(){
	return this->tamanioTabla;
}

void Hash::setTabla(unsigned int* ta){
	this->tabla=ta;
}

unsigned int* Hash::getTabla(){ return this->tabla;}
