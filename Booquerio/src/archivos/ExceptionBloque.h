/*
 * ExceptionBloque.h
 *
 *  Created on: 08/04/2011
 *      Author: martin
 */

#ifndef EXCEPTIONBLOQUE_H_
#define EXCEPTIONBLOQUE_H_
#include <iostream>
#include <cstdlib>
#include <exception>

using namespace std;

class ExceptionBloque : public exception {
public:
	ExceptionBloque();
	const char* what() const throw();
};

#endif /* EXCEPTIONBLOQUE_H_ */
