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
    struct PROGRESS_STATUS *progStat = (struct PROGRESS_STATUS*) progressStatus;
    static long percentcheck = 1 / 50;
    long CurrentStatus = *progStat->CurrentStatus;
    long InitialValue = progStat->InitialValue;
    long TerminationValue = progStat->TerminationValue;
    long currStat = CurrentStatus / TerminationValue;
    long prevstat = 0;
    while (InitialValue != TerminationValue) 
    {
        if (currStat - prevstat >= percentcheck)
        {

        }
        std::cout.flush();
        prevstat = currStat;
        currStat = CurrentStatus / TerminationValue;
    }


    /*
        TODO: 
        Need to compute the percentage of the task that has been completed and add to a progress bar of 50 characters representing the amount of progress that has been made.
        Print new marker characters without a line feed character (use cout.flush()).
        When the progress indicator has reached the termination value, the thread will print a linefeed and exit the thread.
    */
    return (void *)progressStatus;
}

long wordcount(const char* fileName)
{
    std::ifstream check (fileName, std::ifstream::in);
    if (!check.is_open())
        return -1;

    check.seekg(0, check.end);
    long numberOfWords = 0;     // Start with 0 words in the file and increment as we go
    long CurrentStatus = 0;     // Random test values
    long TerminationValue = check.tellg(); // Random test values
    check.seekg(0, check.beg);

    std::cout << TerminationValue << std::endl; 

    /*
        TODO: Need to determine the number of bytes in the file so we can set the TerminationValue appropriately
    */

    // CurrentStatus: A pointer to a long used by wordcount to store the number of bytes processed so far.
    // TerminationValue: Number of bytes in file.
    PROGRESS_STATUS progressStatus(&CurrentStatus, TerminationValue); 
    pthread_t pmThread;     // Progress_monitor thread
    pthread_create(&pmThread, NULL, progress_monitor, &progressStatus);    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void* (*start_routine)(void*), void* arg);

    /*
        TODO:  Need to read one character a time, updating the number of bytes processed and counting the number of words in the file. 
        We will define a word as a non-zero length sequence of non whitespace characters (whitespace characters are tab, space, 
        linefeed, newline, etc.).
    */

    return numberOfWords;
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


