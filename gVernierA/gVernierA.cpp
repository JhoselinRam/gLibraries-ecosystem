#include<gVernierA.h>

/*-------------------------Constructor----------------------------------------------------
||	Constructor de la clase, toma los pines donde se ha conectado la entrada BTA a la 
||	placa Arduino.
||	Guarda esos pines e inicializa algunos parametros.
||
||	Solo los pines SIG1 y SIG2 de la entrada BTA pueden ser configurados el resto de
||	pines deben ser conectados como indica la siguiente tabla.
||
||	BTA			ARDUINO
||
||	VRES		A4	(analog input 4)
||	ID			A5	(analog input 5)	
||	GND			GND
||	5V			5V

*/
gVernierA::gVernierA(int SIG1, int SIG2){

		//Establece los pines del BTA a la placa arduino.
	_sig1 = SIG1;
	_sig2 = SIG2;
	
		//Establece la pendiente, ordenada al origen y coeficiente cuadratico con los valores por default
	_slope     = _gVERNIERA_SLOPE_DEFAULT;
	_intercept = _gVERNIERA_INTERCEPT_DEFAULT;
	_Coeff2    = _gVERNIERA_COEFFICIENT2_DEFAULT;
	
		//ID del dispositivo, cero por defecto
	_id = 0;	
			
		//Establece la funcion de calibracion como lineal por defecto
	_calFunction = &gVernierA::_linear;
	
		//Establece el valor por default de la ganancia del filtro de kalman
	_kGain = _gVERNIERA_DEFAULT_GAIN;
}





