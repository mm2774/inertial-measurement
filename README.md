# Default GitHub Page ('gh-pages' branch)

The is the repo for the final project for ECE 3140 which contains the inertial measurement unit (IMU) using an accelerometer and magnetometer along with a visualizer. This code is integrated with previously created concurrency, scheduling, and acceleromater/i2c.
To run, download/extract the zip, compile and flash the IMU program in 'src' to the FRDM K64F board. 
Then run the Python visualizer which will open up a GUI and a figure that captures the board's orientation. 
Note that the yaw/magnetometer values are slightly noisy so the figure will rotate around its Z-axis with noise on the visual. We can counterract this by implementing a filter such as a moving average filter which uses subsets of data to average out the data and thus reduce noise.
The user can also read the orientation values through a serial console such as PuTTY along with the python notebook.

