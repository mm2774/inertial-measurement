/**
 * @file 3140_i2c.c
 * @author Samuel DiPietro (modified by Muhammad Moughal for final project)
 * @copyright All rights reserved, 2020
 *
 * This file holds function prototypes for the FX0S8700CQ accelerometer
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */


// #include "3140_concur.h"
#include "fsl_common.h"
#include "3140_serial.h"
#include "3140_i2c.h"
#include "3140_module.h"

/* NON-Blocking Who-Am-I Register Read
 *
 * Outputs: Returns 1 if the Who-Am-I Register was able to be read and was
 *			 the correct value. Returns -1 if there was an error
 */
int MODULE_ReadWhoAmI(void)
{
	uint8_t who_am_i_address = FXOS8700CQ_WHOAMI;		// Who_am_I register Address
	uint8_t who_am_i_return;											// Who_am_I buff
	uint8_t find_device  = 0;											// Boolean To verify device
	
	/* Create an initialize a transfer struct, used whenever sending/recieving */
	i2c_master_transfer_t masterTransfer;
	memset(&masterTransfer,0,sizeof(masterTransfer));
	
	masterTransfer.slaveAddress		= FXOS8700CQ_ADDRESS;						// Slave Address
	masterTransfer.direction			= kI2C_Write;								// Read or Write
	masterTransfer.subaddress			= 0;												// Dont know 
	masterTransfer.subaddressSize	= 0;												// Dont know
	masterTransfer.data						= &who_am_i_address;				// Data to send (Can be array)
	masterTransfer.dataSize				= 1;												// Number of bytes to send
	masterTransfer.flags 					= kI2C_TransferNoStopFlag;	// Determines I2C stop/start
	
	/* Please see fsl_i2c.h, line 115 for definitions of _i2c_master_transfer_flags */
	
	/* Initiate a transfer, note that this is non blocking i.e  thread safe */
	I2C_MasterTransferNonBlocking(I2C_CHANNEL_BASE, &g_m_handle, &masterTransfer);

	/*  wait for transfer completed. */
	while ((!nakFlag) && (!completionFlag));

	nakFlag = false;

	if (completionFlag == true)
	{
			completionFlag     = false;
			find_device        = true;
	}

	// If we have found the device, check it's who_am_i register
	if (find_device == true)
	{
			// Initiate single byte read from i2c device
			masterTransfer.direction      = kI2C_Read;
			masterTransfer.subaddress     = 0;
			masterTransfer.subaddressSize = 0;
			masterTransfer.data           = &who_am_i_return;		// buff for read value
			masterTransfer.dataSize       = 1;				
			masterTransfer.flags          = kI2C_TransferRepeatedStartFlag;
					
			I2C_MasterTransferNonBlocking(I2C_CHANNEL_BASE, &g_m_handle, &masterTransfer);
					
			while ((!nakFlag) && (!completionFlag));

			nakFlag = 0;

			if (completionFlag == true)
			{
					completionFlag = 0;
				
					// Check who am i value of the module
					if (who_am_i_return == FXOS8700CQ_WHOAMI)
					{
							return 1;
					}
			}
			return -1;
	}
	
	return 1;
}

/* NON-Blocking: Writes the base setup to the accelerometer & magnetometer. Configures it 
 * 				 for polling accelerometer data and magnetometer data
 *
 * Outputs: Returns 1 if the configuration suceeded, -1 if it failed
 */
