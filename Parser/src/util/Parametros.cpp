/*
 * Parametros.cpp
 *
 *  Created on: 25/03/2011
 *      Author: pablo
 */

#include "Parametros.h"


string Parametros::getParametro(string idParametro)
{
        fstream archivo;
        string parametro = "";
        archivo.open("./config.propiedades", ios::in | ios::binary);
        if (archivo.is_open())
        {
                parametro =buscarParametro(&archivo,idParametro);
                archivo.close();
        }

        return parametro;
}


string Parametros::buscarParametro(std::fstream *archivo, string idParametro)
{
        string linea;
        string salida = "";
        bool match = false;
        int tamanio;

        archivo->seekg(0, ios::end);
        tamanio = archivo->tellg();
        archivo->seekg(0);

        char* buff = new char[tamanio];

        archivo->read(buff, tamanio);
        linea.assign(buff);

        while (!match)
        {

			unsigned int posId = linea.find(idParametro);
			if (posId != string::npos)
			{
					match = true;
					unsigned int pos = linea.find("=", posId);
					if (pos != string::npos)
					{
							unsigned int pos2 = linea.find(";",pos);
							if (pos2 != string::npos)
									salida  = linea.substr(pos+1, pos2-pos-1);
					}
			}
        }

        delete [] buff;

        return salida;
}
