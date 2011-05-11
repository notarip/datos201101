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
unsigned int ArchivoLibros::agregarLibro(Libro* unLibro){

	unsigned int posicion_escritura=0;

	char* tiraBytes=NULL;
	char* libres;
	int posicion=-1;
	unsigned int longreg;
	bool existiaEspacio=false;

	this->serializar(unLibro,&tiraBytes);
	longreg= this->longReg; //<-- longitud de registro variable.//

	fstream archivo;
	fstream bajas;

	string path_bajas=this->path + "-libres";
	bajas.open(path_bajas.c_str() ,ios::in | ios::out | ios::binary );

	if (bajas.good()){

		bajas.seekg(0,ios::beg);

		archivo.open(this->path.c_str(), ios::out| ios::in | ios::binary );

		libres=new char[8];
		bool listo=false;
		unsigned int* longLibre=new unsigned int(0);
		while(!bajas.eof() && !listo){
			bajas.read(libres,2*sizeof(unsigned int)); //<-- copia los libres de algun reg, y el offset//
			memcpy(longLibre,libres,sizeof(unsigned int));
			listo= (longreg <=(*longLibre));
			posicion++;
		}

		if (listo){

			unsigned int* resto=new unsigned int((*longLibre)-longreg);
			unsigned int* offset= new unsigned int(0);
			libres+=4;
			memcpy(offset,libres,sizeof(unsigned int));
			//cout<<"off para copiar:"<<*offset<<endl;
			archivo.seekp( (*offset) ,ios::beg);

			posicion_escritura= archivo.tellp();

			archivo.write(tiraBytes,longreg);
			*(offset)= *(offset) + longreg; //modifico offset
			//cout<<"Offset :"<<*offset<<endl;
			//cout<<"Libre: "<<*resto<<endl;
			memcpy(libres,offset,sizeof(unsigned int));
			libres-=4;

			memcpy(libres,resto,sizeof(unsigned int));

			bajas.seekp(2*sizeof(unsigned int)*posicion,ios::beg);

			bajas.write(libres,2*sizeof(unsigned int));

			existiaEspacio=true;

			//delete resto;
			//delete offset;

		}
		else{
			archivo.seekp(0,ios::end);
			posicion_escritura= archivo.tellp();
			archivo.write(tiraBytes,longreg);
		}
		bajas.close();
		//delete libres;
		//delete longLibre;
	}
	else {
		archivo.open(this->path.c_str(), ios::out | ios::app);
		archivo.seekp(0,ios::end);
		posicion_escritura= archivo.tellp();
		archivo.write(tiraBytes,longreg);

	}
	archivo.close();
	return posicion_escritura;
	delete tiraBytes;


}



void ArchivoLibros::suprimirLibro(unsigned int id){
	char* regBajas=new char[8];
	unsigned int offset= this->obtenerOffset(id);

	fstream archivo;
	//cout<<"El offset es "<<id<<endl;
	archivo.open(this->path.c_str(),ios::in | ios::out );
	//cout<<"El path es "<<path<<endl;
	//cout<<boolalpha<<archivo.good()<<endl;
	if (archivo.good()){
		archivo.seekg(offset,ios::beg);
		archivo.read(regBajas,sizeof(unsigned int));
		archivo.close();
		memcpy(regBajas+sizeof(unsigned int),&offset,sizeof(unsigned int));
		string path_bajas= this->path;
		path_bajas += "-libres";
		//cout<<"intento eliminar:"<<path_bajas<<endl;
		fstream archivo_bajas;

		archivo_bajas.open(path_bajas.c_str() ,ios::in |ios::binary| ios::out | ios::app);

		if (!archivo_bajas.good()){
			archivo_bajas.open(path_bajas.c_str() , ios::out | ios::binary |ios::app);
		}

		archivo_bajas.seekg(0,ios::end);
		archivo_bajas.write(regBajas,2*sizeof(unsigned int));
		archivo_bajas.close();
	}

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
	char* tiraBytes=NULL;
	char* charTamanio =new char[4];
	fstream archivo;
	archivo.open(this->path.c_str(),ios::binary | ios::in);
	if(archivo.good()){
		archivo.seekg(offset,ios::beg);
		archivo.read(charTamanio ,sizeof(unsigned int));
		unsigned int *tamanioReg = new unsigned int(0);
		memcpy(tamanioReg,charTamanio,sizeof(unsigned int));
		tiraBytes= new char[ (*tamanioReg) ];
		memcpy(tiraBytes,tamanioReg ,sizeof(unsigned int));
		archivo.read(tiraBytes+4*sizeof(char),(*tamanioReg)-4*sizeof(char));
	}

	return tiraBytes;
}


