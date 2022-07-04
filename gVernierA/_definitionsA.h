/*
||	Archivo de definiciones para la clase gVernierA.
||
||	Este archivo contiene definiciones con valores por defecto para los diferentes 
||	sensores.
*/

	//Pines definidos del BTA
#define _gVERNIERA_Vres		A4
#define _gVERNIERA_ID		A5

	//ID´s
#define _gVERNIERA_TERMOCOUPLE    1		//Termopar
#define _gVERNIERA_VOLTAGE10      2		//Voltaje +/- 10V
#define _gVERNIERA_TI_CURRENT     3		//Sensor de corriente Texas Instruments
#define _gVERNIERA_RESISTANCE     4		//Resistencia
#define _gVERNIERA_XL_TEMEPRATURE 5		//Sonda de temperatura extra larga
#define _gVERNIERA_DIFF_VOLTAGE   8		//Diferencia de voltaje
#define _gVERNIERA_CURRENT        9		//Sensor de corriente
#define _gVERNIERA_SS_TEMPERATURE 10	//Sonda de temperatura de acero inoxidable o superficial
#define _gVERNIERA_VOLTAGE30      11	//Voltaje +/- 30V
#define _gVERNIERA_TI_LIGHT       12	//Sensor de luz Texas Instruments
#define _gVERNIERA_EXR_HEART_RATE 13	//Sensor de frecuencia cardiaca deportivo
#define _gVERNIERA_VOLTAGE_RAW    14	//Voltaje directo
#define _gVERNIERA_EKG            15	//Sensor EKG (Electrocardiografo)
#define _gVERNIERA_CO2            17	//Sensor de gas CO2
#define _gVERNIERA_O2             18	//Sensor de gas O2
#define _gVERNIERA_WR_TEMPERATURE 78    //Sonda de temperatura de amplio rango
#define _gVERNIERA_ETHANOL        97    //Sensor de etanol
#define _gVERNIERA_SOUND          118	//Sensor de nivel de sonido
#define _gVERNIERA_MELT           92	//Temperatura de fundicion
#define _gVERNIERA_ISE_CA         38	//Electrodo selectivo de iones de calcio
#define _gVERNIERA_ISE_NH4        39	//Electrodo selectivo de iones de amonio
#define _gVERNIERA_ISE_NO3        40	//Electrodo selectivo de iones de nitrato
#define _gVERNIERA_ISE_CL         41	//Electrodo selectivo de iones de cloruro
#define _gVERNIERA_ISE_K          113	//Electrodo selectivo de iones de potasio
#define _gVERNIERA_16_TERMOCOUPLE 123   //Nuevo termopar (Octubre 2016) 


	//Definiciones del protocolo I2C
#define _gVERNIERA_ADDRESS         0x50   //Direccion I2C del dispositivo
#define _gVERNIERA_READ_REGISTER_1 0x0	  //Registro de lectura 1
#define _gVERNIERA_READ_REGISTER_2 0x20	  //Registro de lectura 2
#define _gVERNIERA_READ_REGISTER_3 0x40	  //Registro de lectura 3
#define _gVERNIERA_READ_REGISTER_4 0x60	  //Registro de lectura 4
#define _gVERNIERA_ID_INDEX		   2	  //Indice del ID en el buffer I2C
#define _gVERNIERA_CALTIPE_INDEX   57	  //Indice del tipo de calibracion en el buffer I2C
#define _gVERNIERA_PAGE_INDEX      70     //Indice de la pagina en el buffer I2C
#define _gVERNIERA_INTERCEPT_INDEX 71     //Indice de la ordenada al origen del buffer I2C
#define _gVERNIERA_SLOPE_INDEX     75     //Indice de la pendiente del buffer I2C
#define _gVERNIERA_PAGE_MULTIPLIER 19     //Multiplicador de la pagina, util en la lectura de la ordenada al origen y la pendiente



	//Pendientes, ordenadas y factores cuadraticos
#define _gVERNIERA_SLOPE_DEFAULT             1
#define _gVERNIERA_INTERCEPT_DEFAULT         0
#define _gVERNIERA_COEFFICIENT2_DEFAULT	     0
	
#define _gVERNIERA_SLOPE_TERMOCOUPLE        -2.45455
#define _gVERNIERA_INTERCEPT_TERMOCOUPLE     6.2115

#define _gVERNIERA_SLOPE_VOLTAGE10           4
#define _gVERNIERA_INTERCEPT_VOLTAGE10      -10

#define _gVERNIERA_SLOPE_TI_CURRENT         -2.665
#define _gVERNIERA_INTERCEPT_TI_CURRENT      6.325

#define _gVERNIERA_SLOPE_RESISTANCE         -2.5
#define _gVERNIERA_INTERCEPT_RESISTANCE      6.25

#define _gVERNIERA_SLOPE_XL_TEMEPRATURE      58.341
#define _gVERNIERA_INTERCEPT_XL_TEMEPRATURE -53.073

#define _gVERNIERA_SLOPE_DIFF_VOLTAGE       -2.5
#define _gVERNIERA_INTERCEPT_DIFF_VOLTAGE    6.25

#define _gVERNIERA_SLOPE_CURRENT            -0.25
#define _gVERNIERA_INTERCEPT_CURRENT         0.625

#define _gVERNIERA_SLOPE_VOLTAGE30           15.41
#define _gVERNIERA_INTERCEPT_VOLTAGE30      -40.35

#define _gVERNIERA_SLOPE_ISE                 137.55
#define _gVERNIERA_INTERCEPT_ISE            -168.2


	//Constantes de la ecuacion de Steinhart-Hart para la sonda de temperatura
#define _gVERNIERA_K0		           0.00102119	//K_0 de la ecuacion de Steinhart-Hart
#define _gVERNIERA_K1			       0.000222468	//K_1 de la ecuacion de Steinhart-Hart
#define _gVERNIERA_K2			       1.33342E-7	//K_2 de la ecuacion de Steinhart-Hart
#define _gVERNIERA_TERMISTOR_RESISTOR  15000.0		//Valor en ohms de la resistencia en serie con el termistor
#define _gVERNIERA_KtoC			      -273.15		//Constante para transformar de grados Kelvin a grados Centigrados



	//Variables de los filtros de Kalman
#define _gVERNIERA_DEFAULT_GAIN		    1.0
#define _gVERNIERA_GAIN_DIFF_VOLTAGE	0.1369725