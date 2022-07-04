#include<Arduino.h>
#include"gSonar.h"



/*---------------------------------gSonar Constructor-------------------------------------
||	Description: Set the initial values of the base gSonar class.
||
||	Syntax: gSonar()
||
||	Arguments: No arguments.
||
||	Return: No return.
||
||	See: HC-SR04 constructor
*/
gSonar::gSonar()
{
	setAirTemperature(SonarDefaultTEMPERATURE);
	setGain(SonarDefaultGAIN);
	setSpeedGain(SonarDefaultSpeedGAIN);
	setAccelGain(SonarDefaultAccelGAIN);
	setMaxDistance(SonarMaxDistanceDisable);
	setMaxSpeed(SonarMaxSpeedDisable);  
	setMaxAccel(SonarMaxAccelDisable);
	_distance     = 0;
	_lastDistance = 0;  
	_speed        = 0;
	_lastSpeed    = 0;
	_accel        = 0;   
	_readTime     = 0; 
	_sleepTime    = 0;   
	_status = SonarNormalMODE; 
}



/*---------------------------------HC-SR04 constructor------------------------------------
||	Description: Set the initial values of the HC-SR04 class, the gSonar constructor is
||	  also called here.
||
||	Syntax: HC_SR04 mySonar(trigPin, echoPin)
||
||	Arguments: trigPin.-  The pin number in witch the trigger pin will be connected.
||
||			   echoPin.-  The pin number in witch the echo pin will be connected.
||
||	Return: No return.
||
||	See: gSonar constructor
*/
HC_SR04::HC_SR04(uint8_t trigPin, uint8_t echoPin)
{
	_trigPin = trigPin;
	_echoPin = echoPin;
	_lastTime = 0;
    _lastReadTime = millis();
	
	_addressBook.pushIn((hardwareEnclosure*)this);  //Used to include this object in the 
}													//hardwareEnclosure address book




/*--------------------------------HC-SR04 begin method------------------------------------
||	Description: Set the correct pin mode for the trigger and echo pin.
||
||	Syntax: begin()
||
||	Arguments: No arguments.
||
||	Return: No return.
||
||	See: beginAll (defined in hardwareEnclosure.cpp)
*/
void HC_SR04::begin()
{
	pinMode(_trigPin,OUTPUT);
	pinMode(_echoPin,INPUT);
}

/*--------------------------------HC-SR04 update method-----------------------------------
||	Description: Updates the timers and raw data for the sensor
||
||	Syntax: update()
||
||	Arguments: No arguments.
||
||	Return: No return.
||
||	See: updateAll (defined in hardwareEnclosure.cpp)
||		 sleep()
||       wakeUp()
*/
void HC_SR04::update()
{
	if(_status != SonarPermanentSLEEP)             //If the sensor is not in permanent sleep mode 
	{
		if(_status == SonarTemporalSLEEP)          //if the sensor is in temporal sleep mode
		{
			if(millis() - _lastTime >= _sleepTime) //If the sleep mode ends
				wakeUp();                          //Wake up the sensor
		}
		else                                       //If the sensor is in normal mode
		{
			if(millis() - _lastTime >= HC_SR04_ReadPERIOD) //If a read can be made safely 
			{	
				digitalWrite(_trigPin,LOW);              //Be sure that the _trigPin is LOW
				delayMicroseconds(2);                    //Short delay
				digitalWrite(_trigPin,HIGH);             //Set the _trigPin HIGH
				delayMicroseconds(10);                   //Short delay (needed) 
				digitalWrite(_trigPin,LOW);              //Set the _trigPin LOW
				_readTime = pulseIn(_echoPin,HIGH);      //Read the echo form the _echoPin
				_lastTime = millis();                    //Update the _lastTime variable
			}
		}
	}	
}



