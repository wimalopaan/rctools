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

/*
argv[1]: Number of bytes per packet (required)
argv[2]: Number of Packets (required)
argv[3]: Comport (required)
argv[4]: Filename where bytes are written (without file ending, required)
*/


int main(const int argc, const char* const* const argv) {

    if (argc < 4){
        std::cout << "4 Parameters bytes, packets, comport, filename are required" << std::endl;
        return 0;
    }


    int bytes = atoi(argv[1]);
    int packets = atoi(argv[2]);

    std::string comStart{"/dev/"};
    std::string com{argv[3]};
    std::string completeCom{comStart+com};

    int fd = -1;

    if ((fd = open(completeCom.c_str(),O_RDWR | O_NOCTTY)) < 0) {
        std::cout << "open failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        
    }
    std::string filename{argv[4]};
    std::ofstream file{filename + ".txt"};

    int c = 0; 
    int hasSomethingToRead = -1;
    char toRead[bytes+1];

    while(c < packets){
        int b = 0;
        for (int i= 0; i<bytes; i++){
            hasSomethingToRead = read(fd,&toRead[i],1);
        }
        toRead[bytes] = '\0';
        file << toRead << std::endl;
        c++;
     }

     std::cout << "Reading completed" << std::endl;
    
}