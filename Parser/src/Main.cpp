

#include <iostream>
#include "util/Parametros.h"
#include "parser/Parser.h"

int main (int argc, char** argv)
{

	/*
	 * PRUEBAS DE PARSEO DELA ARCHIVO DE PROPIEDADES
	 */
	cout << Parametros().getParametro("path_datos").c_str() << endl;

	cout << Parametros().getParametro("nada").c_str() << endl;

	cout << Parametros().getParametro("P1").c_str() << endl;
	/*
	 * PRUEBAS DE PARSEO DELA ARCHIVO DE PROPIEDADES
	 */

	/*
	 * PRUEBA DEL PARSEO DEL ARCHIVO DE STOP WORDS
	 */
	Parser *parser = new Parser();

	parser->listarStopWords();

	parser->~Parser();
	/*
	 * PRUEBA DEL PARSEO DEL ARCHIVO DE STOP WORDS
	 */

	/*
	 * PRUEBA DEL PARSEO DEL LIBRO
	 */
	Parser *parser2 = new Parser();

	parser2->parsear("./archivos/libros/libro1.txt");

	parser2->listarLibro();

	parser2->~Parser();

	/*
	 * PRUEBA DEL PARSEO DEL LIBRO
	 */


	return 0;
}
