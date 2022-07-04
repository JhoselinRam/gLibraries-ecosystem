/*
||	Clase gVernierD
||	
||	Autor: Jhoselin Adrian Ramirez Montes
||	Contacto: jhoselin.ramirez92@gmail.com
||
||	Descripcion: Esta clase esta diseñada para ser utilizada con los sensores VERNIER
||		digitales estandar que cuenten con entrada BTD (British Telecom Digital).
||		Provee auto-identificacion del sensor conectado y ajusta los parametros necesarios
||		para su utilizacion, ademas implementa un filtro de Kalman unidimensional para 
||		suavizar el ruido en las mediciones.
||
||	Se ha usado parte de la libreria VernierLib proporcionada por VERNIER para la
||	realizacion de esta clase.
*/

#ifndef __gVERNIERD__
#define __gVERNIERD__

#include<Arduino.h>
#include<_definitionsD.h>

class gVernierD{
	public:
		gVernierD(int DIO0, int DIO1, int DIO2, int DIO3);
		void  begin();
		float read();
		float readRaw();
		int   id();
		
			//Funciones auxiliares, su uso depende del tipo de sensor
		void setSoundSpeed(float speed);
	
	private:
		int   _dio0;        //Pin DIO0 del BTD
		int   _dio1;        //Pin DIO1 del BTD
		int   _dio2;        //Pin DIO2 del BTD
		int   _dio3;        //Pin DIO3 del BTD
		int   _id;          //ID del sensor
		float _data;		//Lectura del sensor
		float _kGain;		//Ganancia del filtro kalman
		float _aux1;		//Variable auxiliar, el uso depende sel sensor
		
		float (gVernierD::*_read)();  //Funcion de lectura de datos, es una diferente para cada sensor
		float _readMotion();		  //Version de la funcion de lectura de datos para el sensor de movimiento
		
		float _readDEFAULT(){return 0;}  //Version por default de la funcion de lectura de datos, retorna cero siempre
};

#endif