/*
||	gStepper class
||
||	Author: Jhoselin Adrian Ramirez Montes
||	Version:1.0.0
||	Contact: jhoselin.ramirez92@gmail.com
||
||	Proporciona una interface simple para controlar motores paso a paso (stepper motor)
*/

#ifndef __gSTEPPER__
#define __gSTEPPER__

#include<Arduino.h>

	//Secuencia de los pines (A, B, /A, /B) necesarios para el movimiento  
#define _gStepper_Step_Sequence1 8   //1000  
#define _gStepper_Step_Sequence2 12  //1100
#define _gStepper_Step_Sequence3 4   //0100
#define _gStepper_Step_Sequence4 6   //0110
#define _gStepper_Step_Sequence5 2   //0010
#define _gStepper_Step_Sequence6 3   //0011
#define _gStepper_Step_Sequence7 1   //0001
#define _gStepper_Step_Sequence8 9   //1001

	//Modos de operacion del motor
#define gStepper_Mode_OnePhase   0   //Pasos completos, una sola fase
#define gStepper_Mode_TwoPhase   1   //Pasos completos, dos fases (Con mas torque que una fase pero consume el doble)
#define gStepper_Mode_HalfStep   2   //Medios pasos, intercala entre una y dos fases

	//Dimension de la posicion deseada
#define gStepper_Grads 0	//Valor en grados
#define gStepper_Steps 1    //Valor en pasos 

	//Algunos valores por default
#define gStepper_Default_Velocity   200		//Velocidad por default, 200 pasos por segundo
#define gStepper_Default_TotalSteps 200     //Cantidad por default de pasos por revolucion

	//Algunos parametros extras
#define gStepper_Forward 0	//Se mueve hacia "adelante"
#define gStepper_Reverse 1	//Se mueve en "reversa"

class gStepper{
	
	public:
		gStepper(uint8_t pinA_1, uint8_t pinB_1, uint8_t pinA_2, uint8_t pinB_2, uint8_t mode = gStepper_Mode_TwoPhase);
		void  begin();
		void  update();
		void  moveTo(float position, uint8_t mode = gStepper_Grads);
		void  advance(float distance, uint8_t mode = gStepper_Grads);
		float getPosition(uint8_t mode = gStepper_Grads);
		void  run(uint8_t mode = gStepper_Forward);
		void  stop();
		void  pause();
		void  resume();
		void  setStepsNumber(uint16_t stepsNumber);
		uint16_t getStepsNumber();
		void  setVelocity(float velocity, uint8_t mode = gStepper_Grads);
		float getVelocity(uint8_t mode = gStepper_Grads);
	private:
		uint8_t  _coilPins[4];     //Pines donde se conectaran las bobinas del motor (A, B, /A, /B)
		int32_t  _currentStep;     //Posicion actual del motor (en pasos)
		int32_t  _targetStep;      //Posicion objetivo del motor (en pasos)
		int32_t  _lastTargetStep;  //Ultima posicion objetivo, usada en caso de llamar a la funcion pasuse
		uint16_t _velocity;        //Velocidad de rotacion del motor
		uint8_t  _sequenceCounter; //Contador de la secuencia
		uint8_t  _pinSequence;     //Secuencia de pines a usar
		uint16_t _totalSteps;      //Numero total de pasos pasa realizar una rotacion completa
		double   _lastTime;        //Cuenta del tiempo que ha pasado desde el ultimo paso que se dio
		
		void (gStepper::*_selectSequence)();         //Apuntador a la funcion de seleccion de secuencia segun el modo (una fase, dos fases o medios pasos)
		void _selectSequence_OnePhase();   //Selecciona la secuencia del modo de una fase
		void _selectSequence_TwoPhase();   //Selecciona la secuencia del modo de dos fases
		void _selectSequence_HalfStep();   //Selecciona la secuencia del modo de medios pasos
		void _setCoilPins();               //Establece los pines de las bobinas segun la secuencia actual
	
};

#endif