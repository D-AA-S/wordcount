#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream> 
#include <pthread.h>

struct PROGRESS_STATUS
{
    // We will refer to the long integer to which this variable dereferences as the progress indicator.
    long* CurrentStatus;    // CurrentStatus is a pointer to a long which represents the current status of the computation being tracked.
    long InitialValue;      // InitialValue is the starting value for the computation.
    long TerminationValue;  // TerminationValue is the value at which the computation is complete.

    PROGRESS_STATUS(long* CurrentStatus, long TerminationValue) {
        this->CurrentStatus = CurrentStatus;
        this->InitialValue = 0;
        this->TerminationValue = TerminationValue;
    }
};

void* progress_monitor(void* progressStatus)    // Should expect void * argument to be a pointer to the PROGRESS_STATUS structure
{
    struct PROGRESS_STATUS *progressStatus = (struct PROGRESS_STATUS*) progressStatus;
}

long wordcount(const char* fileName)
{
    std::ifstream check (fileName, std::ifstream::in);
    if (!check.is_open())
        return 0;
    else
        return 1;

    long CurrentStatus;    
    long TerminationValue;
    
    // CurrentStatus: A pointer to a long used by wordcount to store the number of bytes processed so far.
    // TerminationValue: Number of bytes in file.
    PROGRESS_STATUS progressStatus(&CurrentStatus, TerminationValue); 
    pthread_t pmThread;     // Progress_monitor thread
    pthread_create(&pmThread, NULL, progress_monitor, &progressStatus);    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void* (*start_routine)(void*), void* arg);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "No file specified" << std::endl;
        return -1;
    }
    
    if (argc > 2) {
        std::cout << "Too many files provided. Please enter 1 file name." << std::endl;
        return -1;
    }
    wordcount(argv[1]);
}


