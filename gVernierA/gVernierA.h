/*
||	Clase gVernierA
||	
||	Autor: Jhoselin Adrian Ramirez Montes
||	Contacto: jhoselin.ramirez92@gmail.com
||
||	Descripcion: Esta clase esta diseñada para ser utilizada con los sensores VERNIER
||		analogicos estandar que cuenten con entrada BTA (British Telecom Analog).
||		Provee auto-identificacion del sensor conectado y ajusta los parametros necesarios
||		para su utilizacion, ademas implementa un filtro de Kalman unidimensional para 
||		suavizar el ruido en las mediciones.
||
||	Se ha usado parte de la libreria VernierLib proporcionada por VERNIER para la
||	realizacion de esta clase.
*/

#ifndef __gVERNIERA__
#define __gVERNIERA__

#include<Arduino.h>
#include<math.h>
#include<gI2C.h>
#include<_definitionsA.h>

class gVernierA{
	public:
		gVernierA(int SIG1, int SIG2);
		void  begin();
		float read();
		float readRaw();
		int   id();
	
	private:
		int   _sig1;        //Pin SIG1 del BTA
		int   _sig2;        //Pin SIG2 del BTA
		int   _id;          //ID del sensor
		float _data;		//Lectura del sensor
		float _slope;		//Pendiente de la funcion de calibracion
		float _intercept;	//Ordenada al origen de la funcion de calibracion
		float _Coeff2;      //Coeficiente cuadratico de algunas funciones de calibracion
		float _kGain;		//Ganancia del filtro kalman
		
		float (gVernierA::*_calFunction)(float voltage);	    //Funcion de calibracion
		float _linear(float voltage);							//Variante lineal de la funcion de calibracion
		float _cuadratic(float voltage);						//Variante cuadratica
		float _exponential(float voltage);						//Variante exponencial
		float _logarithmic(float voltage);						//Variante logaritmica		
	
};

#endif