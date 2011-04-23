/*
 * Util.h
 *
 *  Created on: 27/03/2011
 *      Author: pablo
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <list>
#include <sys/types.h>
#include <dirent.h>
#include "Parametros.h"
#define RUTA_ID "path_id"


using namespace std;

class Util {
public:
	/*
	 * Retorna @cadena en mayusculas
	 */
	static string toUpper(string cadena);

	/*
	 * Retorna @cadena en minusculas
	 */
	static string toLower (string cadena);

	/*
	 * Retorna @cadena sin espacios
	 * al principo ni al final
	 */
	static string trim(string cadena);

	/*
	 * Retorna @cadena sin tilde
	 */

	static string sinTilde(string cadena);

	/*
	 * Retorna la lista de archivos que hay en
	 * @carpeta
	 */
	static list<string>* getArchivos(string carpeta);


	/*
	 * Retorna la lista de carpetas que hay en
	 * @carpeta
	 */
	static list<string>* getCarpetas(string carpeta);

	/*
	 * Retorna true si el @archivo existe
	 */
	static bool existeArchivo(string archivo);

	/*
	 *Retorna un id nuevo para un libro
	 */
	static unsigned int generarIdNuevo();

	/*
	 *Reinicia el contador de ids
	 *ojo por que rompe todo
	 */
	static void reiniciarId(unsigned int unId);

	/*
	 * Retorna el numero en formato
	 * string
	 */
	static string DoubleToString(double numero);


	/*
	 * Retorna el numero en formato
	 * string
	 */
	static string UIntToString(unsigned int numero);



private:
	static string convertir(char letra);
};

#endif /* UTIL_H_ */