//recorre todo el archivo y devuelve los libros//
list<Libro>* ArchivoLibros::recuperacionComprensiva(){
	fstream archivo (this->path.c_str(),ios::binary | ios::in);
	archivo.seekg(0,ios::beg);
	char* charTamanio =new char[4];
	Libro* unLibro=NULL;
	list<Libro>* listaLibros=new list<Libro>();
	char* tiraBytes=NULL;
	archivo.seekp(0 , ios::end);
	unsigned int finArchivo = archivo.tellp();
	archivo.seekg(0 , ios::beg);

	while ( archivo.tellg()!= finArchivo){

		archivo.read(charTamanio ,sizeof(unsigned int));
		unsigned int* tamanio = new unsigned int(0);
		memcpy(tamanio,charTamanio,sizeof(unsigned int));
		tiraBytes= new char[ (*tamanio) ];
		memcpy(tiraBytes,tamanio,sizeof(unsigned int));
		archivo.read(tiraBytes+sizeof(unsigned int),(*tamanio)-sizeof(unsigned int));
		this->deserializar(tiraBytes,&unLibro);
		listaLibros->push_back(*unLibro);



	}
	return listaLibros;

}


void ArchivoLibros::serializar(Libro* unLibro,char** tira){
	unsigned int tamanioReg=(unLibro->getAutor().size()) + (unLibro->getTitulo().size())+ (unLibro->getEditorial().size())
	+ (unLibro->getTexto().size()) + (unLibro->getPalabras().size()) + 8*sizeof(unsigned int);

	unsigned int offset=0;

	char* tiraBytes = new char[ tamanioReg ];                       //calcula espacio de
	memcpy(tiraBytes, &(tamanioReg), sizeof(unsigned int));         //dato de long de Reg.

	offset+=sizeof(unsigned int);

	unsigned int * p_id= new unsigned int(unLibro->getId());
	unsigned int * cant_palabras= new unsigned int(unLibro->getCantPalabras());
	memcpy(tiraBytes + offset, p_id , sizeof(unsigned int));

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
	offset+=*tamanioCampos;

	memcpy(tiraBytes+ offset, cant_palabras ,sizeof(unsigned int));
	offset+=sizeof(unsigned int);

	*tamanioCampos= unLibro->getPalabras().size();
	memcpy(tiraBytes+ offset, (tamanioCampos),sizeof(unsigned int));
	offset+=sizeof(unsigned int);

	memcpy(tiraBytes+ offset,(unLibro->getPalabras().c_str()),*tamanioCampos);
	offset+=*tamanioCampos;
	*tira=tiraBytes;

	this->longReg = offset;

	//cout<<longReg<<endl;

}




//deserializa una tira de bytes convirtiendola en un Libro//
void ArchivoLibros::deserializar(char* tiraBytes,Libro** obtenido){

	unsigned int *tamanioCampo =new unsigned int(0);
	unsigned int *id = new unsigned int(0);
	unsigned int *cantPalabras= new unsigned int(0);

	char *titulo,*autor,*editorial,*texto,*palabras;
	unsigned int posicion=sizeof(unsigned int);

	memcpy(id,tiraBytes+ posicion ,sizeof(unsigned int));
	posicion+=sizeof(unsigned int);

	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	titulo=new char[(*tamanioCampo)+1];
	posicion+=sizeof(unsigned int);
	memcpy(titulo,tiraBytes+ posicion ,(*tamanioCampo));
	titulo[*tamanioCampo]='\0';
	posicion+=(*tamanioCampo);
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	autor=new char[(*tamanioCampo)+1];
	posicion+=sizeof(unsigned int);
	memcpy(autor,tiraBytes+ posicion,(*tamanioCampo));
	autor[*tamanioCampo]='\0';
	posicion+=*tamanioCampo;
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	editorial=new char[(*tamanioCampo)+1];
	posicion+=sizeof(unsigned int);
	memcpy(editorial,tiraBytes+ posicion,(*tamanioCampo));
	editorial[*tamanioCampo]='\0';
	posicion+=*tamanioCampo;
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int));
	texto=new char[(*tamanioCampo)+1];
	posicion+=sizeof(unsigned int);
	memcpy(texto,tiraBytes+ posicion,(*tamanioCampo));
	texto[*tamanioCampo]='\0';
	posicion+=*tamanioCampo;

	memcpy(cantPalabras,tiraBytes+ posicion,sizeof(unsigned int));
	posicion+=sizeof(unsigned int);
	memcpy(tamanioCampo,tiraBytes+ posicion,sizeof(unsigned int)); //tamanio de palabras//
	posicion+=sizeof(unsigned int);
	palabras=new char[(*tamanioCampo)+1];
	memcpy(palabras,tiraBytes+ posicion,(*tamanioCampo));
	palabras[*tamanioCampo]='\0';


	string s_autor(autor, strlen(autor));
	string s_titulo(titulo, strlen(titulo));
	string s_editorial(editorial, strlen(editorial));
	string s_texto(texto,strlen(texto));


	string s_palabras(palabras,strlen(palabras));

	Libro* libro= new Libro(*id,s_titulo,s_autor,s_editorial,s_texto,*cantPalabras,s_palabras);

	*obtenido= libro;
	delete []tiraBytes;

}




unsigned int ArchivoLibros::obtenerOffset(unsigned int id){
	return id;
}



ArchivoLibros::~ArchivoLibros() {
	// TODO Auto-generated destructor stub
}
