/*
    wordcount.cpp: Reads a text file and determines how many words are in the file.

    Daniel A. Silva Red ID: 820567716
    Kathryn Shafer  Red ID: 821586552
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream> 
#include <pthread.h>

const int PROG_BAR_SIZE = 50;

struct PROGRESS_STATUS
{
    long* CurrentStatus;    // CurrentStatus is a pointer to a long which represents the current status of the computation being tracked.
    long InitialValue;      // InitialValue is the starting value for the computation.
    long TerminationValue;  // TerminationValue is the value at which the computation is complete.

    PROGRESS_STATUS(long* CurrentStatus, long TerminationValue) {
        this->CurrentStatus = CurrentStatus;
        this->InitialValue = 0;
        this->TerminationValue = TerminationValue;
    }
};

void* progress_monitor(void* progressStatus)
{
    struct PROGRESS_STATUS* progStat = (struct PROGRESS_STATUS*) progressStatus;

    long* CurrentStatus = progStat->CurrentStatus;
    long InitialValue = progStat->InitialValue;
    long TerminationValue = progStat->TerminationValue;

    int currStatus = 0;
    int prevStatus = 0;
    std::string bar = "---------+---------+---------+---------+---------+";
    while (*CurrentStatus <= TerminationValue && currStatus != PROG_BAR_SIZE)
    {
        currStatus = (((double)*CurrentStatus - (double)InitialValue) / (double)TerminationValue) * PROG_BAR_SIZE;      // currStatus contains the number of progress markers that should appear on the screen
        std::cout << bar.substr(prevStatus, (currStatus - prevStatus)) << std::flush;   // Print a substring of the "bar" string, and progress the prevStatus counter to the last status marker that was printed
        prevStatus = currStatus;
    }

    // If we ever run into the strange situation where currentStatus exceeds the termination value but we haven't printed the entire progress bar yet, do this here
    if (*CurrentStatus > TerminationValue && currStatus != PROG_BAR_SIZE) {
        currStatus = PROG_BAR_SIZE;      // Set it equal to the progress bar size to print the rest of the progress bar
        std::cout << bar.substr(prevStatus, (currStatus - prevStatus)) << std::flush;
    }

    std::cout << std::endl;
    return NULL;
}

long wordcount(const char* fileName)
{
    std::ifstream check(fileName, std::ifstream::in);
    if (!check.is_open())
        return -1;

    check.seekg(0, check.end);
    long wordCount = 0;     // Start with 0 words in the file and increment as we go
    long CurrentStatus = 0;     // Current Status starts at 0 (start at the beginning of the file)
    long TerminationValue = (long)check.tellg(); // Sets the termination value to the total number of bytes in the file
    check.seekg(0, check.beg);

    PROGRESS_STATUS progressStatus(&CurrentStatus, TerminationValue);
    pthread_t pmThread;     // Progress_monitor thread
    pthread_create(&pmThread, NULL, progress_monitor, (void *) &progressStatus);    // Creates the new thread pmThread which will use the progress_monitor function with the address of the progressStatus struct as the argument

    char c;
    int noDouble = 1; // Variable used to detect consecutive whitespaces
    while (check.get(c)) {  // Reading one character at a time
        CurrentStatus++;    // Incrementing the number of bytes we have read thus far
        if (isspace(c)) { 
            wordCount++;
            noDouble++;
        }
        else if(noDouble >= 1) // Subtracts noDouble counter by 1 with a lower bound of 0 everytime a non-whitespace is encountered
            noDouble--;
        if (noDouble == 2) // The 2 spaces happen side by side, the wordcount is subtracted and the counter is reset back to 1
        {
            wordCount--;
            noDouble--;
        }
    }
    pthread_join(pmThread, NULL);   // Main thread is finished but we need to wait for the progress_monitor thread to catch up
    return wordCount;
}

int main(int argc, char** argv)
{
    long wordAmount;
    if (argc < 2) {
        std::cout << "No file specified" << std::endl;
        return -1;
    }
    
    if (argc > 2) {
        std::cout << "Too many files provided. Please enter 1 file name." << std::endl;
        return -1;
    }
    wordAmount = wordcount(argv[1]);
    std::cout << "There are " << wordAmount << " words in " << argv[1] << std::endl;
}