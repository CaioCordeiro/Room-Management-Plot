#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <stdlib.h>
#include <string.h>

char *serial(char *ID)
{
	int fd; /*File Descriptor*/

	/*------------------------------- Opening the Serial Port -------------------------------*/

	/* Change /dev/ttyUSB0 to the one corresponding to your system */

	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
												  /* O_RDWR   - Read/Write access to serial port       */
												  /* O_NOCTTY - No terminal will control the process   */
												  /* Open in blocking mode,read will wait              */

	if (fd == -1) /* Error Checking */
		printf("\n  Error! in Opening ttyACM0  ");
	// else
	// 	printf("\n  WAIT ...\n ");

	/*---------- Setting the Attributes of the serial port using termios structure --------- */

	struct termios SerialPortSettings; /* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

	/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings, B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings, B9600); /* Set Write Speed as 9600                       */

	/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |= CS8;	 /* Set the data bits = 8                                 */

	SerialPortSettings.c_cflag &= ~CRTSCTS;		  /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);		   /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/

	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 8; /* Read at least 12 characters */
	SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */

	/*------------------------------- Read data from serial port -----------------------------*/

	tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */
	int bytes_read = 0;	/* Number of bytes read by the read() system call */
	char read_buffer[8];
	int i = 0;

	bytes_read = read(fd, &read_buffer, 8); /* Read the data                   */

	// printf(read_buffer);

	close(fd); /* Close the serial port */
	strcpy(ID,read_buffer);

	return NULL;
}
