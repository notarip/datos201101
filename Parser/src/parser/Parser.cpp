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
		levantarPalabras(&archStopW);
		archStopW.close();
	}

}

Parser::~Parser() {

	stopWords.clear();

}

int Parser::parsear(string archivo)
{

	return ARCHIVO_INVALIDO;
}

void Parser::levantarPalabras(fstream *archStopW)
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
