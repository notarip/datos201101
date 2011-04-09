#include "ArchivoLibros.h"
#include "Libro.h"
#include <iostream>
using namespace std;


int main(int argc, char** argv){
	//prueba libro//

	Libro libro(43,"dsadsadsaad","fdsfdsfds","texto de prueba" , "prueba,prueba,prueba,prueba,prueba,prueba");

	Libro libro2(43,"bcabcabcabcabcabca","abcdabcdabcdabcd","abcdabcdabcdabcda","bcdabcdabcdabcdabcdabcd");

	Libro libro3(15, "gwfewfew","gregregre","gregregasa","reuasfewfefewewfewfewhdasd");

	ArchivoLibros archi("archi_datos.dat");

	//serializacion y deserializacion//

	char* cadena;

	archi.serializar(&libro,&cadena);

	Libro* libro4;

	archi.deserializar(cadena,&libro4);

	cout<<libro4->getId()<<";"<<libro4->getTitulo()<<";"<<libro4->getAutor()<<";"<<libro4->getEditorial()<<endl;


	//Agregado de registros//
	archi.agregarLibro(&libro);
	archi.agregarLibro(&libro2);
	archi.agregarLibro(&libro3);

	//recuperacion de registros //
	Libro* libror1;
	Libro* libror2;
	libror1= archi.recuperarLibro(0);
	libror2= archi.recuperarLibro(198);

	//recuperacion comprensiva//
	cout<<"recuperacion comprensiva:"<<endl;
	std::list<Libro>* p=new list<Libro>;
	p=archi.recuperacionComprensiva();
	std::list<Libro>::iterator it;
	it= p->begin();
	it++;
	it++;
	cout<<it->getId()<<";"<<it->getTexto()<<endl;

	//supresion de registros //

	archi.suprimirLibro(0);
	fstream archdebajas("bajas_rvariables.dat",ios::in);
	char* tira=new char[8];
	archdebajas.read(tira,8);
	cout<<(unsigned int)tira[0]<<";"<<(unsigned int)(tira+4)[0]<<endl;

   //insercion sobre una baja logica//

	Libro libro5(15, "holahola","yoyoyoyo","nononon","chauuchauchauchauchau");

	archi.agregarLibro(&libro5);

}
