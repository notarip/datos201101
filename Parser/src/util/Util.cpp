/*
 * Util.cpp
 *
 *  Created on: 27/03/2011
 *      Author: pablo
 */

#include "Util.h"

string Util::toUpper(string cadena)
{

    string cadenaUpper;

    for ( unsigned int i = 0 ; i < cadena.size() ; i++ ) {

        char letraActual = (char)toupper(cadena[i]);
        cadenaUpper += letraActual;
    }
    return cadenaUpper;
}

string Util::toLower(string cadena)
{
    string cadenaLower;

    for ( unsigned int i = 0 ; i < cadena.size() ; i++ ) {

        char letraActual = (char)tolower((cadena[i]));
        cadenaLower += letraActual;
    }
    return cadenaLower;
}

string Util::sinTilde(string cadena)
{


	string invalidos("áéíóú");
	string parejas("áaéeíióoúu");
	string reemp;
	int i = 0;


	unsigned int pos = cadena.find_first_of(invalidos);
	while (pos != string::npos)
	{
		pos = cadena.find_first_of(invalidos, pos+1);
	}




	return cadena;
}

