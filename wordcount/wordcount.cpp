#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream> 
#include <pthread.h>

const int PROG_BAR_SIZE = 50;

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
    struct PROGRESS_STATUS* progStat = (struct PROGRESS_STATUS*)progressStatus;
    //static long percentcheck = 1;

    long* CurrentStatus = progStat->CurrentStatus;
    long InitialValue = progStat->InitialValue;
    long TerminationValue = progStat->TerminationValue;

    int currStatus = 0;
    int prevStatus = 0;
    std::string bar = "---------+---------+---------+---------+---------+";
    while (*CurrentStatus <= TerminationValue)
    {
        currStatus = (((double)*CurrentStatus - (double)InitialValue) / (double)TerminationValue) * PROG_BAR_SIZE;      // stores the number of progress markers that need to be printed
        std::cout << bar.substr(prevStatus, (currStatus - prevStatus)) << std::flush;
        if (currStatus != prevStatus) prevStatus = currStatus;
    }

    /*
        TODO:
        Need to compute the percentage of the task that has been completed and add to a progress bar of 50 characters representing the amount of progress that has been made.
        Print new marker characters without a line feed character (use cout.flush()).
        When the progress indicator has reached the termination value, the thread will print a linefeed and exit the thread.
    */
    return NULL;
}

long wordcount(const char* fileName)
{
    std::ifstream check(fileName, std::ifstream::in);
    if (!check.is_open())
        return -1;

    check.seekg(0, check.end);
    long wordCount = 0;     // Start with 0 words in the file and increment as we go
    long CurrentStatus = 0;     // Current Status starts with 0
    long TerminationValue = (long)check.tellg(); // Sets the termination value to the number of bytes in the file
    check.seekg(0, check.beg);

    // Need to count the number of words in the file
    // Probably going to start this all over....logic is not correct

    // CurrentStatus: A pointer to a long used by wordcount to store the number of bytes processed so far.
    // TerminationValue: Number of bytes in file.
    PROGRESS_STATUS progressStatus(&CurrentStatus, TerminationValue);
    pthread_t pmThread;     // Progress_monitor thread
    pthread_create(&pmThread, NULL, progress_monitor, (void *) &progressStatus);    // int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void* (*start_routine)(void*), void* arg);

    char c;
    while (check.get(c)) {  // Reading one character at a time
        CurrentStatus++;    // incrementing the number of bytes we have read thus far
        if (c == '\n') 
            CurrentStatus++;
        if (isspace(c))
            wordCount++;
    }

    /*
        TODO:  Need to read one character a time, updating the number of bytes processed and counting the number of words in the file.
        We will define a word as a non-zero length sequence of non whitespace characters (whitespace characters are tab, space,
        linefeed, newline, etc.).
    */
    pthread_join(pmThread, NULL);
    return wordCount;
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
    std::cout << "\nThere are " << wordcount(argv[1]) << " words in " << argv[1] << std::endl;
}


