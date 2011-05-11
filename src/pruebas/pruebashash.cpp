#include "../hash/Hash.h"
#include "math.h"
#include <iostream>

static void f(Hash* hash,unsigned int cant){
	Registro* reg;
	for (unsigned int i=0;i<cant;i++){
		stringstream a;
		unsigned int b= i;
		a<<b;
		string str;
		a>>str;
		reg=new Registro(str);
		reg->agregarAtribEntero(i);
		hash->insertar(reg);
		delete reg;

	}
}

static void f2(Hash* hash,unsigned int cant){
	for (unsigned int i=0;i<cant;i++){
		stringstream a;

		unsigned int b=i;
		a<<b;
		string str;
		a>>str;
		hash->borrar(str);
	}
}


int main323(){
	Hash hash("unhash");
	/*
	char c=' ';
	string str;
	while(c!='q'){


		cout<<"0. insertar registros aleatoriamente."<<endl;
		cout<<"1. eliminar un registro."<<endl;
		cout<<"2. insertar un registro nuevo."<<endl;
		cout<<"3. buscar un registro por su clave."<<endl;
		cout<<"4. elimino."<<endl;
		cin>>c;
		switch(c){
			case '0':{ cout<<"ingrese la cantidad de registros a insertar:";
					  unsigned int cant;
					  cin>>cant;
					  f(&hash,cant);
					  break;
			}
 			case '1':{ cout<<"ingrese el Id del registro a eliminar: ";
					  cin>>str;
					  hash.borrar(str);
					  break;
 			}
			case '2':{ cout<<" ingrese el Id del registro a insertar:";
					  cin>>str;
					  Registro* reg=new Registro(str);
					  hash.insertar(reg);
					  break;
			}
			case '3':{ cout<<" ingrese el Id del registro a buscar:";
					  cin>>str;
					  Registro* reg=hash.buscar(str);
					  if (reg!=NULL) cout<<reg->getString()<<endl;
					  break;
			}
			case '4':{ cout<<" borro aleat.";
					  unsigned int str;
					  cin>>str;
					  f2(&hash,str);
					  break;
			}
		    case 'q':{ break; }

			default:{ cout<<"incorrecto."<<endl;
					 break;}
		}
		cout<<endl;cout<<endl;
		hash.mostrar();

	}
	hash.mostrar2("res.txt");*/
	f(&hash,20000);
	hash.mostrar();
}



