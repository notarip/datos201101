

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


	return 0;
}
