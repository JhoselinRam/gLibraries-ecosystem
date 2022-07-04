#include<gStepper.h>


/*------------------------------------Costructor------------------------------------------
||	Establece el valor inicial de todas las variables y selecciona la funcion a ser usada
||	para actualizar la secuencia asi como el primer elemento de esta segun el modo de 
||	operacion seleccionado
||
||	Los modos de operacion admitidos son
||		gStepper_Mode_OnePhase
||		gStepper_Mode_TwoPhase
||		gStepper_Mode_HalfStep
*/
gStepper::gStepper(uint8_t pinA_1, uint8_t pinB_1, uint8_t pinA_2, uint8_t pinB_2, uint8_t mode){

		//Guarda los pines donde se conectaran las bobias 
	_coilPins[0] = pinA_1;
	_coilPins[1] = pinB_1;
	_coilPins[2] = pinA_2;
	_coilPins[3] = pinB_2;
		
		//Establece el valor inicial de las variables de la clase
	_currentStep     = 0;   //Establece la posicion inicial en el paso cero
	_targetStep      = 0;   //Establece la posicion objetivo en cero tambien
	_lastTargetStep  = 0;   //Establece la ultima posicion objetivo en cero
	_velocity        = gStepper_Default_Velocity;	//Establece la velocidad con su valor por defecto
	_totalSteps      = gStepper_Default_TotalSteps;	//Establece la cantidad de pasos por revolucion con su valor por defecto 
	_lastTime        = 0;	//Inicia el cronometro en cero 
	
		//Selecciona las secuencias a usar segun el modo seleccionado
	_sequenceCounter = 1;	//Inicia en el primer elemento de la secuencia
	
	switch(mode){  //Selecciona la funcion que actualiza la secuencia segun el modo a usar
		case gStepper_Mode_OnePhase:                     //Si se eligio usar el modo de una fase
			_pinSequence    = _gStepper_Step_Sequence1;    //Se establece el primer paso en la secuencia
			_selectSequence = &_selectSequence_OnePhase;   //Se establece la funcion para actualizar la secuencia
			break;  
		case gStepper_Mode_TwoPhase:                     //Si se eligio usar el modo de dos fases
			_pinSequence    = _gStepper_Step_Sequence2;    //Se establece el primer paso de la secuencia
			_selectSequence = &_selectSequence_TwoPhase;   //Se establece la funcion para actualizar la secuencia
			break;
		case gStepper_Mode_HalfStep:                     //Si se eligio usar el modo de medios pasos
			_pinSequence    = _gStepper_Step_Sequence1;    //Se establece el primer paso de la secuencia
			_selectSequence = &_selectSequence_HalfStep;   //Se establece la funcion para actualizar la secuencia
			break;
	}
	
}





/*-------------------------------------Funcion begin--------------------------------------
||	Establece como salida los pines donde se conectaran las terminales de las bobinas del
||	motor como salidas y selecciona el valor de estas segun el primer elemento de la 
||	secuencia
*/
void gStepper::begin(){
	
		//Establece los pines donde estan conectadas las bobinas del motor como salidas
	for(int i=0;i<4;i++)
		pinMode(_coilPins[i],OUTPUT);
	
	_setCoilPins();  //Establece los pines segun el primer elemento de la secuencia
	
}





/*------------------------------------Funcion update--------------------------------------
||	Actualiza el cronometro y determina si es necesario avanzar otro paso
*/
void gStepper::update(){
	
	if(_currentStep != _targetStep){						//Si la posicion actual es diferente de la posicion objetivo
		if(millis() - _lastTime >= 1000.0/_velocity){    //Si ha pasado el suficiente tiempo desde que se dio el ultimo paso
			(this->*_selectSequence)();  //Selecciona el siguiente elemento de la secuencia
			_setCoilPins();			     //Establece los pines segun el elemento de la secuencia
			_lastTime = millis();        //Reinicia el cronometro
		}
	}
	
}





