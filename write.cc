#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions           */
#include <errno.h>   /* E */
#include <iostream>
#include <array>
#include <cassert>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>


/*
argv[1]: Number of bytes per packet (required)
argv[2]: Number of Packets (required)
argv[3]: Delay between packets (enter 0 if not wanted)
argv[3]: Comport (required)
argv[4]: Filename where bytes are written (without file ending, required)
*/


int main(const int argc, const char* const* const argv) {

    
    if (argc < 5){
        std::cout << "5 Parameters bytes, packets, delay, comport, filename are required" << std::endl;
        return 0;
    }


    int bytes = atoi(argv[1]);
    int packets = atoi(argv[2]);
    int del = atoi(argv[3]);

    std::string comStart{"/dev/"};
    std::string com{argv[4]};
    std::string completeCom{comStart+com};
    std::string filename{argv[5]};
    std::ofstream file{filename + ".txt"};


    int fd = -1;

    if ((fd = open(completeCom.c_str(),O_RDWR | O_NOCTTY)) < 0) {
        std::cout << "open failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        
    }

    

     struct termios SerialPortSettings = {};

     tcgetattr(fd, &SerialPortSettings);        /* Get the current attributesof the Serial port */

     cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600
                      */
     cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600
                      */

     SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
     SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stopbits,here it is cleared so 1 Stop bit */
     SerialPortSettings.c_cflag &= ~CSIZE;       /* Clears the mask forsetting the data size             */
     SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8*/

     SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flowControl                         */
     SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enablereceiver,Ignore Modem Control lines       */

     SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /*Disable XON/XOFF flow control both i/p and o/p */
     SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Non Cannonical mode                            */

     SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

     if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) {
         perror("tcsetattr");
     }

    

    char toWrite[bytes];
    char written[bytes+1];


    int bytes_written = 0;
    for (int i=0; i< packets; i++){
        for (int i=0; i<bytes; i++){
            char c = 97 + rand()%26;
            toWrite[i] = c;
            written[i] = c;
        }
        written[bytes] = '\0';
        bytes_written = write(fd,toWrite,sizeof(toWrite));
        file << written << std::endl;

        if (del > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(del));
        }
    }

    std::cout << "Writing completed" << std::endl;


}