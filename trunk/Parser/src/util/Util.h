/*
 * Util.h
 *
 *  Created on: 27/03/2011
 *      Author: pablo
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <list>
#include <sys/types.h>
#include <dirent.h>



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

};

#endif /* UTIL_H_ */
