#include "../archivos/ArchivoLibros.h"
#include "../archivos/Libro.h"
#include <iostream>
using namespace std;


int main423(int argc, char** argv){
	//prueba libro//

	Libro libro(43,"ES EL LIBRO 1","fdsfdsfds","texto de prueba" , "prueba,prueba,prueba,prueba,prueba,prueba");

	Libro libro2(91,"ES EL LIBRO 2","abcdabcdabcdabcd","abcdabcdabcdabcda","bcdabcdabcdabcdabcdabcd");

	Libro libro3(15, "ES EL LIBRO 3","gregregre","gregregasa","reuasfewfefewewfewfewhdasd");

	ArchivoLibros archi("archi_datos");



	//serializacion y deserializacion//

	Libro* libro4=NULL;

	unsigned int off1=archi.agregarLibro(&libro);
	unsigned int off2=archi.agregarLibro(&libro2);
	unsigned int off3=archi.agregarLibro(&libro3);

	libro4= archi.recuperarLibro(off1);

	cout<<libro4->getId()<<";"<<libro4->getTitulo()<<";"<<libro4->getAutor()<<";"<<libro4->getEditorial()<<endl;
	delete libro4;
	libro4= archi.recuperarLibro(off2);
	cout<<libro4->getId()<<";"<<libro4->getTitulo()<<";"<<libro4->getAutor()<<";"<<libro4->getEditorial()<<endl;
	delete libro4;
	Libro* libro5;
	libro5= archi.recuperarLibro(off3);
	cout<<libro5->getId()<<";"<<libro5->getTitulo()<<";"<<libro5->getAutor()<<";"<<libro5->getEditorial()<<endl;
	cout<<off1<<" "<<off2<<" "<<off3<<endl;

	//supresion de registros //

	archi.suprimirLibro(off1);
	archi.suprimirLibro(off2);
	char a;
	cin>>a;
   //insercion sobre una baja logica//

	Libro libro6(15, "ESTE ES EL LIBRO 6","yoyoyoyo","nononon","chauuuchauchau");
	Libro libro7(32, "ESTE ES EL LIBRO 7","FREFERFRE","sdadS","dsada");
	Libro libro8(66, "ESTE ES EL LIBRO 8","ESTE ES MAS LARGO QUE LOS DEMAS","ES UNA PRUEBA","LA ULTIMA PRUEBA");

	int off4=archi.agregarLibro(&libro6);
	int off5=archi.agregarLibro(&libro7);
	int off6=archi.agregarLibro(&libro8);
	cout<<"off4: "<<off4<<" .off5: "<<off5<<" "<<off6<<endl;

	Libro* libro9;
	libro9= archi.recuperarLibro(off6);
	cout<<libro9->getId()<<";"<<libro9->getTitulo()<<";"<<libro9->getAutor()<<";"<<libro9->getEditorial()<<endl;

}
