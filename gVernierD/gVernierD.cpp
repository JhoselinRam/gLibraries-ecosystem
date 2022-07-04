#include<gVernierD.h>

/*-------------------------Constructor----------------------------------------------------
||	Constructor de la clase, toma los pines donde se ha conectado la entrada BTD a la 
||	placa Arduino.
||	Guarda esos pines e inicializa algunos parametros.
||
||	Todos los pines pueden ser configurados donde mas convenga, solo es necesario que el
||	pin DIO3 sea conectado a una entrada analogica de la placa Arduino.
||
||	BTD			ARDUINO
||
||	DIO0		Pin digital
||	DIO1	 	Pin digital
||	DIO2		Pin analogico
||	DIO3		Pin digital	
||	GND			GND
||	5V			5V	
||
||	Todos los pines pueden ser conectados en terminales analogicas o digitales de forma
||	indiferente con exepcion del pin DIO2 que tiene que ser conectado extrictamente en un
||	pin analogico, esto con el fin de poder ejecutar la rutina de auto-identificacion

*/
gVernierD::gVernierD(int DIO0, int DIO1, int DIO2, int DIO3){

		//Establece los pines del BTD a la placa arduino.
	_dio0 = DIO0;
	_dio1 = DIO1;
	_dio2 = DIO2;
	_dio3 = DIO3;
	
		//ID del dispositivo, cero por defecto
	_id = 0;	
		
	
		//Establece la funcion de lectura de datos por defecto
	_read = &gVernierD::_readDEFAULT;	
		
		//Establece el valor por default de la ganancia del filtro de kalman
	_kGain = _gVERNIERD_DEFAULT_GAIN;
}





/*-------------------------Funcion begin()------------------------------------------------
||	Funcion begin(): Establece los pines como entradas o salidas segun sea necesario,
||		realiza la identificacion del sensor conectado segun su ID y establece los valores
||		correctos de los coeficientes de la funcion de calibracion correspondiente a cada 
||		sensor.
*/
void gVernierD::begin(){
	
	float idVoltage;	//Variable auxiliar en la lectura del ID de dispositivos analogicos
		
	idVoltage = analogRead(_dio2) * 5.0 / 1024.0;	//Lee el voltaje del pin DIO2 (AutoID)
		
		//Dependiendo de la lectura del voltaje se determina el tipo de sensor
	if(idVoltage > 3.27 & idVoltage < 3.61)
		_id = _gVERNIERD_MOTION;
	else if(idVoltage > 2.80 && idVoltage < 3.20)
		_id = _gVERNIERD_GO_MOTION;
	else if(idVoltage > 4.45 && idVoltage < 4.46)
		_id = _gVERNIERD_N_MOTION;
	else if(idVoltage > 3.80 && idVoltage < 3.88)
		_id = _gVERNIERD_GATE;
	else if(idVoltage > 4.08 && idVoltage < 4.16)
		_id = _gVERNIERD_DROP;
	else if(idVoltage > 4.32 && idVoltage < 4.40)
		_id = _gVERNIERD_ROTARY;
	else if(idVoltage > 4.50 && idVoltage < 4.59)
		_id = _gVERNIERD_RADIATION;
	else if(idVoltage > 4.45 && idVoltage < 4.46)
		_id = _gVERNIERD_DCU1;
	else if(idVoltage > 1.52 && idVoltage < 1.70)
		_id = _gVERNIERD_DCU2;
	else if(idVoltage > 1.18 && idVoltage < 1.30)
		_id = _gVERNIERD_POLARIMETER;
	else if(idVoltage > 0.86 && idVoltage < 0.95)
		_id = _gVERNIERD_PROJECTILE;
	else if(idVoltage > 0.62 && idVoltage < 0.68)
		_id = _gVERNIERD_DAK_CARRIAGE;
	else if(idVoltage > 0.43 && idVoltage < 0.48)
		_id = _gVERNIERD_MOTION_ENCODER;
	else if(idVoltage > 4.64 && idVoltage < 4.73)
		_id = _gVERNIERD_POSIBLE;
		
	
	switch(_id){	//Dependiendo el ID del dispositivo selecciona valores y tipo de calibracion
		
		case _gVERNIERD_GO_MOTION:					//En caso de ser un detector de movimiento
				pinMode(_dio0,INPUT);				//Establece el pin DIO0 como entrada (echo pin)
				pinMode(_dio1,OUTPUT);				//Establece el pin DIO1 como salida (trigger pin)
				_aux1  = _gVERNIERD_SOUND_SPEED;	//Usa la variable auxiliar como la velocidad del sonido en aire
				_kGain = _gVERNIERD_GO_MOTION_GAIN; //Establece la ganancia del filtro de Kalman para este sensor
				_read  = &gVernierD::_readMotion;	//Establece la funcion de lectura de datos
			break;
		
		case _gVERNIERD_MOTION:
			
			break;
				
		case _gVERNIERD_N_MOTION:
			
			break;
		
		case _gVERNIERD_GATE:
			
			break;
			
		case _gVERNIERD_DROP:
			
			break;
			
		case _gVERNIERD_ROTARY:
			
			break;
			
		case _gVERNIERD_RADIATION:
			
			break;
			
		case _gVERNIERD_DCU1:
			
			break;
		
		case _gVERNIERD_DCU2:
			
			break;
			
		case _gVERNIERD_POLARIMETER:
			
			break;
			
		case _gVERNIERD_PROJECTILE:
			
			break;
			
		case _gVERNIERD_DAK_CARRIAGE:
			
			break;
			
		case _gVERNIERD_MOTION_ENCODER:
			
			break;
			
		case _gVERNIERD_POSIBLE:
			
			break;
			
	}
	
	
		//Realiza una medicion inicial, necesario para el filtro kalman implementado en la funcion read()
	_data = (this->*_read)();
		
}





