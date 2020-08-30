/**
 * @file 3140_module.h
 * @author Samuel DiPietro (modified by Muhammad Moughal for final project)
 * @copyright All rights reserved, 2020
 *
 * This file holds function prototypes for concurrency functions
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "3140_i2c.h"

#ifndef __3140_module__
#define __3140_module__

#define I2C_BAUD_RATE	100000U		// Speed of I2C Transmission 

#define MODULE_I2C_ADDR	0x1D		// I2C Device Address
#define MODULE_WHO_AM_I	0x0D		// Who-Am-I Reg Address
#define MODULE_I2C_BASE	I2C0		// I2C Channel Used

#define MODULE_READ_LEN 13			// Length of Data Read


/* Struct to store read values from the accelerometer & magnetometer */
typedef struct{
	int16_t x;			// Signed Shorts
	int16_t y;
	int16_t z;
	uint8_t s;			// Status Register 
} SRAWDATA;

/* NON-Blocking Who-Am-I Register Read
 *
 * Outputs: Returns 1 if the Who-Am-I Register was able to be read and was
 *			 the correct value. Returns -1 if there was an error
 */
int MODULE_ReadWhoAmI(void);

/* NON-Blocking: Writes the base setup to the accelerometer & magnetometer. Configures it 
 * 				 for polling accelerometer & magnetometer data.
 *
 * Outputs: Returns 1 if the configuration suceeded, -1 if it failed
 */
int MODULE_getDefaultConfig(void);

/* Reads data from the module's registers. Also records status register value
 * Input Args[]		accelDat, magnDat: The pointers to the SRAWDATA structs that will recieve the 
 *							  the polled data
 * 
 * Output: Returns 1 on sucessful read, with the read bytes in the SRAWDATA struct. 
 * 		   Returns -1 on failure to read bytes
 */
int MODULE_getModuleDat( SRAWDATA * accelDat, SRAWDATA * pMagnData);

#endif /* __3140_module__ */