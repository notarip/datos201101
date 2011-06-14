/*
 * pruebaProximidad.cpp
 *
 *  Created on: 14/06/2011
 *      Author: rjc
 */

#include "../ProcesadorConsultas/procesadorConsultas.h"

//int main45(){
//	unsigned int cantidadTerminos =4;
//	list<unsigned int> vectorListas[cantidadTerminos];
//	list<unsigned int>::iterator vectorIteradores[cantidadTerminos];
//
//	vectorListas[0].push_back(17);
//	vectorListas[0].push_back(96);
//	vectorListas[0].push_back(12);
//	vectorListas[0].push_back(79);
//	vectorListas[0].push_back(44);
//
//	vectorListas[1].push_back(18);
//	vectorListas[1].push_back(15);
//	vectorListas[1].push_back(46);
//	vectorListas[1].push_back(58);
//
//	vectorListas[2].push_back(54);
//	vectorListas[2].push_back(61);
//	vectorListas[2].push_back(3);
//	vectorListas[2].push_back(83);
//	vectorListas[2].push_back(21);
//
//	vectorListas[3].push_back(99);
//	vectorListas[3].push_back(87);
//	vectorListas[3].push_back(69);
//	vectorListas[3].push_back(5);
//	vectorListas[3].push_back(30);
//	vectorListas[3].push_back(52);
//
//	vectorListas[3].sort();
//	vectorListas[2].sort();
//	vectorListas[1].sort();
//	vectorListas[0].sort();
//
//	for ( unsigned int i = 0 ; i< cantidadTerminos ; i++){
//			vectorIteradores[i] = vectorListas[i].begin();
//		}
//
//		// voy a ir calculando secuencias optimas para cada unos de las posicionse
//		// del ultimo termino
//
//		float pesoDocumento = 0;
//
//		while ( vectorIteradores[cantidadTerminos-1] != vectorListas[cantidadTerminos-1].end()){
//			list<unsigned int> unaSecuencia;
//			unaSecuencia.push_back(*vectorIteradores[cantidadTerminos-1]);
//			list<unsigned int> secOptima;
//
//			ProcesadorConsultas().ArmarSecuenciaOptima(unaSecuencia,&secOptima,cantidadTerminos,vectorListas);
//
//			if (secOptima.size() !=0 ){
//				pesoDocumento += 1 / pow(ProcesadorConsultas().obtenerSeparacion(secOptima),2);
//
//				//limpiar listas
//
//				for(unsigned int j=0; j<cantidadTerminos-1; j++){
//					while(vectorListas[j].front()<=(*vectorIteradores[cantidadTerminos-1])){
//						vectorListas[j].pop_front();
//					}
//				}
//			}
//			vectorIteradores[cantidadTerminos-1]++;
//			list<unsigned int>::iterator itImpresion=secOptima.begin();
//			while(itImpresion!=secOptima.end()){
//				cout<<*itImpresion<<"-";
//				itImpresion++;
//			}
//			cout<<endl;
//		}
//
//}