/*-------------------------Funcion begin()------------------------------------------------
||	Funcion begin(): Establece los pines como entradas o salidas segun sea necesario,
||		realiza la identificacion del sensor conectado segun su ID y establece los valores
||		correctos de los coeficientes de la funcion de calibracion correspondiente a cada 
||		sensor.
*/
void gVernierA::begin(){
	
	float idVoltage;	//Variable auxiliar en la lectura del ID de dispositivos analogicos
		
	idVoltage = analogRead(_gVERNIERA_ID) * 5.0 / 1024.0;	//Lee el voltaje del pin ID analogico
		
		//Dependiendo de la lectura del voltaje se determina el tipo de sensor
	if(idVoltage > 0.86 && idVoltage < 0.95)
		_id = _gVERNIERA_TERMOCOUPLE;
	else if(idVoltage > 3.72 && idVoltage < 3.86)
		_id = _gVERNIERA_VOLTAGE10;
	else if(idVoltage > 1.92 && idVoltage < 2.13)
		_id = _gVERNIERA_TI_CURRENT;
	else if(idVoltage > 1.18 && idVoltage < 1.30)
		_id = _gVERNIERA_RESISTANCE;
	else if(idVoltage > 3.27 && idVoltage < 3.68)
		_id = _gVERNIERA_XL_TEMEPRATURE;
	else if(idVoltage > 4.64 && idVoltage < 4.73)
		_id = _gVERNIERA_DIFF_VOLTAGE;
	else if(idVoltage > 4.73 && idVoltage < 4.82)
		_id = _gVERNIERA_CURRENT;
	else if(idVoltage > 2.38 && idVoltage < 2.63)
		_id = _gVERNIERA_SS_TEMPERATURE;
	else if(idVoltage > 2.85 && idVoltage < 3.15)
		_id = _gVERNIERA_VOLTAGE30;
	else if(idVoltage > 1.52 && idVoltage < 1.68)
		_id = _gVERNIERA_TI_LIGHT;
	else if(idVoltage > 0.43 && idVoltage < 0.48)
		_id = _gVERNIERA_EXR_HEART_RATE;
	else if(idVoltage > 4.08 && idVoltage < 4.16)
		_id = _gVERNIERA_VOLTAGE_RAW;
	else if(idVoltage > 0.62 && idVoltage < 0.68)
		_id = _gVERNIERA_EKG;
	else if(idVoltage > 4.32 && idVoltage < 4.40)
		_id = _gVERNIERA_CO2;
	else if(idVoltage > 4.50 && idVoltage < 4.59)
		_id = _gVERNIERA_O2;
		
	if(_id == 0){	//Si no se encontro el id con el metodo anterior, revisa mediante el protocolo I2C
		
	}
		
	
	switch(_id){	//Dependiendo el ID del dispositivo selecciona valores y tipo de calibracion
		
		case _gVERNIERA_TERMOCOUPLE:
			_slope       = _gVERNIERA_SLOPE_TERMOCOUPLE;		//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_TERMOCOUPLE;	//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_VOLTAGE10:
			_slope       = _gVERNIERA_SLOPE_VOLTAGE10;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_VOLTAGE10;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
		
		case _gVERNIERA_TI_CURRENT:
			_slope       = _gVERNIERA_SLOPE_TI_CURRENT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_TI_CURRENT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_RESISTANCE:
			_slope       = _gVERNIERA_SLOPE_RESISTANCE;			//Establece la pendiente
			_intercept   = _gVERNIERA_SLOPE_RESISTANCE;			//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_XL_TEMEPRATURE:
			_slope       = _gVERNIERA_SLOPE_XL_TEMEPRATURE;		//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_XL_TEMEPRATURE;	//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_DIFF_VOLTAGE:
			_slope       = _gVERNIERA_SLOPE_DIFF_VOLTAGE;		//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DIFF_VOLTAGE;	//Establece la ordenada al origen
			_kGain       = _gVERNIERA_GAIN_DIFF_VOLTAGE;		//Establece la ganancia del filtro de Kalman
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_CURRENT:
			_slope       = _gVERNIERA_SLOPE_CURRENT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_CURRENT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
		
		case _gVERNIERA_SS_TEMPERATURE:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_logarithmic;			//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_VOLTAGE30:
			_slope       = _gVERNIERA_SLOPE_VOLTAGE30;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_VOLTAGE30;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_TI_LIGHT:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_EXR_HEART_RATE:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_VOLTAGE_RAW:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_EKG:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_CO2:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_O2:
			_slope       = _gVERNIERA_SLOPE_DEFAULT;			//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_DEFAULT;		//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_WR_TEMPERATURE:
			_calFunction = &gVernierA::_cuadratic;				//Establece el tipo de calibracion
			break;
		
		case _gVERNIERA_ETHANOL:
			_calFunction = &gVernierA::_exponential;			//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_SOUND:
			_calFunction = &gVernierA::_cuadratic;				//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_MELT:
			_calFunction = &gVernierA::_cuadratic;				//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_ISE_CA:
			_slope       = _gVERNIERA_SLOPE_ISE;				//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_ISE;			//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_ISE_NH4:
			_slope       = _gVERNIERA_SLOPE_ISE;				//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_ISE;			//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_ISE_NO3:
			_slope       = _gVERNIERA_SLOPE_ISE;				//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_ISE;			//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_ISE_CL:
			_slope       = _gVERNIERA_SLOPE_ISE;				//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_ISE;			//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_ISE_K:
			_slope       = _gVERNIERA_SLOPE_ISE;				//Establece la pendiente
			_intercept   = _gVERNIERA_INTERCEPT_ISE;			//Establece la ordenada al origen
			_calFunction = &gVernierA::_linear;					//Establece el tipo de calibracion
			break;
			
		case _gVERNIERA_16_TERMOCOUPLE:
			_calFunction = &gVernierA::_cuadratic;				//Establece el tipo de calibracion
			break;
		
	}
	
	
		//Realiza una medicion inicial, necesario para el filtro kalman implementado en la funcion read()
	_data = readRaw();
		
}





/*-------------------------Funcion read()-------------------------------------------------
||	Funcion read(): Retorna el valor de la medicion filtrado mediante un filtro de Kalman.
||
||		En esta implementacion del filtro kalman, al ser unidimencional, solo es necesario
||	conocer el valor de la ganancia la cual es constante.
||	El valor de la ganancia depende del sensor y debe ser obtenida experimentalmente.
*/
float gVernierA::read(){
	
	float newData;	//Variable auxiliar
	
	newData = readRaw();	//Lee un nuevo dato y lo guarda en newData
	_data += _kGain*(newData - _data);	//Filtra los datos
	
	return _data;	//Retorna el valor calculado
	
}





