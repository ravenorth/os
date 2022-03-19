#include <iostream>
#include <fstream>
#include <string>
#include "Handler.h"

const std::string TABLE_FILE_PATH = "../files/table.txt";

int main()
{
    std::ifstream inFile(TABLE_FILE_PATH);
    Handler handler(inFile);

    std::string line;
    while (getline(std::cin, line))
    {
        std::cout << line << ":\t" << handler.Run(line) << std::endl;
    }
}
