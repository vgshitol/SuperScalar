//
// Created by Vishal Shitole on 12/3/18.
//

#ifndef SUPERSCALAR_SUPERSCALAR_H
#define SUPERSCALAR_SUPERSCALAR_H

#include <vector>
#include <math.h>
#include <algorithm>
#include "RMT.h"
#include "IssueQueue.h"
#include "ReorderBuffer.h"
#include "Instruction.h"
#include "Fetch.h"
#include "Decode.h"
#include "Rename.h"
#include "RegisterRead.h"
#include "Dispatch.h"
#include "IssueQueueStage.h"
#include "Execute.h"
#include "Writeback.h"
#include "Retire.h"
#include "algorithm"

using namespace std;

enum stateOfInput{

    GET_INSTRUCTION_FROM_FILE,
    PROCESS_PIPELINE,
    DONE
};

class SuperScalar{
public:
    int rob_size;
    int iq_size;
    int width;

    vector <RMT> rmt;
    vector <ReorderBuffer> rob;
    vector <Instruction> instruction;
    vector <Instruction> finishedInstruction;

    bool eofFlag;

    Fetch fetchStage;
    Decode decodeStage;
    Rename renameStage;
    RegisterRead registerReadStage;
    Dispatch dispatchStage;
    IssueQueueStage issueQueueStage;
    Execute executeStage;
    Writeback writebackStage;
    Retire retireStage;

    unsigned long stallCycle;

    int endTime;

    unsigned long NumberOfInstructions;

    int instructionLoad;


    int getAcceptableWidth() {
        return width - instruction.size();
    }


    /**
     *
     * @param rob_size
     * @param iq_size
     * @param width
     */
    SuperScalar(int rob_size, int iq_size, int width, unsigned long int stallCycle = 0, int instructionLoad = 0){
        this->rob_size = rob_size;
        this->iq_size = iq_size;
        this->width =  width;
        this->initRMT();

        this->fetchStage.width = width;
        this->decodeStage.width = width;
        this->renameStage.width = width;
        this->registerReadStage.width = width;
        this->dispatchStage.width = width;
        this->issueQueueStage.width = iq_size;
        this->executeStage.width = width*5;
        this->writebackStage.width = width*5;
        this->retireStage.width = width;
        this->stallCycle = stallCycle;
        this->instructionLoad = instructionLoad;



    }

    void setInstructions(unsigned long pc, int op_code, int dest, int src1, int src2, int width_counter,
            unsigned long int instruction_number) {
        Instruction temp_instr;
        temp_instr.setInstructionParameters(pc,  op_code,  dest,  src1,  src2, true, true,instruction_number);
        instruction.push_back(temp_instr);
    }

    void endOfInstructions(bool eofFlag){
        this->eofFlag = eofFlag;
    }

    void initRMT(){
        rmt.resize(67);
    }


    struct less_than_key {
        inline bool operator()(Instruction struct1, Instruction struct2) {
            return (struct1.instructionNumber < struct2.instructionNumber);
        }
    };

    void sortFinishedInstructions(){

        sort (finishedInstruction.begin(), finishedInstruction.end(),less_than_key()) ;
    }

  void DisplayFinishedInstructions2(){
    //    sortFinishedInstructions();
        int i = 0;
      while (!finishedInstruction.empty()){ // && i <= 135
            Instruction temp_instruction = finishedInstruction.at(0);


            temp_instruction.decodeStart = temp_instruction.fetchStart + temp_instruction.fetchCycle;
            temp_instruction.renameStart = temp_instruction.decodeStart + temp_instruction.decodeCycle;
            temp_instruction.registerReadStart = temp_instruction.renameStart + temp_instruction.renameCycle;
            temp_instruction.DispatchStart = temp_instruction.registerReadStart + temp_instruction.registerReadCycle;
            temp_instruction.IssueQueueStart = temp_instruction.DispatchStart + temp_instruction.DispatchCycle;
            temp_instruction.ExecuteStart = temp_instruction.IssueQueueStart + temp_instruction.IssueQueueCycle;
            temp_instruction.WriteBackStart = temp_instruction.ExecuteStart + temp_instruction.ExecuteCycle;
            temp_instruction.RetireStart = temp_instruction.WriteBackStart + temp_instruction.WriteBackCycle;

            cout << temp_instruction.instructionNumber << " ";

            cout << "fu{" << temp_instruction.op_code << "} ";
            cout << "src{" << temp_instruction.rs1 << "," << temp_instruction.rs2 << "} ";
            cout << "dst{" << temp_instruction.dest << "} ";
            cout << "FE{" << temp_instruction.fetchStart << "," << temp_instruction.fetchCycle << "} ";
            cout << "DE{" << temp_instruction.decodeStart <<"," << temp_instruction.decodeCycle << "} ";
            cout << "RN{"<< temp_instruction.renameStart <<"," << temp_instruction.renameCycle << "} ";
            cout << "RR{"<< temp_instruction.registerReadStart <<"," << temp_instruction.registerReadCycle << "} ";
            cout << "DI{"<< temp_instruction.DispatchStart <<"," << temp_instruction.DispatchCycle << "} ";
            cout << "IS{"<< temp_instruction.IssueQueueStart <<"," << temp_instruction.IssueQueueCycle << "} ";
            cout << "EX{"<< temp_instruction.ExecuteStart <<"," << temp_instruction.ExecuteCycle << "} ";
            cout << "WB{"<< temp_instruction.WriteBackStart <<"," << temp_instruction.WriteBackCycle << "} ";
            cout << "RT{"<< temp_instruction.RetireStart <<"," << temp_instruction.RetireCycle << "} ";

            cout << endl;

            finishedInstruction.erase(finishedInstruction.begin());
            //   finishedInstruction.erase(finishedInstruction.begin());
                NumberOfInstructions = temp_instruction.instructionNumber + 1;
                endTime = temp_instruction.RetireStart + temp_instruction.RetireCycle;


        }

    }



