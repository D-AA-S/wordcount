#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream> 

/*void* progress_monitor(void*)
{

}*/

long wordcount(const char* fileName)
{
    std::ifstream check (fileName, std::ifstream::in);
    if (!check.is_open()) { return 0; }
    else { return 1; }
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