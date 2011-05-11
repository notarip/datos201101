/*
 * ExceptionBloque.cpp
 *
 *  Created on: 08/04/2011
 *      Author: martin
 */

#include "ExceptionBloque.h"

ExceptionBloque::ExceptionBloque() {
	// TODO Auto-generated constructor stub

}
const char* ExceptionBloque::what() const throw(){

	return "Error: Se quizo grabar un bloque de mayor tamaño que el permitido";
}

