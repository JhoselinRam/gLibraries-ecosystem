/*
||	gSonar library
||	HC-SR04 class
||
||	Author: Jhoselin Adrian Ramirez Montes
||	Version:1.0.0
||	Contact: jhoselin.ramirez92@gmail.com
||
||	This classes are part of the Hardware Enclosure group and can be used with the 
||	general beginAll and updateAll functions, see hardwareEnclosure.cpp
||
||	Description: This library include classes to operate contactless ultrasonic distance
||	sensors, include reads in centimeters, millimeters and inches, has a temperature 
||	compensation and implement a Kalman filter to smooth the data, you can get the raw
||	data as well.
||
||	The supported devices are
||		HC-SR04  (+/- 3mm)
||
||	The Hardware Enclosure is a group of libraries dedicated to simplify at the maximum
||	possible the use of various devices like buttons, leds, etc providing high level 
||	function easy to understand and implement in your projects.
|| 
||
||	Update History
||	--------------------------------------------------------------------------------------
||	Version 1.0.0 dd/mm/yyyy: First Release.
||	--------------------------------------------------------------------------------------
*/

#ifndef __gSONAR__
#define __gSONAR__

#include<Arduino.h>
#include"hardwareEnclosure.h"

#define f2c(Tf) (5.0/9)*((Tf)-32)       //Macro to transform Fahrenheit into Celcius degrees

#define SonarDefaultTEMPERATURE     26.0
#define SonarMaxDistanceDisable     -1.0
#define SonarMaxSpeedDisable        -1.0
#define SonarMaxAccelDisable        -1.0
#define SonarDefaultGAIN         0.485
#define SonarDefaultSpeedGAIN    0.375
#define SonarDefaultAccelGAIN    0.25
#define HC_SR04_ReadPERIOD          50

#define unit_MM   0
#define unit_CM   1
#define unit_INCH 2

#define SonarNormalMODE     0
#define SonarTemporalSLEEP  1
#define SonarPermanentSLEEP 2

//Base class 
class gSonar:public hardwareEnclosure
{
	public: 
		gSonar();
		virtual void begin() = 0;
		virtual void update() = 0;
		float    read(uint8_t unit = unit_MM);
		float    readSpeed(uint8_t unit = unit_MM);
		float    readAcceleration(uint8_t unit = unit_MM);
		uint16_t readRaw();
		void     setAirTemperature(uint8_t temperature);
		uint8_t  getAirTemperature();
		void     sleep();
		void     sleep(uint32_t sleepTime);
		void     wakeUp();
		void     setGain(float newGain);
		float    getGain();
		void     setSpeedGain(float newSpeedGain);
		float    getSpeedGain();
		void     setAccelGain(float newAccelGain);
		float    getAccelGain();
		void     setMaxDistance(float maxDistance, uint8_t unit = unit_MM);
		float    getMaxDistance(uint8_t unit = unit_MM);
		void     setMaxSpeed(float maxSpeed, uint8_t unit = unit_MM);
		float    getMaxSpeed(uint8_t unit = unit_MM);
		float    setMaxAccel(float maxAccel, uint8_t unit = unit_MM);
		void     getMaxAccel(uint8_t unit = unit_MM);
	protected: 
		uint16_t _readTime;             //Raw output data from the sensor
		uint8_t  _status;               //Status of the sensor
		uint32_t _sleepTime;            //Time that the sensor will spend sleep
		unsigned long _lastTime;        //Last time since the sensor was updated
		unsigned long _lastReadTime;    //Tast time the sensor has been read
	private:
		uint8_t  _temperature;          //Temperature of the air
		float    _distance;             //Filtered data
		float    _lastDistance; 	    //The last filtered data
		float    _speed;				//The speed of the object
		float    _lastSpeed;	        //The last speed calculated 
		float    _accel;				//The acceleration of the object
		float    _velocity;     	    //Velocity of the sound in the air
		float    _gain;                 //Gain of the filter
		float    _speedGain;			//Gain of the speed filter
		float    _accelGain;			//Gain of the acceleration filter
		float    _maxDistance; 		    //Maximum allowed distance
		float    _maxSpeed;        		//Maximum allowed speed
		float    _maxAccel;				//Maximum allowed acceleration
		
};


//Class for the HC-SR04 ultrasonic sensor

class HC_SR04:public gSonar
{
	public:
		HC_SR04(uint8_t trigPin, uint8_t echoPin);
		virtual void begin();
		virtual void update();
		uint16_t singleRead();
	private:
		uint8_t _trigPin;        //Pin on the arduino board in which the trigger pin will be connected 
		uint8_t _echoPin;        //Pin on the arduino board in which the echo pin will be connected 
};

#endif










