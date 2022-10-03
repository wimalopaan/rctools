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
#include "lyra.hpp"
#include <memory>

/*
argv[1]: Number of bytes per packet (required)
argv[2]: Number of Packets (required)
argv[3]: Delay between packets (enter 0 if not wanted)
argv[3]: Comport (required)
argv[4]: Filename where bytes are written (without file ending, required)
*/

int main(const int argc, const char* const* const argv) {

    int bytes = 1;
    int packets = 10;
    int delay = 0;
    std::string filename{"test.txt"};
    std::string comport{"ttyUSB0"};
    bool help = false;

    auto cli = lyra::cli()
        | lyra::opt( bytes, "bytes" )
        ["-b"]["--bytes"]
        ("How many bytes per packet?")
        | lyra::opt( packets, "packets" )
        ["-p"]["--packets"]
        ("How many packets should be sent?")
        | lyra::opt( delay, "delay" )
        ["-d"]["--delay"]
        ("How much delay between packets?")
        | lyra::opt( filename, "filename" )
        ["-f"]["--filename"]
        ("Which filename?")
        | lyra::opt( comport, "comport" )
        ["-c"]["--comport"]
        ("Which comport?")
        | lyra::help(help)
        ["-h"]["--help"]
        ("Help message");
    auto result = cli.parse( { argc, argv } );

    
    if (help){
        std::cout <<  "-b Number of bytes per packet, default 1" << std::endl;
        std::cout <<  "-p Number of packets, default 10"  << std::endl;
        std::cout <<  "-d Delay in Milliseconds, default 0" << std::endl;               
        std::cout <<  "-f Filename, default test.txt" << std::endl;
        std::cout <<  "-c Comport, default ttyUSB0" << std::endl;
    }
   

    std::ofstream file{filename};
    std::string com = "/dev/" + comport;


    int fd = -1;

    if ((fd = open(com.c_str(),O_RDWR | O_NOCTTY)) < 0) {
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



    //[SendData
    
    std::unique_ptr<char[]> toWrite = std::make_unique<char[]>(bytes);
    std::unique_ptr<char[]> written = std::make_unique<char[]>(bytes+1);

    for (int i=0; i< packets; i++){
        for (int i=0; i<bytes; i++){
            char c = 97 + rand()%26;
            toWrite.get()[i] = c;
            written.get()[i] = c;
        }
        written.get()[bytes] = '\0';
        write(fd,toWrite.get(),bytes);
        file << written.get() << std::endl;

        if (delay > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    //]

    std::cout << "Writing completed" << std::endl;

    


}

