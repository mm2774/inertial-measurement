# Inertial Measurement Unit

### 6-DOF IMU and Visualizer - by Mo Moughal (mm2774)

#### Introduction

This project is an implementation of an inertial measurement unit (IMU) on the FRDM K64F board. The IMU measures orientation of the board which can be represented by roll, pitch, and yaw values. These values measure the rotation about the x,y,z axes centered on the object. The orientation values are derived from the readings of the board’s accelerometer and magnetometer of which a top view visual is shown from pg. 7 of the module’s data sheet.

The accelerometer measures the amount of acceleration the board is going through in the x,y,z directions. These readings can be used to calculate roll and pitch through trigonometry. Note that it cannot be used to measure yaw, because there is no change in force on the rotation of the z axis. For example, if the device is lying flat, the reading on the z axis would stay constant if the device is rotated along its z axis. Additionally, the device cannot be subject to other forces, otherwise the accelerometer would not be reading its values based on just the Earth’s gravitational field and so we would need to use more than trigonometry to get the values.

The magnetometer measures the forces from the Earth’s magnetic field on the board. The force is a measurement of magnitude and direction in the x,y,z axes. Hence we can combine the data from the accelerometer (the roll and pitch) and the measurements from the magnetometer’s components to calculate the yaw value through trigonometry. Some limitations of the magnetometer include that the magnetic field strength changes with respect to latitude, with it being highest at the poles and lowest at the equator. Additionally, the presence of magnets and electronic items nearby can interfere with the magnetometer’s measurements.

Overall, the project does what is expected in the project’s requirements. The accelerometer and magnetometer are initialized correctly and the data values are processed into orientation values which are sent to a serial port. A Python program successfully reads the serial, creates a GUI, and processes the orientation to rotate an object in real-time on the GUI. One limitation is the noisy yaw measurements which could be improved by averaging the values over five time steps, for example.

This project required learning about communication protocols such as I2C and serial/UART, the ideas behind the accelerometer and magnetometer modules (and how to initialize them), the concept of orientation, and lastly, the connection between two programs (C and Python) through serial communication to solve the requirement of displaying the orientation in real-time through a GUI.

[Diagram of Modules](https://imgur.com/a/Q5Z7lRP)
#### System Overview
[System Block Diagram](https://imgur.com/a/dV4lMQ3)

#### Video
[Project Video](https://vimeo.com/422653163)

#### System Description
The K64F board is provided with accelerometer and magnetometer modules included within the FXOS8700CQ module. The data from these modules is read through I2C and stored in a structure holding the three components of each read. The instantiation of the modules and I2C functionality is followed from the tutorial and the FXOS8700CQ data sheet.

Next, in main.c two concurrent processes are created. One contains logic for toggling the blue LED which lets the user know the board is running while the second process is running. The second process runs the I2C read function to read the data from the accelerometer and magnetometer modules into their respective data structures. The data is then processed into orientation data (roll, pitch, and yaw) which is held in an array for simple access. This array is accessed to send the orientation data to the serial port.

Lastly, the Python program reads orientation data through the serial port, runs a GUI program, and adjusts the rotation of the figure in the GUI through each read of orientation data obtained from the serial port. The packages PyOpenGL, PyGame, and PySerial are used to accomplish these tasks.

#### Testing

This project did not have traditional test cases, but much of the testing was done iteratively with human interaction to make sure correct functionality was achieved. For the C program, the debugger was utilized to find some issues with the orientation processing functionality. Additionally, the serial console PuTTY was used to read the values outputted to serial by the C program. First it was used to read and check the modules’ values, then it was used to check for the orientation values. For the Python program, Jupyter Notebook/Lab was used to quickly run and test results through the GUI. For the real-time figure orientation, the board’s orientation was adjusted by hand and the figure’s orientation was monitored to make sure the orientations were consistent between both.

#### Resources
This project utilized three resources:

1. https://www.nxp.com/products/sensors/motion-sensors/6-axis/digital-motion-sensor-3d-accelerometer-2g-4g-8g-plus-3d-magnetometer:FXOS8700CQ

2. http://students.iitk.ac.in/roboclub/2017/12/21/Beginners-Guide-to-IMU.html

3. https://github.com/thecountoftuscany/PyTeapot-Quaternion-Euler-cube-rotation

The first source (section 13) was used to initialize the magnetometer and modify the rest of the program to read both magnetometer and accelerometer values. The second resource was used for processing the accelerometer and magnetometer data into orientation data. The third resource was used to create the Python GUI and modify the figure through orientation data acquired through the serial port.

#### Work Distribution
This project was done by a single person.
