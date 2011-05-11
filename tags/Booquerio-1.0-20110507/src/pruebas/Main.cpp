//TODO impresión de libro entero, cuando sale por pantalla chotea las ñ y los tides.
//TODO sacar a otra clase por que sino levanta las editoriales para cada libro



#include <iostream>
#include "../util/Parametros.h"
#include "../parser/Parser.h"
#include "../archivos/Libro.h"

void ascii()
{

	int i;

	printf("C%cdigo\t-\tLetra\n\n", 162);

	for(i=0; i<256; i++)
	{
		printf("%d\t-\t%c\n", i, i);
	}

}

void ascii2()
{
 string a = "áéíóú-aeiou";
 char* pc = new char[a.size()];
 string b;
 string c;

 cout << a.c_str() << endl;

 for(int i= 0; i<a.size();i++)
 {
	 //cout << a[i] << "-" << (int)(a[i]) <<endl;
	 b.append(1,a[i]);
	 pc[i] = a.at(i);
 }

 cout << b.c_str() <<endl;
 cout << pc << endl;

}

void pausa()
{
	int a;
	cin >> a;
}


void pruebaTilde()
{
	fstream arc;
	string linea;

	arc.open("./archivos/pruebas/prueba1.txt");

	arc.seekg(0, ios::end);
	int tamanio = arc.tellg();
	arc.seekg(0);

	//char* buff = new char[tamanio];
	char* buff = new char[1];
//	arc.read(buff, tamanio);
	while (!arc.eof())
	{
		arc.read(buff, 1);
		linea.append(buff);
	}

	Util().sinTilde(linea);

	delete [] buff;
}

int main0 (int argc, char** argv)
{


//	string enter = "\naa\n";
//	cout << enter.c_str();
//	for (int j = 0; j < enter.size();j++)
//		if (enter.at(j) == '\n')
//				cout << "barra n" << endl;
//
//	unsigned int pos = enter.find('\n');

	//ascii2();
	//pruebaTilde();

	if (true)
	{


		if (false)
		{
			/*
				 * PRUEBAS DE PARSEO DELA ARCHIVO DE PROPIEDADES
				 */


			cout << Parametros().getParametro("path_datos").c_str() << endl;

			cout << Parametros().getParametro("nada").c_str() << endl;

			cout << Parametros().getParametro("P1").c_str() << endl;
		}
		/*
		 * PRUEBAS DE PARSEO DELA ARCHIVO DE PROPIEDADES
		 */

		/*
		 * PRUEBA DEL PARSEO DEL ARCHIVO DE STOP WORDS
		 */
		if (false)
		{
			Parser *parser = new Parser();

			parser->listarStopWords();

			pausa();

			parser->~Parser();

			pausa();
		}
		/*
		 * PRUEBA DEL PARSEO DEL ARCHIVO DE STOP WORDS
		 */

		/*
		 * PRUEBA DEL PARSEO DEL LIBRO
		 */
		if (false)
		{
			Parser *parser2 = new Parser();

			parser2->parsear("./archivos/pruebas/libro1.txt");

			parser2->listarLibro(true);


			parser2->~Parser();
		}
		/*
		 * PRUEBA DEL PARSEO DEL LIBRO
		 */

		if (false)
		{
			Parser *parser3 = new Parser();
			fstream salida;

			salida.open("./archivos/pruebas/salida_libro1.txt",ios::out);
			parser3->parsear("./archivos/pruebas/prueba1.txt");

			set<string> *lista = parser3->obtenerPalabras();

			for (set<string>::iterator it = lista->begin(); it != lista->end(); it++)
			{
				cout << (*it).c_str() << endl;
				salida.write((*it).c_str(),(*it).size());
				salida.write("\n",1);
			}
			salida.close();
		}

		if (false)
		{

			list<string> *archivos= Util().getArchivos("./archivos/libros/");

			for (list<string>::iterator it= archivos->begin(); it != archivos->end();it++)
				cout << (*it).c_str() << endl;

			archivos->clear();
			delete archivos;


			cout << "Fin: listado de archivos en ./archivos/libros/" << endl;
			pausa();
		}



		if (false)
		{
			string rutaLibros = Parametros().getParametro(CARPETA_LIBROS);
			list<string> *archivos2 = Util().getArchivos(rutaLibros);
			archivos2->sort();
			string libro;
			Parser *parser4 = new Parser();
			int cont = 1;

			for (list<string>::iterator it= archivos2->begin(); it != archivos2->end();it++)
			{
				libro = rutaLibros + (*it);
				parser4->parsear(libro);
				parser4->listarLibro(false);
				cout << "--------------------------" << endl;

				if (cont == 0)
				{
					set<string> *listap = parser4->obtenerPalabras();
					for (set<string>::iterator it2 = listap->begin(); it2 != listap->end(); it2++)
						cout << (*it2).c_str() << endl;
					listap->clear();
					break;
				}
				else
					cont--;

				parser4->~Parser();
				parser4 = new Parser();
			}

			parser4->~Parser();
			archivos2->clear();

			cout << "Fin de listar libro" << endl;
			pausa();
		}
	}

	if (true)
	{
		string rutaLibros2 = "";

		rutaLibros2 += Parametros().getParametro(CARPETA_LIBROS);

		list<string> *archivos3 = Util().getArchivos(rutaLibros2);

		Parser *parser10 = new Parser();

		//parser10->parsear(rutaLibros2 + archivos3->front());

		parser10->parsear("./archivos/pruebas/Alejando Pérez - La saga - El Tenebroso.txt");


		Libro *unLibro;

		unLibro = parser10->getLibro();

		parser10->~Parser();

		parser10 = new Parser();

		set<string> *listap2 = parser10->obtenerPalabras(*unLibro);

		for (set<string>::iterator itp2 = listap2->begin(); itp2 != listap2->end(); itp2++)
			cout << (*itp2).c_str() << endl;

		listap2->clear();

		parser10->~Parser();

		unLibro->~Libro();

	}

	return 0;
}
