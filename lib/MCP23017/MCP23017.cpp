/*************************************************** 
 This is a library for the MCP23017 i2c port expander

 These displays use I2C to communicate, 2 pins are required to
 interface
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <pgmspace.h>
#include "MCP23017.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// minihelper to keep Arduino backward compatibility
static inline void wiresend(uint8_t x) {
#if ARDUINO >= 100
	Wire.write((uint8_t) x);
#else
	Wire.send(x);
#endif
}

static inline uint8_t wirerecv(void) {
#if ARDUINO >= 100
	return Wire.read();
#else
	return Wire.receive();
#endif
}


MCP23017::MCP23017(uint8_t address) {
  	if (address > 7) {
		address = 7;
	}
	i2caddr = address;
};

/**
 * Bit number associated to a give Pin
 */
uint8_t MCP23017::bitForPin(uint8_t pin){
	return pin%8;
}

/**
 * Register address, port dependent, for a given PIN
 */
uint8_t MCP23017::regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
	return(pin<8) ?portAaddr:portBaddr;
}

/**
 * Reads a given register
 */
uint8_t MCP23017::readRegister(uint8_t addr){
	// read the current GPINTEN
	Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
	wiresend(addr);
	Wire.endTransmission();
	Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
	return wirerecv();
}


/**
 * Writes a given register
 */
void MCP23017::writeRegister(uint8_t regAddr, uint8_t regValue){
	// Write the register
	Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
	wiresend(regAddr);
	wiresend(regValue);
	Wire.endTransmission();
}


/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
void MCP23017::updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
	uint8_t regValue;
	uint8_t regAddr=regForPin(pin,portAaddr,portBaddr);
	uint8_t bit=bitForPin(pin);
	regValue = readRegister(regAddr);

	// set the value for the particular bit
	bitWrite(regValue,bit,pValue);

	writeRegister(regAddr,regValue);
}

////////////////////////////////////////////////////////////////////////////////

void MCP23017::begin() {

	Wire.begin();

	// all inputs on port A and B
	writeRegister(MCP23017_IODIRA,0xff);
	writeRegister(MCP23017_IODIRB,0xff);
}

//MODE----------------------------------------------------------------------
void MCP23017::pinMode(uint8_t pin, uint8_t mode) {
	updateRegisterBit(pin,(mode==INPUT),MCP23017_IODIRA,MCP23017_IODIRB);
}

void MCP23017::portMode(uint8_t port, uint8_t mode){
	Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
	if (port == PORTA){
		wiresend(MCP23017_IODIRA);
	}else{
		wiresend(MCP23017_IODIRB);
	}
	wiresend(mode);
	Wire.endTransmission();
}

//WRITE----------------------------------------------------------------------
void MCP23017::pinWrite(uint8_t pin, uint8_t value){
	uint8_t gpio;
	uint8_t bit=bitForPin(pin);

	// read the current GPIO output latches
	uint8_t regAddr=regForPin(pin,MCP23017_OLATA,MCP23017_OLATB);
	gpio = readRegister(regAddr);

	// set the pin and direction
	bitWrite(gpio,bit,value);

	// write the new GPIO
	regAddr=regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
	writeRegister(regAddr,gpio);
}

void MCP23017::portWrite(uint8_t port, uint8_t value){
	Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
	if (port == PORTA){
		wiresend(MCP23017_GPIOA);
	}else{
		wiresend(MCP23017_GPIOB);
	}
	wiresend(value);
	Wire.endTransmission();
}

//READ----------------------------------------------------------------------
uint8_t MCP23017::pinRead(uint8_t pin){
	uint8_t bit=bitForPin(pin);
	uint8_t regAddr=regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
	return (readRegister(regAddr) >> bit) & 0x1;
}

uint8_t MCP23017::portRead(uint8_t port){
	uint8_t a;

	// read the current GPIO output latches
	Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
	if (port == PORTA){
		wiresend(MCP23017_GPIOA);
	}else{
		wiresend(MCP23017_GPIOB);
	}
	Wire.endTransmission();

	Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
	a = wirerecv();
	return a;
}


//PULLUP----------------------------------------------------------------------
 void MCP23017::pinPullUp(uint8_t pin, uint8_t value){
	updateRegisterBit(pin,value,MCP23017_GPPUA,MCP23017_GPPUB);
}

void MCP23017::portPullUp(uint8_t port, uint8_t value){
		Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
	if (port == PORTA){
		wiresend(MCP23017_GPPUA);
	}else{
		wiresend(MCP23017_GPPUB);
	}
	wiresend(value);
	Wire.endTransmission();
}