    bool architectureStages(void){
        bool fe;
        bool de;
        bool rn;
        bool rr;
        bool di;
        bool iq;
        bool ex;
        bool wb;
        bool rt;

        rt = retireStage.execute(&writebackStage.instruction, &rob, &rmt, &executeStage.instruction, &issueQueueStage.instruction,
                                 &dispatchStage.instruction, &registerReadStage.instruction, &renameStage.instruction , &finishedInstruction);
        wb = writebackStage.execute(&executeStage.instruction, &issueQueueStage.instruction, &dispatchStage.instruction,
                                    &registerReadStage.instruction, &renameStage.instruction, &rob);
        ex =  executeStage.execute(&issueQueueStage.instruction, &dispatchStage.instruction,width);
        iq = issueQueueStage.execute(&dispatchStage.instruction, dispatchStage.width, dispatchStage.lastCycle);
        di =  dispatchStage.execute(&registerReadStage.instruction, registerReadStage.lastCycle);
        rr =  registerReadStage.execute(&renameStage.instruction, &rob, renameStage.renameComplete,renameStage.lastCycle);
        rn =  renameStage.execute(&decodeStage.instruction, &rmt, &rob, rob_size,decodeStage.lastCycle);
        de = decodeStage.execute(&fetchStage.instruction, fetchStage.lastCycle);
        fe =   fetchStage.execute(&instruction, &stallCycle, eofFlag); //4 - acceptable width from decode.


          DisplayFinishedInstructions2();
        return eofFlag && rt && wb && ex && iq && di && rr && rn && de && fe ;
    }


    void GetInstructionFromFile(FILE *FP){
        enum stateOfInput currentState = GET_INSTRUCTION_FROM_FILE;
        enum stateOfInput nextstate = GET_INSTRUCTION_FROM_FILE;

        bool pipelineComplete;
        unsigned long int instruction_number = 0;
        unsigned long int instruction_Count = 0;
        int op_type, dest, src1, src2;  // Variables are read from trace file
        unsigned long int pc; // Variable holds the pc read from input file

        int fileReturn;
        int width_counter = 0;

        do{

            switch (currentState){
                case GET_INSTRUCTION_FROM_FILE:{

                    if(getAcceptableWidth() > 0){
                        fileReturn = fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2);

                        if(fileReturn != EOF){
                            setInstructions(pc, op_type, dest, src1, src2, width_counter, instruction_number );
                            nextstate = GET_INSTRUCTION_FROM_FILE;
                            endOfInstructions(false);
                            instruction_number++;
                            instruction_Count++;
                        }

                        if(fileReturn == EOF){
                            for (int i = 0; i < instruction.size(); ++i) {
                                instruction.at(i).fetchStart = instructionLoad; // get the first instruction from the file
                            }
                            instructionLoad++;
                            endOfInstructions(true);
                            nextstate = PROCESS_PIPELINE;
                        }
                    }
                    else if(getAcceptableWidth() <= 0 && fileReturn != EOF) {
                        if(instruction_Count == width){
                            for (int i = 0; i < instruction.size(); ++i) {
                                instruction.at(i).fetchStart = instructionLoad; // get the first instruction from the file
                            }
                            instructionLoad++;
                        }
                        instruction_Count = 0;

                        nextstate = PROCESS_PIPELINE;
                    }
                }
                    break;
                case PROCESS_PIPELINE: {
                    pipelineComplete = architectureStages();
                    if(pipelineComplete)nextstate = DONE;
                    else nextstate = GET_INSTRUCTION_FROM_FILE;
                }
                    break;
                case DONE:break;
            }

            currentState = nextstate;


        }while (currentState != DONE);
    }

};

#endif //SUPERSCALAR_SUPERSCALAR_H
