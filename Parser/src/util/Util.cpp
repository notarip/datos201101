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


string Util::trim(string cadena)
{

	//borra los del principio
	while (cadena.size() > 0 && cadena.at(0) == ' ')
		cadena.erase(0,1);

	//borra los del final
	while (cadena.size() > 0 && cadena.at(cadena.length()-1) == ' ')
		cadena.erase(cadena.length()-1,1);

	return cadena;

}

string Util::sinTilde(string cadena)
{


	string invalidos("áéíóú");
	string parejas("áaéeíióoúu");
	string reemp;


	unsigned int pos = cadena.find_first_of(invalidos);
	while (pos != string::npos)
	{
		pos = cadena.find_first_of(invalidos, pos+1);
	}

	return cadena;
}


list<string>* Util::getArchivos(string carpeta)
{

	DIR *directorio;
	struct dirent *elemento;
	list<string> *archivos = new list<string>();

	directorio = opendir(carpeta.c_str());

	while (elemento = readdir(directorio))
	{
		if (elemento->d_type == DT_REG)
			archivos->push_back(elemento->d_name);
	}

	return archivos;

}

list<string>* Util::getCarpetas(string carpeta)
{
	DIR *directorio;
	struct dirent *elemento;
	list<string> *carpetas = new list<string>();

	directorio = opendir(carpeta.c_str());

	while (elemento = readdir(directorio))
	{
		if (elemento->d_type == DT_DIR)
			carpetas->push_back(elemento->d_name);
	}

	return carpetas;

}

