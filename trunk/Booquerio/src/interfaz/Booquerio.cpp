/*
 * Booquerio.cpp
 *
 *  Created on: 03/04/2011
 *      Author: pablo
 *      Funcionalidad
 * Archivo de configuración (con directorio de almacenamiento)
 * Tomar Texto: ./ejecutable -i "archivo de texto"
 * Procesar Editorial: ./ejecutable -e (procesa los no procesados)
 * Procesar Autor: ./ejecutable –a
 * Procesa Título: ./ejecutable –t
 * Procesa Palabras: ./ejecutable –p
 * Listar Archivos Tomados: ./ejecutable –l (muestra identificador, Título, Autor,
 * Editorial y cantidad de palabras registradas para ese libro).
 * Obtener Archivo: ./ejecutable -o ID_Archivo
 * Quita Archivo: ./ejecutable -q ID_Archivo (se elminan las entradas en los
 * otros índices)
 * Ver Estructura: Genera archivos en forma de texto plano, que describen las
 * estructuras y contenidos de los archivos de almacenamiento y control del
 * sistema.
 * - ./ejecutable -v [-e árbol de Editorial, -a árbol de Autor, -t hash de
 * Título, -p hash de Palabra] "Nombre Archivo"
 * - Nombre y estructuras para los archivos generados:
 * - Archivo de Estructura de control: "Nombre Archivo"_Índice,
 * "Nombre Archivo"_tabla.
 * - Archivos de control de espacios libres: "Nombre
 * Archivo"_libres.
 * - Archivos de bloques de datos: "Nombre Archivo"_datos.
 * - Estructura: para árboles la indicada en teórica. Separadores:
 * Bloques con "|", Registros con ";", atributos con ",".
 *
 */


#define ERROR_DE_LLAMADA -10
#define TOMAR_TEXTO 10
#define PROCESAR_EDITORIAL 20
#define PROCESAR_AUTOR 30
#define PROCESAR_TITULO 40
#define PROCESAR_PALABRAS 50
#define LISTAR_ARCHIVOS_TOMADOS 60
#define OBTENER_ARCHIVO 70
#define QUITAR_ARCHIVO 80
#define VER_ESTRUCTURA_EDITORIAL 90
#define VER_ESTRUCTURA_AUTOR 91
#define VER_ESTRUCTURA_TITULO 92
#define VER_ESTRUCTURA_PALABRA 93


#include <iostream>
#include "../servicios/Servicios.h"

using namespace std;


void error_de_llamada()
{

	cout << "No se reconoce el comando, ¿ necesita ayuda ?: "  << endl;
	cout << "Tomar Texto: ./ejecutable -i \"archivo de texto\"" <<endl;
	cout << "Procesar Editorial: ./ejecutable -e (procesa los no procesados)"<<endl;
	cout << "Procesar Autor: ./ejecutable –a" << endl;
	cout << "Procesa Título: ./ejecutable –t" << endl;
	cout << "Procesa Palabras: ./ejecutable –p" << endl;
	cout << "Listar Archivos Tomados: ./ejecutable –l (muestra identificador, Título, Autor," << endl;
	cout <<	" Editorial y cantidad de palabras registradas para ese libro)." << endl;
	cout << "Obtener Archivo: ./ejecutable -o ID_Archivo" << endl;
	cout << "Quita Archivo: ./ejecutable -q ID_Archivo (se elminan las entradas en los otros indices)" << endl;
	cout << "Ver Estructura: ./ejecutable -v [-e árbol de Editorial, -a árbol de Autor, -t hash de" << endl;
	cout << "Título, -p hash de Palabra] \"Nombre Archivo\"" << endl;

}

int determinar_operacion(int cant_parm, char** parm, string *parametro)
{

	if (cant_parm > 4 || cant_parm < 2)
		return ERROR_DE_LLAMADA;

	if (parm[1][1] == 'i')
	{
		parametro->append(parm[2]);
		return TOMAR_TEXTO;
	}
	if (parm[1][1] == 'e')
		return PROCESAR_EDITORIAL;

	if (parm[1][1] == 'a')
		return PROCESAR_AUTOR;

	if (parm[1][1] == 't')
		return PROCESAR_TITULO;

	if (parm[1][1] == 'p')
		return PROCESAR_PALABRAS;

	if (parm[1][1] == 'l')
		return LISTAR_ARCHIVOS_TOMADOS;

	if (parm[1][1] == 'o')
	{
		parametro->append(parm[2]);
		return OBTENER_ARCHIVO;
	}

	if (parm[1][1] == 'q')
	{
		parametro->append(parm[2]);
		return QUITAR_ARCHIVO;
	}


	if (parm[1][1] == 'v')
	{
		if (parm[2][1] == 'a')
			return VER_ESTRUCTURA_AUTOR;
		if (parm[2][1] == 't')
			return VER_ESTRUCTURA_TITULO;
		if (parm[2][1] == 'e')
			return VER_ESTRUCTURA_EDITORIAL;
		if (parm[2][1] == 'p')
			return VER_ESTRUCTURA_PALABRA;
	}

	return ERROR_DE_LLAMADA;
}


int main (int argc, char** argv)
{

	string parametro;
	int operacion =	determinar_operacion(argc, argv, &parametro);

	switch(operacion)
	{
		case TOMAR_TEXTO:Servicios().tomarTexto(parametro);break;
		case PROCESAR_AUTOR:Servicios().procesarAutores();break;
		case PROCESAR_EDITORIAL:Servicios().procesarEditoriales();break;
		case PROCESAR_TITULO:Servicios().procesarTitulos();break;
		case PROCESAR_PALABRAS:Servicios().procesarPalabras();break;
		case LISTAR_ARCHIVOS_TOMADOS:/*llamada al servicio que lista los libros tomados*/;break;
		case OBTENER_ARCHIVO:/*llamada al servicio que devuelve un libro*/;break;
		case QUITAR_ARCHIVO:/*llamada al servicio que saca un libro*/;break;
		case VER_ESTRUCTURA_AUTOR:/*llamada al servicio que muestra estructuras autor*/;break;
		case VER_ESTRUCTURA_EDITORIAL:/*llamada al servicio que muestra estructuras autor*/;break;
		case VER_ESTRUCTURA_PALABRA:/*llamada al servicio que muestra estructuras autor*/;break;
		case VER_ESTRUCTURA_TITULO:/*llamada al servicio que muestra estructuras autor*/;break;
		case ERROR_DE_LLAMADA:error_de_llamada();break;
		default:error_de_llamada();break;
	}

	return 0;

}