/*------------------------------------Funcion moveTo--------------------------------------
||	Mueve el motor a la posicion ingresada a la funcion, este valor puede ser expresado en
||	grados o en pasos segun el valor de variable mode.
||	Se encuentra en grados por default
||
||	Los modos admitidos son
||		gStepper_Grads
||		gStepper_Steps
*/
void gStepper::moveTo(float position, uint8_t mode){
	
	switch(mode){				//Segun el modo a ser usado
		case gStepper_Grads:	//Si se elige trabajar en grados
				_targetStep = round(position*(float)_totalSteps/360.0); //Se convierte la posicion ingresada a pasos
			break;
		case gStepper_Steps:	//Si se elige trabajar en pasos
				_targetStep = round(position);	//Simplemente se redondea la posicion ingresada
			break;
	}
	
}





/*-----------------------------------Funcion advance--------------------------------------
||	Hace avanzar al motor la distancia ingresada sin importar su ubicacion actual, este
||	valor puede ser expresado en grados o en pasos segun el valor de la variable mode.
||	Se encuentra en grados por default
||
||	Los modos admitidos son
||		gStepper_Grads
||		gStepper_Steps
*/
void gStepper::advance(float distance, uint8_t mode){
	
	switch(mode){				//Segun el modo a ser usado
		case gStepper_Grads:	//Si se elige trabajar en grados
				_targetStep = _currentStep + round(distance*(float)_totalSteps/360.0); //Se convierte la distancia ingresada a pasos y calcula la nueva posicion objetivo
			break;
		case gStepper_Steps:	//Si se elige trabajar en pasos
				_targetStep = _currentStep + round(distance);	//Simplemente se redondea la posicion ingresada y calcula la nueva posicion objetivo
			break;
	}
	
}





/*--------------------------------Funcion getPosition-------------------------------------
||	Retorna la posicion actual del motor, esta puede ser expresada en gados o en pasos 
||	dependiendo del valor de la variable mode.
||	Se encuentra en grados por default
||
||	Los modos admitidos son
||		gStepper_Grads
||		gStepper_Steps
*/
float gStepper::getPosition(uint8_t mode){
	
	switch(mode){						//Segun el modod usado
		case gStepper_Grads:
			return float(_currentStep)*360.0/_totalSteps;	//Regresa la posicion actual en grados 
			break;
		case gStepper_Steps:
			return _currentStep;							//O en pasos
			break;
	}
	
}





/*------------------------------------Funcion run-----------------------------------------
||	Hace avanzar al motor de forma indefinida (o hasta que el contador de pasos llegue
||	al limite), el valor de la variable mode determina si se avanza hacia adelante o en 
||	reversa.
||	Se avanza hacia adelante de forma predeterminada
||
||	Los modos admitidos son
||		gStepper_Forward
||		gStepper_Reverse 
*/
void gStepper::run(uint8_t mode){

	switch(mode){					//Segun el modo
		case gStepper_Forward:		//Si se elige hacia adelante
			_targetStep = 2147483640;		//Establece la posicion objetivo lo mas grande posible
			break;
		case gStepper_Reverse:		//Si se elige en reversa
			_targetStep = -2147483640;		//Establece la posicion objetivo lo mas pequeño posible
			break;
	}

}





/*-------------------------------------Funcion stop---------------------------------------
||	Detiene el motor
*/
void gStepper::stop(){

	_targetStep = _currentStep;	//Establece la posicion objetivo como la posicion actual
	
}





/*-------------------------------------Funcion pause--------------------------------------
||	Detiene el motor, se puede continuar con su funcionamiento regular con una llamada a 
||	la funcion resume
*/
void gStepper::pause(){
	
	_lastTargetStep = _targetStep;	//Guarda la posicion objetivo
	stop();							//Detiene el avanse del motor
	
}





/*--------------------------------------Funcion resume------------------------------------
||	Esta funcion se usa despues de una llamada a la funcion pause para continuar el
||	movimiento del motor desde donde se lo dejo
*/
void gStepper::resume(){
	
	_targetStep = _lastTargetStep;	//Establece la ultima posicion objetivo guardada
	
}





