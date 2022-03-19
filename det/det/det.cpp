#include "Handler.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    std::ifstream inFile = std::ifstream(argv[1]);
    std::ofstream outFile = std::ofstream(argv[2]);
    Handler handler(inFile, outFile);
    handler.Run();
}

