#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_proc.h"
#include "SuperScalar.h"

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

enum stateOfInput{
    GET_INSTRUCTION_FROM_FILE,
    PROCESS_PIPELINE,
    DONE
};

int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params
    int op_type, dest, src1, src2;  // Variables are read from trace file
    unsigned long int pc; // Variable holds the pc read from input file

    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }

    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);
    trace_file          = argv[4];
    printf("rob_size:%lu "
           "iq_size:%lu "
           "width:%lu "
           "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);

    SuperScalar superScalar(params.rob_size, params.iq_size,params.width);

    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    int fileReturn;
    int width_counter = 0;

    enum stateOfInput currentState = GET_INSTRUCTION_FROM_FILE;
    enum stateOfInput nextstate = GET_INSTRUCTION_FROM_FILE;

    bool pipelineComplete;
    unsigned long int instruction_number = 0;
    do{

        switch (currentState){
            case GET_INSTRUCTION_FROM_FILE:{

                if(superScalar.getAcceptableWidth() > 0){
                    fileReturn = fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2);

                    if(fileReturn != EOF){
                        superScalar.setInstructions(pc, op_type, dest, src1, src2, width_counter, instruction_number, superScalar.timingCycle);
                        nextstate = GET_INSTRUCTION_FROM_FILE;
                        superScalar.endOfInstructions(false);
                        instruction_number++;
                        superScalar.timingCycle++;
                    }

                    if(fileReturn == EOF){
                        superScalar.endOfInstructions(true);
                        nextstate = PROCESS_PIPELINE;
                    }
                }
                else if(superScalar.getAcceptableWidth() <= 0) {
                    nextstate = PROCESS_PIPELINE;
                }
            }
            break;
            case PROCESS_PIPELINE: {
                pipelineComplete = superScalar.architectureStages();
                if(pipelineComplete)nextstate = DONE;
                else nextstate = GET_INSTRUCTION_FROM_FILE;
            }
            break;
            case DONE:break;
        }

        currentState = nextstate;


    }while (currentState != DONE);

    superScalar.DisplayFinishedInstructions();

    return 0;
}
