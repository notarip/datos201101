/*
 * Parser.cpp
 *
 *  Created on: 26/03/2011
 *      Author: pablo
 */

#include "Parser.h"

Parser::Parser() {

	fstream archStopW;
	string path_stopW = Parametros().getParametro(TAG_PATH_STOP_WORDS);

	archStopW.open(path_stopW.c_str(), ios::binary | ios::in);
	if (archStopW.is_open())
	{
		levantarStopWords(&archStopW);
		archStopW.close();
	}

}

Parser::~Parser() {

	stopWords.clear();

}

int Parser::parsear(string archivo)
{

	fstream archLibro;

	archLibro.open(archivo.c_str(), ios::binary | ios::in);
	if (archLibro.is_open())
	{
		procesarLibro(&archLibro);
		archLibro.close();

	}
	return ARCHIVO_INVALIDO;
}


void Parser::procesarLibro(fstream *archLibro)
{

    string linea;
    int tamanio;

	archLibro->seekg(0, ios::end);
	tamanio = archLibro->tellg();
	archLibro->seekg(0);

	char* buff = new char[tamanio];
	archLibro->read(buff, tamanio);
	linea.assign(buff);

	unsigned int posIni = 0;
	unsigned int posTag;
	unsigned int posFin;

	//levanta el autor
	posTag = linea.find("AUTOR:",posIni);
	posTag += 6; //lenght de AUTOR:
	posFin = linea.find('\n',posTag);
	this->autor = linea.substr(posTag, posFin - posTag);
	posIni = posFin + 1;

	//levanta la editorial
	posTag = linea.find("EDITORIAL:",posIni);
	posTag += 10; //lenght de EDITORIAL:
	posFin = linea.find('\n',posTag);
	this->editorial = linea.substr(posTag, posFin - posTag);
	posIni = posFin + 1;

	//levanta el titulo
	posTag = linea.find("TITULO:",posIni);
	posTag += 7; //lenght de TITULO:
	posFin = linea.find('\n',posTag);
	this->titulo = linea.substr(posTag, posFin - posTag);
	posIni = posFin + 1;

	//levanta texto
	posFin = linea.find('\0',posTag);
	this->texto = linea.substr(posIni, posFin - posIni);

    delete [] buff;
}



list<string> *Parser::procesarPalabras()
{
	list<string> *palabras = new list<string>();

//	unsigned int posIni = 0;
	//unsigned int posFin;






	return palabras;
}


void Parser::levantarStopWords(fstream *archStopW)
{

    string linea;
    int tamanio;

    archStopW->seekg(0, ios::end);
    tamanio = archStopW->tellg();
    archStopW->seekg(0);

    char* buff = new char[tamanio];
    archStopW->read(buff, tamanio);
    linea.assign(buff);


    unsigned int posIni = 0;
    unsigned int posFin;
    bool fin = false;
    string stopWord;

    while (!fin)
    {
		posFin = linea.find(',',posIni);
		if (posFin == string::npos) //no encontro la coma (caso de ultima palabra)
		{
			posFin = linea.find('\n',posIni);
			fin = true;
		}

		stopWord = linea.substr(posIni, posFin - posIni); //string a almacenar
		this->stopWords.push_back(stopWord);
		posIni = posFin + 1;
    }

    delete [] buff;

}


void Parser::listarStopWords()
{
	/* PARA PROBAR LA LISTA DE STOP WORDS*/

	for (list<string>::iterator it = stopWords.begin(); it != stopWords.end(); it++)
		cout << (*it).c_str() << endl;

}


void Parser::listarLibro()
{
	cout << this->autor << endl;
	cout << this->editorial << endl;
	cout << this->titulo << endl;
	cout << this->texto << endl;

}
