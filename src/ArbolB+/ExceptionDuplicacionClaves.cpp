/*
 * ExceptionDuplicacionClaves.cpp
 *
 *  Created on: 29/04/2011
 *      Author: martin
 */

#include "ExceptionDuplicacionClaves.h"

ExceptionDuplicacionClaves::ExceptionDuplicacionClaves() {
	// TODO Auto-generated constructor stub

}

const char* ExceptionDuplicacionClaves::what() const throw(){

	return "Error: Se insertar en el arbol una clave que ya existia";
}
