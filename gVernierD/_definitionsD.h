/*
||	Archivo de definiciones para la clase gVernierD.
||
||	Este archivo contiene definiciones con valores por defecto para los diferentes 
||	sensores.
*/


	//ID´s
#define _gVERNIERD_GO_MOTION      1     //Detector de movimiento "GO"
#define _gVERNIERD_MOTION         2		//Detector de movimiento
#define _gVERNIERD_N_MOTION       3		//"Motion not used"
#define _gVERNIERD_GATE           4		//Foto compuerta 
#define _gVERNIERD_DROP           5		//Cuanta gotas
#define _gVERNIERD_ROTARY         6		//Sensor de rotacion
#define _gVERNIERD_RADIATION      7		//Monitor de radiacion
#define _gVERNIERD_DCU1           8 	//"Digital Control Unit" de primera generacion
#define _gVERNIERD_DCU2           9 	//"Digital Control Unit" de segunda generacion
#define _gVERNIERD_POLARIMETER    10	//Polarimetro
#define _gVERNIERD_PROJECTILE     11	//Estacion de lanzamiento de proyectiles 
#define _gVERNIERD_DAK_CARRIAGE   12	//Riel del aparato de difraccion
#define _gVERNIERD_MOTION_ENCODER 13	//Decodificador de movimiento
#define _gVERNIERD_POSIBLE        14	//Posible sensor digital

	//Velocidad del sonido en m/s en condiciones normales 
#define _gVERNIERD_SOUND_SPEED    343.0	//Velocidad del sonido en condiciones normales (0% de humedad, 20 grados centigrados)


	//Variables de los filtros de Kalman
#define _gVERNIERD_DEFAULT_GAIN		    1.0
#define _gVERNIERD_GO_MOTION_GAIN       0.3814872



