int MODULE_getDefaultConfig(void)
{
	// First Check WhoAmI Register to ensure that its working
	if (MODULE_ReadWhoAmI()){
		
		// See the FXOS8700CQ datasheet for the configuration reference

		// write 0000 0000 = 0x00 to accelerometer control register 1 toplace FXOS8700CQ into
		// standby
		// [7-1] = 0000 000
		// [0]: active=0
		if (I2C_WriteReg(FXOS8700CQ_ADDRESS, FXOS8700CQ_CTRL_REG1, 0x00) != 1) 
		{
			return -1;
		}
		
		// write 0001 1111 = 0x1F to magnetometer control register 1
		// [7]: m_acal=0: auto calibration disabled
		// [6]: m_rst=0: no one-shot magnetic reset
		// [5]: m_ost=0: no one-shot magnetic measurement
		// [4-2]: m_os=111=7: 8x oversampling (for 200Hz) to reduce magnetometer noise
		// [1-0]: m_hms=11=3: select hybrid mode with accel and magnetometer active
		if (I2C_WriteReg(FXOS8700CQ_ADDRESS, FXOS8700CQ_M_CTRL_REG1, 0x1F) != 1)
		{
			return -1;
		}		
		
		// write 0010 0000 = 0x20 to magnetometer control register 2
		// [7]: reserved
		// [6]: reserved
		// [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the accelerometer registers
		// [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
		// [3]: m_maxmin_dis_ths=0
		// [2]: m_maxmin_rst=0
		// [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle
		if (I2C_WriteReg(FXOS8700CQ_ADDRESS, FXOS8700CQ_M_CTRL_REG2, 0x20) != 1)
		{
			return -1;
		}
		
		// write 0000 0001= 0x01 to XYZ_DATA_CFG register
		// [7]: reserved
		// [6]: reserved
		// [5]: reserved
		// [4]: hpf_out=0
		// [3]: reserved
		// [2]: reserved
		// [1-0]: fs=01 for accelerometer range of +/-4g range with 0.488mg/LSB
		if (I2C_WriteReg(FXOS8700CQ_ADDRESS, FXOS8700CQ_XYZ_DATA_CFG, 0x01) != 1)
		{
			return -1;
		}

		// write 0000 1101 = 0x0D to accelerometer control register 1
		// [7-6]: aslp_rate=00
		// [5-3]: dr=001 for 200Hz data rate (when in hybrid mode)
		// [2]: lnoise=1 for low noise mode
		// [1]: f_read=0 for normal 16 bit reads
		// [0]: active=1 to take the part out of standby and enable sampling
		if (I2C_WriteReg(FXOS8700CQ_ADDRESS, FXOS8700CQ_CTRL_REG1, 0x0D) != 1)
		{
			return -1;
		}

	} else {
		return -1;
	}
	
	return 1; // I2C Writes sucessful, FXOS8700CQ sucessfully configured
	
}

/* Reads data from the module's registers. Also records status register value
 * Input Args[]		accelDat, magnDat: The pointers to the SRAWDATA structs that will recieve the 
 *							  the polled data
 * 
 * Output: Returns 1 on sucessful read, with the read bytes in the SRAWDATA struct. 
 * 		   Returns -1 on failure to read bytes
 */
int MODULE_getModuleDat( SRAWDATA * accelDat, SRAWDATA * magnDat)
{
	// Initiate a multibyte read from the device of the legth ACCEL_READ_LEN
	uint8_t buff[MODULE_READ_LEN];
	I2C_ReadRegs(FXOS8700CQ_ADDRESS, FXOS8700CQ_STATUS, buff, MODULE_READ_LEN);
	
	// Save the status register because why not, might be useful
	accelDat->s							= (uint8_t) buff[0];
	// Now get the Data (note that it comes in two different bytes
	accelDat->x             = (int16_t)((buff[1] << 8) | buff[2]) >> 2;
	accelDat->y             = (int16_t)((buff[3] << 8) | buff[4]) >> 2;
	accelDat->z             = (int16_t)((buff[5] << 8) | buff[6]) >> 2;
	
	// copy the magnetometer byte data into 16 bit words
	magnDat->x = (buff[7] << 8) | buff[8];
	magnDat->y = (buff[9] << 8) | buff[10];
	magnDat->z = (buff[11] << 8) | buff[12];
			
	return 1; // Sucessful Data Read

}


