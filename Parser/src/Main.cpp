

#include <iostream>
#include "util/Parametros.h"
#include "parser/Parser.h"



void pausa()
{
	int a;
	cin >> a;
}


int main (int argc, char** argv)
{


//	string enter = "\naa\n";
//	cout << enter.c_str();
//	for (int j = 0; j < enter.size();j++)
//		if (enter.at(j) == '\n')
//				cout << "barra n" << endl;
//
//	unsigned int pos = enter.find('\n');


	if (true)
	{
		bool prueba1 = false;
		bool prueba2 = false;
		bool prueba3 = false;
		bool prueba4 = false;
		bool prueba5 = false;
		bool prueba6 = true;



		if (prueba1)
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
		if (prueba2)
		{
			Parser *parser = new Parser();

			parser->listarStopWords();

			parser->~Parser();
		}
		/*
		 * PRUEBA DEL PARSEO DEL ARCHIVO DE STOP WORDS
		 */

		/*
		 * PRUEBA DEL PARSEO DEL LIBRO
		 */
		if (prueba3)
		{
			Parser *parser2 = new Parser();

			parser2->parsear("./archivos/libros/libro1.txt");

			parser2->listarLibro(true);

			parser2->~Parser();
		}
		/*
		 * PRUEBA DEL PARSEO DEL LIBRO
		 */

		if (prueba4)
		{
			Parser *parser3 = new Parser();
			fstream salida;

			salida.open("./archivos/libros/salida_libro1.txt",ios::out);
			parser3->parsear("./archivos/libros/libro1.txt");

			list<string> *lista = parser3->obtenerPalabras();

			for (list<string>::iterator it = lista->begin(); it != lista->end(); it++)
			{
				cout << (*it).c_str() << endl;
				salida.write((*it).c_str(),(*it).size());
				salida.write("\n",1);
			}
			salida.close();
		}

		if (prueba5)
		{

			list<string> *archivos= Util().getArchivos("./archivos/libros/");

			for (list<string>::iterator it= archivos->begin(); it != archivos->end();it++)
				cout << (*it).c_str() << endl;

			archivos->clear();
			delete archivos;


			cout << "Fin: listado de archivos en ./archivos/libros/" << endl;
			pausa();
		}



		if (prueba6)
		{
			string rutaLibros = Parametros().getParametro(CARPETA_LIBROS);
			list<string> *archivos2 = Util().getArchivos(rutaLibros);
			string libro;
			Parser *parser4 = new Parser();
			int cont = 10;

			for (list<string>::iterator it= archivos2->begin(); it != archivos2->end();it++)
			{
				libro = rutaLibros + (*it);
				parser4->parsear(libro);
				parser4->listarLibro(false);
				cout << "--------------------------" << endl;

				if (cont == 0)
				{
					list<string> *listap = parser4->obtenerPalabras();
					for (list<string>::iterator it2 = listap->begin(); it != listap->end(); it2++)
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

	return 0;
}