/*-------------------------Funcion read()-------------------------------------------------
||	Funcion read(): Retorna el valor de la medicion filtrado mediante un filtro de Kalman.
||
||		En esta implementacion del filtro kalman, al ser unidimencional, solo es necesario
||	conocer el valor de la ganancia la cual es constante.
||	El valor de la ganancia depende del sensor y debe ser obtenida experimentalmente.
*/
float gVernierD::read(){
	
	float newData;	//Variable auxiliar
	
	newData = (this->*_read)();      	//Lee un nuevo dato y lo guarda en newData
	_data += _kGain*(newData - _data);	//Filtra los datos
	
	return _data;	//Retorna el valor calculado
	
}





/*-------------------------Funcion readRaw()----------------------------------------------
||	Funcion readRaw(): Retorna la medicion hecha por el sensor sin pasarla por el filtro
||		de Kalman.
*/
float gVernierD::readRaw(){
	return (this->*_read)();	
}





/*-------------------------Funcion _readMotion()------------------------------------------
||	Funcion _readMotion(): Realiza la medicion del sensor de movimiento.
||		
||	Dada la orden el sensor envia una señal ultrasonica la cual rebota en algun obstaculo
||	frente al mismo.
||	Se mide el tiempo que le toma a la señal en viajar desde el sensor hasta el obstaculo 
||	y regresar y usando la velocidad del sonido en el aire se calcula la distancia.
*/
float gVernierD::_readMotion(){
	
	unsigned long duration;	//Tiempo que tarda el pulso en llegar al obstaculo y volver
	int echo = 0;			//Detector del eco
	float distance;			//Distancia medida en centimetros
	
	digitalWrite(_dio1,LOW);				//Se asegura que el pin trigger se encuentre en LOW
	delayMicroseconds(4000);				//Pequeño delay
	digitalWrite(_dio1,HIGH);				//Establece en HIGH el pin trigger
	
	duration = micros();					//Comienza a contar el tiempo
	while(echo == 0)						//Mientras no se escuche el eco
		echo = digitalRead(_dio0);				//Revisa el pin echo
		
	digitalWrite(_dio1,LOW);				//Apaga el pin trigger
	
	duration = micros() - duration;			//Calcula el tiempo transcurrido en microsegundos
	distance = _aux1 * duration / 20000.0;	//Calcula la distancia.
	
	return distance;	//Retorna la distancia leida por el sensor
}





/*-------------------------Funcion setSoundSpeed()----------------------------------------
||	Funcion setSoundSpeed(): Establece la velocidad del sonido en el aire en metros por
||	segundo.
||
||	Utiliza la variable auxiliar _aux1 para guardar este dato, esta variable puede ser 
||	usada para diferentes cosas dependiendo del sensor.
*/
void gVernierD::setSoundSpeed(float speed){
	_aux1 = speed;	//Establece la velocidad del sonido en el aire
}





/*-------------------------Funcion id()---------------------------------------------------
||	Funcion id(): Retorna el ID del dispositivo como un entero.
||
||	Para conocer a que sensor corresponde cada ID consultar el archivo _definitionsD.h
*/
int gVernierD::id(){
	return _id;		//Retorna el ID del sensor
}