/*--------------------------------HC-SR04 singleRead method-------------------------------
||	Description: Perform a single read if the sensor is in sleep mode and return the raw
||	  data, is the sensor is not in sleep mode calling this method will have no effect.
||
||	Syntax: singleRead()
||
||	Arguments: No arguments.
||
||	Return: A number between 0 and 65,535 representing the time in microseconds that takes 
||			  the sound wave to travel from the sensor to the obstacle and return to the 
||			  sensor.
||
||	Notes: To obtain the obstacle distance you must multiply the velocity of the sound 
||		     in the air for the return of this method and divide dy 2.
||		   This method also updates the _readTime so, you can use the readRaw method to
||		   get the output of the single read.
||
||	See: update()
||		 updateAll()(defined in hardwareEnclosure.cpp)
||		 sleep()
*/
uint16_t HC_SR04::singleRead()
{
	uint8_t actualStatus;       //Used to store the actual status of the sensor
	unsigned long actualTime;   //Used to store the actual time
	
	if(_status != SonarNormalMODE) //If the sensor is in sleep mode
	{
		actualStatus = _status;    //Save the actual status
		actualTime = _lastTime;    //Save the actual time
	
		_status = SonarNormalMODE; //Set the status to normal mode
		update();                  //Call the update method to calculate the _readTime variable
	
		_status = actualStatus;    //Restore the original status value
		_lastTime = actualTime;    //Restore the original time value
	}
	
	return _readTime;              //Return the raw data
}


/*-----------------------------------read method------------------------------------------
||	Description: Returns the distance of a nearby obstacle to the sensor, this method 
||	  implement a simple one dimensional Kalman filter to smooth the data.
||
||	Syntax: read(unit)
||
||	Arguments: unit(optional).-  A number representing the distance unit used, the only 
||								 allowed values are: unit_MM to use millimeters, 
||								 unit_CM to use centimeters and unit_INCH to use inches.
||								   Default value: unit_MM.
||
||	Return: A floating point number representing the distance of an obstacle near to the 
||		      sensor in the specified units.
||
||	Notes: Due the filtration, the first data of this method after enter in sleep mode
||		     may change but it rapidly converge to a certain value.
||
||	See: update()
||		 updateAll() (defined in hardwareEnclosure.cpp)
||		 readRaw()
||		 Macro unit_MM (defined in gSonar.h)
||		 Macro unit_CM (defined in gSonar.h)
||		 Macro unit_INCH (defined in gSonar.h)
||		 setAirTemperature()
||		 getAirTemperature()
||		 setSensorError()
||		 getSensorError()
||		 setProcessError()
||		 getProcessError()
*/
float gSonar::read(uint8_t unit)
{
	float measurement; //Distance calculated with the raw data
	float rawSpeed;    //Velocity calculated with the raw data
	float rawAccel;	   //Acceleration calculated with the raw data
	float dt;          //Delta time
	
	
	
	measurement = _velocity*_readTime/2.0;					//Distance of the obstacle 
	dt = (millis() - _lastReadTime) / 1000.0;				//dt in seconds
	_lastReadTime = millis();								//Update the latReadtime variable
	
	
	
	
	//--------- Noise filter ---------
	_distance += _gain*(measurement - _distance); //Apply the noise filter




	rawSpeed = (_distance - _lastDistance)/dt;		//Raw speed of the object
	_speed  += _speedGain*(rawSpeed - _speed);      //Speed of the object
	_speed  += 0.0001;                              //Prevents division by cero
	
	rawAccel = (_speed - _lastSpeed)/dt;			//Raw acceleration of the object
	_accel  += _accelGain*(rawAccel - _accel);		//Acceleration of the object
	_accel  += 0.0001;								//Prevents division by cero


	//--------- Acceleration filter ------		  //If the max acceleration filter is enabled
	if(_maxAccel != SonarMaxAccelDisable){
		if(abs(_accel) > _maxAccel){
			_accel    = (_accel / abs(_accel)) * _maxAccel;
			_speed    = _lastSpeed + _accel*dt;
			_distance = _lastDistance + _speed*dt + 1.0/2.0*_accel*dt*dt; 
		}
	}
	
			
	//_________ Velocity filter -----
	if(_maxSpeed != SonarMaxSpeedDisable){		  //If the max speed filter is enabled
		if(abs(_speed) > _maxSpeed){
			_speed = (_speed/abs(_speed)) * _maxSpeed;
			_accel = (_speed - _lastSpeed)/dt + 0.0001;
			_distance = _lastDistance + _speed*dt + 1.0/2.0*_accel*dt*dt;
		}
	}
	
	

	
	//--------- Distance filter ------
	if(_maxDistance != SonarMaxDistanceDisable){   //If the max distance filter is enabled
		_distance = (_distance > _maxDistance)? _maxDistance : _distance;		//Apply the filter
		_distance = (_distance < 0)? 0 : _distance;
		_speed    = (_distance - _lastDistance)/dt + 0.0001;
		_accel    = (_speed - _lastSpeed)/dt + 0.0001;
	}
		
	
	_lastDistance = _distance;		//Update the last distance variable	
	_lastSpeed    = _speed;			//Update the last speed variable      
	
	
	//-----------
	
	if(unit == unit_MM)            //Return the data in the correct units 
		return _distance;
	else
		if(unit == unit_CM)
			return _distance * 0.1;
		else
			if(unit == unit_INCH)
				return _distance * 0.0394;
}