/*-------------------------------Funcion setStepsNumber-----------------------------------
||	Establece el numero de pasos necesarios para dar una revolucion completa, este valor
||	varia si se usa el modo de una (o dos) fases o el modo de medios pasos, siendo este 
||	el dobre que el anterior
*/
void gStepper::setStepsNumber(uint16_t stepsNumber){
	
	_totalSteps = abs(stepsNumber);   //Toma el valor absoluto del argumento, solo por si acaso
	
}





/*--------------------------------Funcion getStepsNumber----------------------------------
||	Regresa el valor de pasos necesarios para dar una revolucion completa
*/
uint16_t gStepper::getStepsNumber(){
	
	return _totalSteps;	//Regresa el valor de los pasos 
	
}





/*--------------------------------Funcion setVelocity-------------------------------------
||	Establece el valor de la velocidad de rotacion del motor, las unidades de esta 
||	dependen del valor de la vaiable mode pudiendo ser grados/sec o pasos/sec
||	Se encuentra en grados/sec por default
||
||	Los modos admitidos son
||		gStepper_Grads
||		gStepper_Steps
*/
void gStepper::setVelocity(float velocity, uint8_t mode){

	switch(mode){				//Segun las unidades elegidas
		case gStepper_Grads:		//Si se eligio gados/sec
			_velocity = round(abs(velocity)*(float)_totalSteps/360.0);	//Se transfora el valor a pasos/sec
			break;
		case gStepper_Steps:		//Si se eligio pasos/sec
			_velocity = round(abs(velocity));	//Solo se redondea el valor absoluto de la cantidad ingresada
			break;
	}

}





/*---------------------------------Funcion getVelocity------------------------------------
||	Regresa el valor de la velocidad de rotacion del motor segun las unidades indicadas
||	las cuales pueden ser grados/sec o pasos/sec
||	Se encuentra en grados/sec por default
||
||	Los modos admitidos son
||		gStepper_Grads
||		gStepper_Steps
*/
float gStepper::getVelocity(uint8_t mode){

	switch(mode){			//Segun las unidades elegidas
		case gStepper_Grads:		//Si se eligio gados/sec
			return (float)_velocity*360.0/_totalSteps;	//Se transforma la velocidad a grados/sec
			break;
		case gStepper_Steps:		//Si se eligio pasos/sec
			return _velocity;	//Se regresa el valor de la velocidad
			break;
	}

}





/*------------------------Funcion privada _selectSequence_OnePhase------------------------
||	Actualiza los elementos de la secuencia en el modo de operacion de una fase
*/
void gStepper::_selectSequence_OnePhase(){
	
	if(_currentStep < _targetStep){		//Si la posicion actual es menor que la posicion objetivo
		
		_sequenceCounter++;			//Aumenta el contador de la secuencia
		if(_sequenceCounter > 4)	//Si el contador sobrepasa el limite (solo hay cuatro elementos)
			_sequenceCounter = 1;	//Regresa el contador al primer elemento
			
		_currentStep++;		//Aumenta en uno la posicion actual
			
	}
	else{								//Si la posicion actual es mayor que la posicion objetivo 
	
		_sequenceCounter--;			//Decrementa el contador de la secuencia
		if(_sequenceCounter < 1)	//Si el contador es menor que uno (el principio de la lista)
			_sequenceCounter = 4;	//Regresa el contador al ultimo elemento
			
		_currentStep--;		//Decrementa en uno la posicion actual
	}
	
	
	
		//Se selecciona el elemento de la secuencia segun el valor del contador
	switch(_sequenceCounter){
		case 1:
			_pinSequence = _gStepper_Step_Sequence1;
			break;
		case 2:
			_pinSequence = _gStepper_Step_Sequence3;
			break;
		case 3:
			_pinSequence = _gStepper_Step_Sequence5;
			break;
		case 4:
			_pinSequence = _gStepper_Step_Sequence7;
			break;
	}
	
}





