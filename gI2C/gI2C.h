/*
||	I2C communication function library
||
||	Author: Jhoselin Adrian Ramirez Montes
||	Version:1.0.0
||	Contact: jhoselin.ramirez92@gmail.com
||
||
||	Description: This library implement several simple functions to interface with the I2C
||	(TWI) bus. 
||	Arduino by itself provide ways to do that but they are not as friendly as they can be.
||
||
||	Update History
||	--------------------------------------------------------------------------------------
||	Version 1.0.0 dd/mm/yyyy: First Release.
||	--------------------------------------------------------------------------------------
*/

#ifndef __gI2C__
#define __gI2C__

#include<Arduino.h>
#include<Wire.h>

//Read functions
	void readBit(uint8_t address, uint8_t register, uint8_t bit, uint8_t *container);
	void readBits(uint8_t address, uint8_t register, uint8_t startBit, uint8_t endBit, uint8_t *container);
    void readRegister(uint8_t addres, uint8_t register, uint8_t *container);
    void readRegisters(uint8_t address, uint8_t startRegister, uint8_t endRegister, uint8_t *container);
//Write functions    
    void writeBit(uint8_t address, uint8_t register, uint8_t bit, uint8_t data);
    void writeBits(uint8_t address, uint8_t register, uint8_t startBit, uint8_t endBit, uint8_t data);
    void writeRegister(uint8_t address, uint8_t register, uint8_t data);
    void writeRegisters(uint8_t address, uint8_t startRegister, uint8_t endRegister, uint8_t *data);
    
#endif