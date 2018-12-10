//
// Created by Vishal Shitole on 12/3/18.
//

#ifndef SUPERSCALAR_SUPERSCALAR_H
#define SUPERSCALAR_SUPERSCALAR_H

#include <vector>
#include "ARF.h"
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

using namespace std;

class SuperScalar{
public:
    int rob_size;
    int iq_size;
    int width;
    vector <RMT> rmt;
    vector <ReorderBuffer> rob;
    vector <Instruction> instruction;

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


    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        SuperScalar::acceptable_width = acceptable_width;
    }

    /**
     *
     * @param rob_size
     * @param iq_size
     * @param width
     */
    SuperScalar(int rob_size, int iq_size, int width){
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


        this->decodeStage.setAcceptable_width(width);
        this->renameStage.setAcceptable_width(width);
        this->registerReadStage.setAcceptable_width(width);
        this->dispatchStage.setAcceptable_width(width);
        this->issueQueueStage.setAcceptable_width(iq_size);
        this->executeStage.setAcceptable_width(width*5);
        this->writebackStage.setAcceptable_width(width*5);
        this->retireStage.setAcceptable_width(width);
    }

    void setInstructions(unsigned long pc, int op_code, int dest, int src1, int src2, int width_counter) {
        Instruction temp_instr;
        temp_instr.setInstructionParameters(pc,  op_code,  dest,  src1,  src2);
        instruction.push_back(temp_instr);
    }

    void endOfInstructions(bool eofFlag){
        this->eofFlag = eofFlag;
    }

    void initRMT(){
        rmt.resize(67);
    }




    bool architectureStages(void){

        retireStage.setAcceptable_width(width*5);
        retireStage.execute(&writebackStage.instruction, &rob);
        writebackStage.execute(&executeStage.instruction);
        executeStage.execute(&issueQueueStage.instruction);
        issueQueueStage.execute(&dispatchStage.instruction, executeStage.getAcceptable_width(), dispatchStage.width);
        dispatchStage.execute(&registerReadStage.instruction, issueQueueStage.getAcceptable_width());
        registerReadStage.execute(&renameStage.instruction, dispatchStage.getAcceptable_width());
        renameStage.execute(&decodeStage.instruction, registerReadStage.getAcceptable_width(), &rmt, &rob, rob_size);
        decodeStage.execute(&fetchStage.instruction, renameStage.getAcceptable_width());
        fetchStage.execute(&instruction, decodeStage.getAcceptable_width()); //4 - acceptable width from decode.

        return eofFlag;
    }

};

#endif //SUPERSCALAR_SUPERSCALAR_H
