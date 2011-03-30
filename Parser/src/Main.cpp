

#include <iostream>



#include "util/Parametros.h"
#include "parser/Parser.h"


int main (int argc, char** argv)
{

	/*
	 * PRUEBAS DE PARSEO DELA ARCHIVO DE PROPIEDADES
	 */
	bool prueba1 = false;
	bool prueba2 = false;
	bool prueba3 = false;
	bool prueba4 = false;
	bool prueba5 = true;

	if (prueba1)
	{
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

		parser2->listarLibro();

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

		list<string> *archivos= Util().getCarpetas(".");

		for (list<string>::iterator it= archivos->begin(); it != archivos->end();it++)
			cout << (*it).c_str() << endl;

		archivos->clear();
		delete archivos;
	}


	return 0;
}