/*----------------------------------readRaw method----------------------------------------
||	Description: This method return the raw data obtained from the sensor, this is useful 
||	  if you want to process the data in other plataform like processing.
||
||	Syntax: readRaw()
||
||	Arguments: No arguments.
||
||	Return: A number between 0 and 65,535 representing the time in microseconds that takes 
||			  the sound wave to travel from the sensor to the obstacle and return to the 
||			  sensor.
||
||	Notes: To obtain the obstacle distance you must multiply the velocity of the sound 
||		     in the air for the return of this method and divide dy 2.
||
||	See: update()
||		 updateAll() (defined in hardwareEnclosure.cpp)
||		 read()
*/
uint16_t gSonar::readRaw()
{
	return _readTime;
}



/*-------------------------------setAirTemperature method---------------------------------
||	Description: Change the default value of the air temperature, this value is used to
||	  calculate the distance output. Due the dependency of the velocity of the sound in 
||	  the air with the temperature, you can use this function to continuously update this 
||	  value and obtain better results.
||
||	Syntax: setAirTemperature(temperature)
||
||	Arguments: temperature.- A number between 0 and 255 representing the air temperature 
||							   in Celsius degrees.
||
||	Return: No return.
||
||	Notes: If you want to use Fahrenheit degrees you can use the f2c macro defined in
||		    gSonar.h
||		    e.g. To set the temperature in 82 Fahrenheit degrees use 
||			 setAirTemperature(f2c(82))
||
||	See: read()
||		 getAirTemperature()
*/
void gSonar::setAirTemperature(uint8_t temperature)
{
	_temperature = temperature;
	_velocity = (331.4+0.6*_temperature)/1000.0;  //Velocity in mm/us
}



/*--------------------------------getAirTemperature method--------------------------------
||	Description: Gets the air temperature used to calculate the distance.
||
||	Syntax: getAirTemperature()
||
||	Arguments: No arguments.
||
||	Return: A number between 0 and 255 representing the air temperature in Celsius 
||            degrees.
||
||	See: read()
||		 setAirTemperature()
*/
uint8_t gSonar::getAirTemperature()
{
	return _temperature;
}



/*-------------------------------------sleep method---------------------------------------
||	Description: Set the sensor into sleep mode permanently, in this mode the sensor will
||	  not perform new measurements and the read and readRaw methods will return the last 
||	  calculated value.
||
||	Syntax: sleep()
||
||	Arguments: No arguments.
||
||	Return: No return.
||
||	See: update()
||	     updateAll() (defined in hardwareEnclosure.cpp)
||		 wakeUp()
*/
void gSonar::sleep()
{
	if(_status != SonarPermanentSLEEP)
		_status = SonarPermanentSLEEP;
}



/*-------------------------------------sleep method---------------------------------------
||	Description: Set the sensor into sleep mode temporarily, in this mode the sensor will
||	  not perform new measurements and the read and readRaw methods will return the last 
||	  calculated value.
||
||	Syntax: sleep(sleepTime)
||
||	Arguments: sleepTime.- A unsigned 32 bit value (unsigned long) representing the time
||		                     in milliseconds that the sensor will spend in sleep mode.
||
||	Return: No return.
||
||	Notes: Consecutive calls to sleep do not restart the sleep timer.
||
||	See: update()
||	     updateAll() (defined in hardwareEnclosure.cpp)
||		 wakeUp()
*/
void gSonar::sleep(uint32_t sleepTime)
{
	if(_status != SonarTemporalSLEEP)
	{
		_status = SonarTemporalSLEEP;
		_sleepTime = sleepTime;
		_lastTime = millis();
	}
}



