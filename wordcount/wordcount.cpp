#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream> 

/*void* progress_monitor(void*)
{

}*/

long wordcount(const char* fileName)
{
    long ah = 1; 
    std::ifstream check (fileName, std::ifstream::in);
    if (!check.is_open()) { std::cout << "shit" << std::endl; }
    else { std::cout << "pog" << std::endl; }
    return ah; 
}

int main(int argc, char** argv)
{
    wordcount(argv[1]);
}


typedef struct 
{ long* CurrentStatus,
longInitialValue,
longTerminationValue; 
} PROGRESS_STATUS;