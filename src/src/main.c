#include "fsl_common.h"
#include "3140_module.h"
#include "3140_serial.h"
#include "3140_i2c.h"
#include <stdio.h>
#include <math.h>
#include "3140_concur.h"
#include "utils.h"

float arr[3]; //holds (roll, pitch, yaw) orientation data
static char string[100]; //holds formatted orientation data to transfer through UART

// Helper calculates orientation (roll, pitch, yaw) from the accel and magn readings and stores them in array arr
void process_data(float ax, float ay, float az, float mx, float my, float mz, float arr[]) 
{
	double pi = 2*acos(0.0); //calc pi
	float pitch = 180 * atan2( ax, sqrt(ay*ay + az*az))/pi;
  float roll = 180 * atan2(ay, sqrt(ax*ax + az*az))/pi;
   
  float mag_x = mx*cos(pitch) + my*sin(roll)*sin(pitch) + mz*cos(roll)*sin(pitch);
  float mag_y = my * cos(roll) - mz * sin(roll);
  float yaw = 180 * atan2(-mag_y,mag_x)/pi;
    
  arr[0] = pitch; arr[1] = roll; arr[2] = yaw;
}

// p1 reads and then transmits a string holding orientation data (roll, pitch, yaw) over UART
void p1 (void)
{
	// holds (x,y,z) components of accel and magn reading
	SRAWDATA accelDat;
	SRAWDATA magnDat;
	
	while (1)
	{
		MODULE_getModuleDat(&accelDat,&magnDat);
		process_data(accelDat.x,accelDat.y,accelDat.z,magnDat.x,magnDat.y,magnDat.z, arr);
		sprintf(string,"r%fr, p%fp, y%fy\n\r", arr[0], arr[1], arr[2]); //formatted for python front-end
		uart_putString(string); //transmit string to serial through UART
	}
}

// Blink the LED concurrently for the user
void p2 (void)
{
	while (1) {
		delay ();
    LEDBlue_Toggle();
	}
}

int main (void)
{
	int clock;
	clock=SystemCoreClock;

	LED_Initialize();
	I2C_ReleaseBus(); // Force Restart the bus, this is done via GPIO commands
									// and not via the onboard i2c module

	I2C_Init();				// Initialize I2C bus and Uart Serial Communications
	uart_init();

	uint8_t a = MODULE_ReadWhoAmI();
	uint8_t b = MODULE_getDefaultConfig();
	
	if (process_create (p1,100) < 0) {
		return -1;
	}

	if (process_create (p2,32) < 0) {
		return -1;
	}
	
	process_start ();

	while (1);

	return 0;
}
