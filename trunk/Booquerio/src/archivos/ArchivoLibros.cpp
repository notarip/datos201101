/*
 * ArchivoLongVariable.cpp
 *
 *  Created on: 01/04/2011
 *      Author: rjc
 */

#include "ArchivoLibros.h"


ArchivoLibros::ArchivoLibros(string path) {
	this->path= path;
}


//busca espacio en el archivo e inserta el registro variable //
void ArchivoLibros::agregarLibro(Libro* unLibro){
	char* tiraBytes=NULL;
	int posicion=-1;
	this->serializar(unLibro,&tiraBytes);
	unsigned int longreg= this->longReg;//(unsigned int)tiraBytes[0]; //<-- longitud de registro variable.//
	char path_bajas[]="bajas_rvariables.dat"; //<-- debe ser parametrizable por el usuario//
	fstream archivo;
	fstream bajas (path_bajas ,ios::in | ios::out );
	unsigned int offset=0;
	if (bajas){
		archivo.open(this->path.c_str(), ios::out| ios::in | ios::binary | ios::ate);
		char* libres=new char[8];
		bool listo=false;
		while(!bajas.eof() || !listo){
			bajas.read(libres,2*sizeof(unsigned int)); //<-- copia los libres de algun reg, y el offset//
			listo= (longreg <=(unsigned int)libres[0]);
			posicion++;
		}

		if (listo){
			unsigned int* resto=new unsigned int((unsigned int)libres[0]-longreg);
			cout<<*resto<<endl;
			memcpy(libres,resto,sizeof(unsigned int));
			bajas.seekg(posicion*(2*sizeof(unsigned int)),ios::beg);
			bajas.write(libres,sizeof(unsigned int));
			offset= (unsigned int)libres[1];
			archivo.seekp(offset,ios::beg);
			archivo.write(tiraBytes,longreg);
		}

	}
	else{
		archivo.open(this->path.c_str(), ios::out| ios::in | ios::binary | ios::app);
		cout<<"no reescribo."<<endl;
		archivo.seekp(0,ios::end);
		archivo.write(tiraBytes,longreg);
	}

	archivo.close();
	bajas.close();
}



void ArchivoLibros::suprimirLibro(unsigned int id){
	char* regBajas=new char[8];
	unsigned int offset= this->obtenerOffset(id);
	fstream archivo (this->path.c_str(),ios::in | ios::out );
	archivo.seekg(offset,ios::beg);
	archivo.read(regBajas,sizeof(unsigned int));
	archivo.close();
	memcpy(regBajas+sizeof(unsigned int),&offset,sizeof(unsigned int));
	char path_bajas[]="bajas_rvariables.dat";
	fstream archivo_bajas (path_bajas,ios::in | ios::out | ios::app);
	archivo_bajas.seekg(0,ios::end);
	archivo_bajas.write(regBajas,2*sizeof(unsigned int));
	archivo_bajas.close();
}



Libro* ArchivoLibros::recuperarLibro(unsigned int id){
	char* tiraBytes=NULL;
	Libro* obtenido=NULL;

	tiraBytes = this->levantarRegistro(id);
	this->deserializar(tiraBytes,&obtenido);
	return obtenido;
}


char* ArchivoLibros::levantarRegistro(unsigned int id){
	unsigned int offset= this->obtenerOffset(id);
	char* charTamanio =new char[4];
	fstream archivo (this->path.c_str(),ios::in | ios::out);
	archivo.seekg(offset,ios::beg);
	archivo.read(charTamanio ,sizeof(unsigned int));
	unsigned int tamanio = (unsigned int)charTamanio[0];
	char* tiraBytes= new char[ tamanio ];
	memcpy(tiraBytes,&tamanio,sizeof(unsigned int));
	archivo.read(tiraBytes+sizeof(unsigned int),tamanio-sizeof(unsigned int));
	return tiraBytes;
}


