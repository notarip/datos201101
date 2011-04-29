/*
 * ExceptionDuplicacionClaves.h
 *
 *  Created on: 29/04/2011
 *      Author: martin
 */

#ifndef EXCEPTIONDUPLICACIONCLAVES_H_
#define EXCEPTIONDUPLICACIONCLAVES_H_

#include <iostream>
#include <cstdlib>
#include <exception>

using namespace std;

class ExceptionDuplicacionClaves : public exception {
public:
	ExceptionDuplicacionClaves();
	const char* what() const throw();

};

#endif /* EXCEPTIONDUPLICACIONCLAVES_H_ */