/*-------------------------------------wakeUp method--------------------------------------
||	Description: This method set off the sleep mode immediately.
||
||	Syntax: wakeUp()
||
||	Arguments: No arguments.
||
||	Return: No return.
||
||	See: update()
||		 updateAll()(defined in hardwareEnclosure.h)
||       sleep()
*/
void gSonar::wakeUp()
{
	if(_status != SonarNormalMODE)
		_status = SonarNormalMODE;
}



void gSonar::setGain(float newGain){
	_gain = constrain(newGain,0.0,1.0);
}



float gSonar::getGain(){
	return _gain;
}



void gSonar::setSpeedGain(float newSpeedGain){
	_speedGain = constrain(newSpeedGain,0.0,1.0);
}



float gSonar::getSpeedGain(){
	return _speedGain;
}



void gSonar::setAccelGain(float newAccelGain){
	_accelGain = constrain(newAccelGain,0.0,1.0);
}



float gSonar::getAccelGain(){
	return _accelGain;
}



void gSonar::setMaxDistance(float maxDistance, uint8_t unit = unit_MM){
	if(maxDistance < 0)
		maxDistance = SonarMaxDistanceDisable;
	
	switch(unit){
		case unit_MM:
				_maxDistance = maxDistance;
			break;
		case unit_CM:
				_maxDistance = maxDistance * 10;
			break;
		case unit_INCH:
				_maxDistance = maxDistance * 25.3807;
			break;
	}
}



float gSonar::getMaxDistance(uint8_t unit = unit_MM){
	switch(unit){
		case unit_MM:
				return _maxDistance;
			break;
		case unit_CM:
				return _maxDistance * 0.1;
			break;
		case unit_INCH:
				return _maxDistance * 0.0394;
			break;
	}
}



void gSonar::setMaxSpeed(float maxSpeed, uint8_t unit = unit_MM){
	if(maxSpeed < 0)
		maxSpeed = SonarMaxSpeedDisable;
		
	switch(unit){
		case unit_MM:
				_maxSpeed = maxSpeed;
			break;
		case unit_CM:
				_maxSpeed = maxSpeed * 10;
			break;
		case unit_INCH:
				_maxSpeed = maxSpeed * 25.3807;
			break;
	}
}



float gSonar::getMaxSpeed(uint8_t unit = unit_MM){
	switch(unit){
		case unit_MM:
				return _maxSpeed;
			break;
		case unit_CM:
				return _maxSpeed * 0.1;
			break;
		case unit_INCH:
				return _maxSpeed * 0.0394;
			break;
	}
}



float gSonar::setMaxAccel(float maxAccel, uint8_t unit = unit_MM){
	if(maxAccel < 0)
		maxAccel = SonarMaxAccelDisable;
		
	switch(unit){
		case unit_MM:
				_maxAccel = maxAccel;
			break;
		case unit_CM:
				_maxAccel = maxAccel * 10;
			break;
		case unit_INCH:
				_maxAccel = maxAccel * 25.3807;
			break;
	}
}



void gSonar::getMaxAccel(uint8_t unit = unit_MM){
	switch(unit){
		case unit_MM:
				return _maxAccel;
			break;
		case unit_CM:
				return _maxAccel * 0.1;
			break;
		case unit_INCH:
				return _maxAccel * 0.0394;
			break;
	}
	
}



float gSonar::readSpeed(uint8_t unit = unit_MM){
	switch(unit){
		case unit_MM:
				return _speed;
			break;
		case unit_CM:
				return _speed * 0.1;
			break;
		case unit_INCH:
				return _speed * 0.0394;
			break;
	}
}



float gSonar::readAcceleration(uint8_t unit = unit_MM){
	switch(unit){
		case unit_MM:
				return _accel;
			break;
		case unit_CM:
				return _accel * 0.1;
			break;
		case unit_INCH:
				return _accel * 0.0394;
			break;
	}
}