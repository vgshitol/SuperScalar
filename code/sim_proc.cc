#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_proc.h"
#include "SuperScalar.h"
#include <iostream>
#include <sstream>

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
*/

using namespace std;


int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params

    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }

    stringstream robSize(argv[1]);
    stringstream iqSize(argv[2]);
    stringstream width(argv[3]);

    robSize >> params.rob_size;
    iqSize >> params.iq_size;
    width >> params.width;

    trace_file          = argv[4];

    SuperScalar superScalar(params.rob_size, params.iq_size,params.width);

    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    superScalar.GetInstructionFromFile(FP);


    //    superScalar.finalResults();
    cout << "# === Simulator Command =========" << endl;
    printf("./sim rob_size:%d "
           "iq_size:%d "
           "width:%d "
           "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);
    cout << "# === Processor Configuration ===" << endl;
    cout << "# ROB_SIZE =" <<  params.rob_size << endl;
    cout << "# IQ_SIZE  =" <<  params.iq_size << endl;
    cout << "# WIDTH    =" <<  params.width << endl;
    cout << "# === Simulation Results ========" << endl;
    cout << "# Dynamic Instruction Count    = " << superScalar.NumberOfInstructions << endl;
    cout << "# Cycles                       = " << superScalar.endTime << endl;
    cout << "# Instructions Per Cycle (IPC) = ";
    float cyclesPerInstruction = (float) superScalar.NumberOfInstructions/ superScalar.endTime* 1.0;

    printf("%.2f\n", cyclesPerInstruction);
    return 0;
}
