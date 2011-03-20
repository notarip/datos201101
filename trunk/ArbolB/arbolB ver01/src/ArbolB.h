/*
 * ArbolB.h
 *
 *  Created on: 18/03/2011
 *      Author: pablo
 */

#ifndef ARBOLB_H_
#define ARBOLB_H_

#include <string>
#include <iostream>
#include <fstream>

#include "Definiciones.h"
#include "Elemento.h"
#include "Nodo.h"
using namespace std;

#define TN 16 //el tamaño en bytes del nodo

class ArbolB {

public:
	ArbolB();
	virtual ~ArbolB();
	/*Crea un ArbolB vacio inicializa una raiz vacia y la almacena en un archivo
	* nuevo, creando tambien un archivo de nodos libres vacios*/
	void Crear();
	/*Abre el archivo de almacenamiento del arbol y el de referencia
	 * a nodos libres y lee la raiz */
	void Abrir();
	/*Cierra los archivos componentes del arbol*/
	void Cerrar();
	/*Inserta un elemento en el arbol*/
	void Insertar(Elemento elemento, string resultado);
	/*si @resultado = "inexistente" en elemento devuelve el siguiente que
	 * encuentre o el ultimo del arbol en el caso que la clave sea mayor
	 * al ultimo elemento del arbol"*/
	void Buscar(TClave clave, Elemento elemento, string resultado);
	/*Elimina el elemento identificado por @clave o devuelve
	 * @resultado = "inexsistente" si no existe*/
	void Suprimir(TClave clave,string resultado);
	/*Muestra en pantalla el contenido del nodo raiz por defecto y permite
	 * solicitar que se muestre el contenido de otros nodos especeficando
	 * numeros relativos en el archivo de nodos, solo muestra claves de los
	 * elementos*/
	void Visualizar();
	/*Exporta el arbol en pre order a un archivo de texto*/
	void Exportar();

private:
	unsigned int CI;
	unsigned int MitadDerCI;
	unsigned int MitadIzqCI;
	unsigned int CH;
	unsigned int MitadDerCH;
	unsigned int MitadIzqCH;
	unsigned int CR;
	fstream nodos;
	fstream libres;
	Nodo raiz;
};

#endif /* ARBOLB_H_ */
