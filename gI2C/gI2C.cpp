#include<gI2C.h>



/*-----------------------------------readBit function-------------------------------------
||	Description: Read a single bit from a register
||
||	Syntax: readBit(address,regAddress,bit,&container)
||
||	Arguments: address.-     The I2C address device.
||
||		       regAddress.-  The register address.
||
||			   bit.-         The bit number to read, this number must be between 0 and 7.
||
||	   		   container.-   A pointer to a container, the data read will be stored here.
||
||	Return: No return.
||
||	See: readBits()
*/
void readBit(uint8_t address, uint8_t regAddress, uint8_t bit, uint8_t *container)
{
	readBits(address,regAddress,bit,bit,container);
}



/*-----------------------------------readBits function------------------------------------
||	Description: Read adjacent bits from a register
||
||	Syntax: readBits(address,regAddress,startBit,endBit,&container)
||
||	Arguments: address.-     The I2C address device.
||
||		       regAddress.-  The register address.
||
||			   startBit.-    The first bit to be read.
||
||			   endBit.-      The last bit to be read.
||
||	   		   container.-   A pointer to a container, the data read will be stored here.
||
||	Return: No return.
||
||	See: readRegister()
*/
void readBits(uint8_t address, uint8_t regAddress, uint8_t startBit, uint8_t endBit, uint8_t *container)
{
	uint8_t mask;
	
	if(startBit > endBit)     //Check if startBit is smaller than endBit, if not the values are exchanged 
		{
			mask = startBit;  //Uses the mask as an auxiliary variable
			startBit = endBit;
			endBit = mask;
		}
		
		//Prepare the mask
	mask = 1;
	for(uint8_t i=0;i<endBit-startBit;i++)
		mask = (mask<<1) | 1;
	mask <<= startBit;
	
	
	readRegister(address,regAddress,container); //Read the whole register and store the data in the container variable
	
	   //Eliminate the undesired bits
	*container &= mask; 
	*container >>= startBit;                  
}



/*--------------------------------readRegister function-----------------------------------
||	Description: Read an entire register
||
||	Syntax: readRegister(address,regAddress,&container)
||
||	Arguments: address.-     The I2C address device.
||
||		       regAddress.-  The register number.
||
||	   		   container.-   A pointer to a container, the data read will be stored here.
||
||	Return: No return.
||
||	See: readRegisters()
*/
void readRegister(uint8_t address, uint8_t regAddress, uint8_t *container)
{
	readRegisters(address,regAddress,regAddress,container);	
}



/*-------------------------------readRegisters function-----------------------------------
||	Description: Read a set of adjacent registers
||
||	Syntax: readRegisters(address, startRegister,endRegister, &container)
||
||	Arguments: address.-        The I2C address device.
||
||		       startRegister.-  The first register to be read.
||
||			   endRegister.-    The last Register to be read.
||
||	   		   container.-      A pointer to a container, the data read will be stored
||								here.
||
||	Return: No return.
*/
void readRegisters(uint8_t address, uint8_t startRegister, uint8_t endRegister, uint8_t *container)
{
	uint8_t length;
	
	if(startRegister > endRegister)  //Check if startRegister is smaller than endRegister, if not the values are exchanged 
	{
		length = startRegister;      //Uses the length as an auxiliary variable to make the exchange
		startRegister = endRegister;
		endRegister = length;
	}
	
	length = endRegister - startRegister + 1; //Number of Registers to be read
	
	Wire.beginTransmission(address);
		Wire.write(startRegister);
	Wire.endTransmission();
	Wire.beginTransmission(address);
	Wire.requestFrom(address,length);
	
	for(uint8_t i=0;i<length;i++)
	{
		while(!Wire.available());   //Wait until data are available, this should be short
		container[i] = Wire.read();
	}
	Wire.endTransmission();
	
}

	

/*---------------------------------writeBit function--------------------------------------
||	Description: Write a single bit in a register
||
||	Syntax: writeBit(address,regAddress,bit,data)
||
||	Arguments: address.-     The I2C address device.
||
||		       regAddress.-  The register number.
||
||			   bit.-         The bit number to write, this number must be between 0 and 7.
||
||	   		   data.-        The data to be write, this must be 1 or 0.
||
||	Return: No return.
||
||	See: writeBits()
*/
void writeBit(uint8_t address, uint8_t regAddress, uint8_t bit, uint8_t data)
{
	writeBits(address,regAddress,bit,bit,data);
}



/*----------------------------------writeBits function------------------------------------
||	Description: Write adjacent bits from a register
||
||	Syntax: writeBits(address,regAddress,startBit,endBit,container)
||
||	Arguments: address.-     The I2C address device.
||
||		       regAddress.-  The register number.
||
||			   startBit.-    The first bit to be write.
||
||			   endBit.-      The last bit to be write.
||
||	   		   data.-        The data to be write
||
||	Return: No return.
||
||	See: writeRegister()
*/
void writeBits(uint8_t address, uint8_t regAddress, uint8_t startBit, uint8_t endBit, uint8_t data)
{
	uint8_t registerData;
	
	if(startBit > endBit)     //Check if startBit is smaller than endBit, if not the values are exchanged 
		{
			registerData = startBit;  //Uses the registerData as an auxiliary variable
			startBit = endBit;
			endBit = registerData;
		}
	
	readRegister(address,regAddress,&registerData);  //Read the current information in the register
	
	for(uint8_t i=0;i<endBit-startBit+1;i++)
		bitWrite(registerData,startBit+i,bitRead(data,i));  //Include the new data	
		
	writeRegister(address,regAddress,registerData); //And write back the register
}



/*-------------------------------writeRegister function-----------------------------------
||	Description: Write an entire register
||
||	Syntax: writeRegister(address,regAddress,container)
||
||	Arguments: address.-     The I2C address device.
||
||		       regAddress.-  The register number.
||
||	   		   data.-        The data to be write
||
||	Return: No return.
||
||	See: writeRegisters()
*/
void writeRegister(uint8_t address, uint8_t regAddress, uint8_t data)
{
	writeRegisters(address,regAddress,regAddress,&data);
}



/*------------------------------writeRegisters function-----------------------------------
||	Description: Write a set of adjacent registers
||
||	Syntax: writeRegisters(address, startRegister,endRegister, &container)
||
||	Arguments: address.-        The I2C address device.
||
||		       startRegister.-  The first register to be read.
||
||			   endRegister.-    The last Register to be read.
||
||	   		   data.-           A pointer to the data to be write.
||
||	Return: No return.
*/
void writeRegisters(uint8_t address, uint8_t startRegister, uint8_t endRegister, uint8_t *data)
{
	uint8_t length;
	if(startRegister > endRegister)  //Check if startRegister is smaller than endRegister, if not the values are exchanged 
	{
		length = startRegister;      //Uses the length as an auxiliary variable to make the exchange
		startRegister = endRegister;
		endRegister = length;
	}
	
	length = endRegister - startRegister + 1; //Number of Registers to be write
	
	Wire.beginTransmission(address);
		Wire.write(startRegister);
		for(uint8_t i=0;i<length;i++)
			Wire.write(data[i]);
	Wire.endTransmission();
}