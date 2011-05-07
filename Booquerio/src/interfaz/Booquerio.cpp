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
#define SUCCESS	0





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
#define VER_ESTRUCTURA_PRIMARIO 94
#define AGREGAR_VARIOS 95


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
	cout << "Título, -p hash de Palabra -i indice primario] \"Nombre Archivo\"" << endl;

}

void error_archivo_listas()
{
	cout << "Error en la configuración: no se hallo el archivo de listas." << endl;
}

void error_ruta_libro_invalida()
{
	cout << "Error en ruta: el nombre del archivo es invalido o no hay permisos sobre el directorio." << endl;
}

void error_ruta_archivo_libros()
{
	cout << "Error en la configuración: no se hallo el archivo que contiene los libros." << endl;
}

void error_ruta_b_mas_primario()
{
	cout << "Error en la configuración: no se hallo el archivo que contiene el arbol primario." << endl;
}

void error_libro_inexsistente()
{
	cout << "Error: el libro que se quiere eliminar es inexistente" << endl;
}

void sin_errores()
{
	cout << "Satisfactorio." << endl;
}

void procesamiento_de_errores(int error)
{
	//armar un case de errores con los mensajes

	switch(error)
	{
		case SUCCESS : sin_errores();break;
		case ERROR_DE_LLAMADA: error_de_llamada();break;
		case ERROR_ARCHIVO_LISTAS: error_archivo_listas();break;
		case ERROR_RUTA_INVALIDA:error_ruta_libro_invalida();break;
		case ERROR_RUTA_ARCHIVO_LIBROS:error_ruta_archivo_libros();break;
		case ERROR_RUTA_BMAS_PRIMARIO:error_ruta_b_mas_primario();break;
		case ERROR_LIBRO_INEXISTENTE:error_libro_inexsistente();break;
	}
}


int determinar_operacion(int cant_parm, char** parm, string *parametro)
{

	if (cant_parm > 5 || cant_parm < 2)
		return ERROR_DE_LLAMADA;

	if (parm[1][1] == 'i')
	{
		if (cant_parm == 3) parametro->append(parm[2]);
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
		if (cant_parm == 3) parametro->append(parm[2]);
		return OBTENER_ARCHIVO;
	}

	if (parm[1][1] == 'q')
	{
		parametro->append(parm[2]);
		return QUITAR_ARCHIVO;
	}

	if (parm[1][1] == 'z')
	{
		if (cant_parm == 3) parametro->append(parm[2]);
		return AGREGAR_VARIOS;
	}


	if (parm[1][1] == 'v')
	{
		if (parm[2][1] == 'a')
		{
			if (cant_parm == 4)  parametro->append(parm[3]);
			return VER_ESTRUCTURA_AUTOR;
		}
		if (parm[2][1] == 't')
		{
			if (cant_parm == 4)  parametro->append(parm[3]);
			return VER_ESTRUCTURA_TITULO;
		}
		if (parm[2][1] == 'e')
		{
			if (cant_parm == 4)  parametro->append(parm[3]);
			return VER_ESTRUCTURA_EDITORIAL;
		}
		if (parm[2][1] == 'p')
		{
			if (cant_parm == 4)  parametro->append(parm[3]);
			return VER_ESTRUCTURA_PALABRA;
		}
		if (parm[2][1] == 'i')
		{
			if (cant_parm == 4)  parametro->append(parm[3]);
			return VER_ESTRUCTURA_PRIMARIO;
		}
	}

	return ERROR_DE_LLAMADA;
}



int main (int argc, char** argv)
{

	string parametro;
	int operacion =	determinar_operacion(argc, argv, &parametro);
	int error = 0;

	switch(operacion)
	{
		case TOMAR_TEXTO:error = Servicios().tomarTexto(parametro);break;
		case PROCESAR_AUTOR:error = Servicios().procesarAutores();break;
		case PROCESAR_EDITORIAL:error = Servicios().procesarEditoriales();break;
		case PROCESAR_TITULO:error = Servicios().procesarTitulos();break;
		case PROCESAR_PALABRAS:error = Servicios().procesarPalabras();break;
		case LISTAR_ARCHIVOS_TOMADOS:error = Servicios().listarLibros();break;
		case OBTENER_ARCHIVO:error = Servicios().obtenerLibro(parametro);break;
		case QUITAR_ARCHIVO:error = Servicios().quitarArchivo(parametro);break;
		case VER_ESTRUCTURA_AUTOR:error = Servicios().verEstructuraAutor(parametro);break;
		case VER_ESTRUCTURA_EDITORIAL:error = Servicios().verEstructuraEditorial(parametro);break;
		case VER_ESTRUCTURA_TITULO:error = Servicios().verEstructuraTitulos(parametro);break;
		case VER_ESTRUCTURA_PALABRA:error = Servicios().verEstructuraPalabras(parametro);break;
		case VER_ESTRUCTURA_PRIMARIO:error = Servicios().verEstructuraPrimario(parametro);break;
		case AGREGAR_VARIOS:error = Servicios().agregarVarios(parametro);break;
		case ERROR_DE_LLAMADA:error = ERROR_DE_LLAMADA;break;
		default:error = ERROR_DE_LLAMADA;break;
	}
	procesamiento_de_errores(error);

	return 0;

}