//recorre todo el archivo y devuelve los libros//
list<Libro>* ArchivoLibros::recuperacionComprensiva(){
	fstream archivo (this->path.c_str(),ios::in | ios::out);
	archivo.seekg(0,ios::beg);
	char* charTamanio =new char[4];
	Libro* unLibro=NULL;
	list<Libro>* listaLibros=new list<Libro>();
	std::list<Libro>::iterator it;
	it=listaLibros->end();

	while (!archivo.eof()){
		archivo.read(charTamanio ,sizeof(unsigned int));
		unsigned int tamanio = (unsigned int)charTamanio[0];
		char* tiraBytes= new char[ tamanio ];
		memcpy(tiraBytes,&tamanio,sizeof(unsigned int));
		archivo.read(tiraBytes+sizeof(unsigned int),tamanio-sizeof(unsigned int));
		this->deserializar(tiraBytes,&unLibro);
		listaLibros->push_back(*unLibro);
		delete []tiraBytes;
	}
	return listaLibros;

}



//serializa los datos de un Libro en una tira de bytes//
void ArchivoLibros::serializar(Libro* unLibro,char** tira){
	unsigned int tamanioReg=(unLibro->getAutor().size()) + (unLibro->getTitulo().size())+(unLibro->getTexto().size())
	+(unLibro->getEditorial().size()) + 6*sizeof(unsigned int);
	this->longReg = tamanioReg;
	unsigned int offset=0;
	char* tiraBytes = new char[ tamanioReg ];
	memcpy(tiraBytes, &(tamanioReg), sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	unsigned int * p_id= new unsigned int(unLibro->getId());
	memcpy(tiraBytes+ offset, p_id , sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	unsigned int* tamanioCampos=new unsigned int(unLibro->getTitulo().size());
	memcpy(tiraBytes+ offset, (tamanioCampos) ,sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	memcpy(tiraBytes+ offset, (unLibro->getTitulo().c_str()),*tamanioCampos);
	offset+=*tamanioCampos;
	*tamanioCampos= unLibro->getAutor().size();
	memcpy(tiraBytes+ offset,(tamanioCampos),sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	memcpy(tiraBytes+ offset,(unLibro->getAutor().c_str()),*tamanioCampos);
	offset+=*tamanioCampos;
	*tamanioCampos= unLibro->getEditorial().size();
	memcpy(tiraBytes+ offset,(tamanioCampos),sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	memcpy(tiraBytes+ offset,(unLibro->getEditorial().c_str()),*tamanioCampos);
	offset+=*tamanioCampos;
	*tamanioCampos= unLibro->getTexto().size();
	memcpy(tiraBytes+ offset,(tamanioCampos),sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	memcpy(tiraBytes+ offset,(unLibro->getTexto().c_str()),*tamanioCampos);
	*tira=tiraBytes;
	cout<<tamanioReg<<endl;
}




//deserializa una tira de bytes convirtiendola en un Libro//
void ArchivoLibros::deserializar(char* tiraBytes,Libro** obtenido){
	unsigned int *tamanioCampo =new unsigned int(0);
	unsigned int *id = new unsigned int(0);
	char *titulo,*autor,*editorial,*texto;
	unsigned int posicion=sizeof(unsigned int);

	memcpy(id,tiraBytes+ posicion ,sizeof(unsigned int));
	posicion+=sizeof(unsigned int);
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	titulo=new char[*tamanioCampo];
	posicion+=sizeof(unsigned int);
	memcpy(titulo,tiraBytes+ posicion ,(*tamanioCampo));
	posicion+=(*tamanioCampo);
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	autor=new char[(*tamanioCampo)];
	posicion+=sizeof(unsigned int);
	memcpy(autor,tiraBytes+ posicion,(*tamanioCampo));
	posicion+=*tamanioCampo;
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	editorial=new char[(*tamanioCampo)];
	posicion+=sizeof(unsigned int);
	memcpy(editorial,tiraBytes+ posicion,(*tamanioCampo));
	posicion+=*tamanioCampo;
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	texto=new char[(*tamanioCampo)];
	posicion+=sizeof(unsigned int);
	memcpy(texto,tiraBytes+ posicion,(*tamanioCampo));
	posicion+=*tamanioCampo;

	string s_autor(autor, strlen(autor));
	string s_titulo(titulo, strlen(titulo));
	string s_editorial(editorial, strlen(editorial));
	string s_texto(texto,strlen(texto));

	Libro* libro= new Libro(*id,s_titulo,s_autor,s_editorial,s_texto);

	*obtenido= libro;


}




unsigned int ArchivoLibros::obtenerOffset(unsigned int id){
	return id;
}




ArchivoLibros::~ArchivoLibros() {
	// TODO Auto-generated destructor stub
}