/*-------------------------Funcion readRaw()----------------------------------------------
||	Funcion readRaw(): Retorna la medicion hecha por el sensor sin pasarla por el filtro
||		de Kalman.
*/
float gVernierA::readRaw(){
	
	float rawVoltage;	//Voltaje leido directamente del sensor
		
		
		//Si se trata de algun sensor que use la linea de 10 volts
	if(_id == _gVERNIERA_VOLTAGE10 || _id == _gVERNIERA_VOLTAGE30)
		rawVoltage = analogRead(_sig2) * 5.0 / 1024.0;	//Lee la linea de 10 volts (Se necesita hardware adicional o se dañara la placa Arduino)
	else
		rawVoltage = analogRead(_sig1) * 5.0 / 1024.0;	//De otro modo lee la linea de 5 volts 
	
	
		//Se transforma y retorna el voltaje leido mediante la funcion de calibracion
	return (this->*_calFunction)(rawVoltage);
	
}





/*-------------------------Funcion id()---------------------------------------------------
||	Funcion id(): Retorna el ID del dispositivo como un entero.
||
||	Para conocer a que sensor corresponde cada ID consultar el archivo _definitionsD.h
*/
int gVernierA::id(){
	return _id;		//Retorna el ID del sensor
}





/*-------------------------Funcion _linear()----------------------------------------------
||	Funcion _linear(): Es la variante lineal de la funcion de calibracion, la mas comun 
||		entre todos los sensores.
||		Toma como argumento el valor del voltaje leido del sensor (0V - 5V) y realiza un
||		mapeo lineal al rango establecido por el sensor.
*/
float gVernierA::_linear(float voltage){
	return _slope*voltage + _intercept;
}





/*-------------------------Funcion _cuadratic()-------------------------------------------
||	Funcion _cuadratic(): Es la variante cuadratica de la funcion de calibracion.
||		Toma como argumento el valor del voltaje leido del sensor (0V - 5V) y realiza un
||		mapeo cuadratico al rango establecido por el sensor.
*/
float gVernierA::_cuadratic(float voltage){
	return _Coeff2*pow(voltage,2) + _slope*voltage + _intercept;
}





/*-------------------------Funcion _exponential()-----------------------------------------
||	Funcion _exponential(): Es la variante exponencial de la funcion de calibracion.
||		Toma como argumento el valor del voltaje leido del sensor (0V - 5V) y realiza un
||		mapeo exponencial al rango establecido por el sensor.
*/
float gVernierA::_exponential(float voltage){
	return _intercept * pow(voltage, _slope);
}





/*-------------------------Funcion _logarithmic()-----------------------------------------
||	Funcion _logarithmic(): Es la variante logaritmica de la funcion de calibracion.
||		Toma como argumento el valor del voltaje leido del sensor (0V - 5V) y realiza un
||		mapeo logaritmico al rango establecido por el sensor, usando la ecuacion de 
||		Steinhart-Hart.
||
||		Usado solo por la sonda de temperatura de acero inoxidable.
||
||		Internamente el sensor de temperatura tiene una resistencia de 15k ohms en serie 
||		con el termistor y la lectura se toma entre las mismas formando asi un divisor de 
||		voltaje.
||
||		Diagrama:
||        	[Tierra] -- [termistor] -------- | -- [Resistencia de 15k ohms] -- [Vcc (5v)]
||                                           |
||                                        Arduino
||
||		Esta configuracion permite usar un divisor de voltaje entre el termistor y la
||		resistencia fija, y con ayuda de la lectura del voltaje en el Arduino determinar 
||		el valor de la resistencia del termistor.
*/
float gVernierA::_logarithmic(float voltage){
	long  resistance;	//Valor de la resistencia del termistor
	float logR;			//Logaritmo de la resistencia del termistor
	
	resistance = (_gVERNIERA_TERMISTOR_RESISTOR * voltage) / (5.0 - voltage); //Determina el valor de la resistencia en el termistor mediante el divisor de voltaje
	logR = log(resistance);	 //Calcula el logaritmo de la resistencia en el termistor
	
		//Usa la ecuacion de Steinhart-Hart para determinar la temperatura
	return 1.0 / (_gVERNIERA_K0 + _gVERNIERA_K1*logR + _gVERNIERA_K2*pow(logR,3)) + _gVERNIERA_KtoC;
	
}