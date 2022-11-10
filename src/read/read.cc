#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions           */
#include <errno.h>   /* E */
#include <array>
#include <fstream>
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include "lyra.hpp"

int main(const int argc, const char *const *const argv)
{

    int bytes = 1;
    int packets = 10;
    std::string filename{"test.txt"};
    std::string comport{"ttyUSB0"};
    bool help = false;

    auto cli = lyra::cli() | lyra::opt(bytes, "bytes")["-b"]["--bytes"]("How many bytes per packet?") | lyra::opt(packets, "packets")["-p"]["--packets"]("How many packets should be sent?") | lyra::opt(filename, "filename")["-f"]["--filename"]("Which filename?") | lyra::opt(comport, "comport")["-c"]["--comport"]("Which comport?") | lyra::help(help)["-h"]["--help"]("Help message");
    auto result = cli.parse({argc, argv});

    if (help)
    {
        std::cout << "-b Number of bytes per packet, default 1" << std::endl;
        std::cout << "-p Number of packets, default 10" << std::endl;
        std::cout << "-f Filename, default test.txt" << std::endl;
        std::cout << "-c Comport, default ttyUSB0" << std::endl;
    }

    std::ofstream file{filename};
    std::string com = "/dev/" + comport;

    int fd = -1;

    if ((fd = open(com.c_str(), O_RDWR | O_NOCTTY)) < 0)
    {
        std::cout << "open failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
    }

    char p;
    read(fd, &p, 1);
    char c = '%';

    //[ReadData
    while (true)
    {
        std::vector<char> characters;

        while (c != 'P')
        {
            if (c != '%'){
                characters.push_back(c);
            }
            read(fd, &c, 1);
        }

        int size = characters.size() + 2;
        std::unique_ptr<char[]> toRead = std::make_unique<char[]>(size);
        toRead.get()[0] = 'P';
        for (int i = 0; i < characters.size(); i++)
        {
            toRead.get()[i + 1] = characters.at(i);
        }
        toRead.get()[size - 1] = '\0';
        file << toRead.get() << std::endl;
        std::cout << toRead.get() << std::endl;
        characters.clear();
        c='%';
    }
    //]
}
