CODES:

1)final_transmitter: burn it in the transmitter arduino that is 
connected to COM3(your port should be different)port of the pc with USB-B cable.
This arduino has an RF module connected with it.

2)Rf_Red_blue: Burn this code in the arduino MEGA board mounted on the robot's body.

3)fast_red_bue.m :run this MATLAB code after making everything else ready.

Note that, you must have an 'IP webcam' software installed on your smartphone.you may 
download and install the software from google play for your android phone

Also download 'ipcam' support package using MATLAB 'SupportPackageInstaller' command in MATLAB R2015a or newer version


COMPONENTS USED ON THE ROBOT:

1.NRF24L01
2. SharpIR
3.Servos(2)
4.L298 motor shield
5.DC geared motors for wheels with cluster
6.connector,switches etc