/*------------------------Funcion privada _selectSequence_TwoPhase------------------------
||	Actualiza los elementos de la secuencia en el modo de operacion de dos fases
*/
void gStepper::_selectSequence_TwoPhase(){
	
	if(_currentStep < _targetStep){		//Si la posicion actual es menor que la posicion objetivo
		
		_sequenceCounter++;			//Aumenta el contador de la secuencia
		if(_sequenceCounter > 4)	//Si el contador sobrepasa el limite (solo hay cuatro elementos)
			_sequenceCounter = 1;	//Regresa el contador al primer elemento
			
		_currentStep++;		//Aumenta en uno la posicion actual
			
	}
	else{								//Si la posicion actual es mayor que la posicion objetivo 
	
		_sequenceCounter--;			//Decrementa el contador de la secuencia
		if(_sequenceCounter < 1)	//Si el contador es menor que uno (el principio de la lista)
			_sequenceCounter = 4;	//Regresa el contador al ultimo elemento
			
		_currentStep--;		//Decrementa en uno la posicion actual
	}
	
	
	
		//Se selecciona el elemento de la secuencia segun el valor del contador
	switch(_sequenceCounter){
		case 1:
			_pinSequence = _gStepper_Step_Sequence2;
			break;
		case 2:
			_pinSequence = _gStepper_Step_Sequence4;
			break;
		case 3:
			_pinSequence = _gStepper_Step_Sequence6;
			break;
		case 4:
			_pinSequence = _gStepper_Step_Sequence8;
			break;
	}
	
}





/*------------------------Funcion privada _selectSequence_HalfStep------------------------
||	Actualiza los elementos de la secuencia en el modo de operacion de una fase
*/
void gStepper::_selectSequence_HalfStep(){
	
	if(_currentStep < _targetStep){		//Si la posicion actual es menor que la posicion objetivo
		
		_sequenceCounter++;			//Aumenta el contador de la secuencia
		if(_sequenceCounter > 8)	//Si el contador sobrepasa el limite (solo hay cuatro elementos)
			_sequenceCounter = 1;	//Regresa el contador al primer elemento
			
		_currentStep++;		//Aumenta en uno la posicion actual
			
	}
	else{								//Si la posicion actual es mayor que la posicion objetivo 
	
		_sequenceCounter--;			//Decrementa el contador de la secuencia
		if(_sequenceCounter < 1)	//Si el contador es menor que uno (el principio de la lista)
			_sequenceCounter = 8;	//Regresa el contador al ultimo elemento
			
		_currentStep--;		//Decrementa en uno la posicion actual
	}
	
	
	
		//Se selecciona el elemento de la secuencia segun el valor del contador
	switch(_sequenceCounter){
		case 1:
			_pinSequence = _gStepper_Step_Sequence1;
			break;
		case 2:
			_pinSequence = _gStepper_Step_Sequence2;
			break;
		case 3:
			_pinSequence = _gStepper_Step_Sequence3;
			break;
		case 4:
			_pinSequence = _gStepper_Step_Sequence4;
			break;
		case 5:
			_pinSequence = _gStepper_Step_Sequence5;
			break;
		case 6:
			_pinSequence = _gStepper_Step_Sequence6;
			break;
		case 7:
			_pinSequence = _gStepper_Step_Sequence7;
			break;
		case 8:
			_pinSequence = _gStepper_Step_Sequence8;
			break;
	}
	
}





/*----------------------------Funcion privada _setCoilPins--------------------------------
||	Establece los valores de los pines de las bobinas del motor segun el elemento de la 
||	secuencia actual
*/
void gStepper::_setCoilPins(){
	
		//Establece el valor de las salidas de los pines segun la secuencia
	digitalWrite(_coilPins[0],(bitRead(_pinSequence,3))?HIGH:LOW);
	digitalWrite(_coilPins[1],(bitRead(_pinSequence,2))?HIGH:LOW);
	digitalWrite(_coilPins[2],(bitRead(_pinSequence,1))?HIGH:LOW);
	digitalWrite(_coilPins[3],(bitRead(_pinSequence,0))?HIGH:LOW);
	
}