#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions           */
#include <errno.h>   /* E */
#include <iostream>
#include <array>
#include <fstream> 
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include "lyra.hpp"



int main(const int argc, const char* const* const argv) {

    int bytes = 1;
    int packets = 10;
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


    //[ReadData
    int c = 0; 
    std::unique_ptr<char[]> toRead = std::make_unique<char[]>(bytes+1);
    while(c < packets){
        for (int i= 0; i<bytes; i++){
            read(fd,&(toRead.get()[i]),1);
        }
        toRead.get()[bytes] = '\0';
        file << toRead.get() << std::endl;
        c++;
     }
     //]

     std::cout << "Reading completed" << std::endl;
    
}
