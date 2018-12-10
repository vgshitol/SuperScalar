//
// Created by Vishal Shitole on 12/4/18.
//

#ifndef SUPERSCALAR_INSTRUCTION_H
#define SUPERSCALAR_INSTRUCTION_H

typedef enum InstructionStage{
    FETCH,
    DECODE,
    RENAME,
    REGISTER_READ,
    DISPATCH,
    ISSUE_QUEUE,
    EXECUTE,
    WRITEBACK,
    RETIRE
}stage;

class Instruction{
public:
    unsigned long int pc;
    int op_code;
    int dest;
    int rs1;
    bool rs1_ready;
    int rs2;
    bool rs2_ready;
    bool issue_valid;

    int timingCounter;
    stage currentStage;
    int fetchCycle;
    int decodeCycle;
    int renameCycle;
    int registerReadCycle;
    int DispatchCycle;
    int IssueQueueCycle;
    int ExecuteCycle;
    int WriteBackCycle;
    int RetireCycle;

    void setInstructionParameters(unsigned long pc, int op_code, int dest, int src1, int src2, bool rs1_ready = true, bool rs2_ready = true){
        this->pc = pc;
        this->op_code = op_code;
        this->dest = dest;
        this->rs1 = src1;
        this->rs2 = src2;
        this->fetchCycle = 0;
        this->decodeCycle = 0;
        this->renameCycle = 0;
        this->registerReadCycle = 0;
        this->DispatchCycle = 0;
        this->IssueQueueCycle = 0;
        this->ExecuteCycle = 0;
        this->WriteBackCycle = 0;
        this->RetireCycle = 0;
        this->rs1_ready = rs1_ready;
        this->rs2_ready = rs2_ready;
   }
};
#endif //SUPERSCALAR_INSTRUCTION_H