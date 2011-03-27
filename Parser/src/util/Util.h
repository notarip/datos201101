/*
 * Util.h
 *
 *  Created on: 27/03/2011
 *      Author: pablo
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string>
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
	 * Retorna @cadena sin tilde
	 */
	static string sinTilde(string cadena);

};

#endif /* UTIL_H_ */
