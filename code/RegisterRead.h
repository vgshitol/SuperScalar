//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_REGISTERREAD_H
#define SUPERSCALAR_REGISTERREAD_H
#include "vector"
#include "Instruction.h"
#include "ReorderBuffer.h"

using namespace std;

class RegisterRead{
public:
    vector <Instruction> instruction;
    int width;
    bool lastCycle;

    bool execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> *rob, bool renameComplete = false, bool lastCycle = false) {

        this->lastCycle = lastCycle;
        int numberOfInstructionsInLoop;
        int instr_size = instruction.size();

        if(lastCycle) numberOfInstructionsInLoop = instructionsVector->size();
        else numberOfInstructionsInLoop = width - instr_size;

        if(!instructionsVector->empty() && instruction.empty() && renameComplete) {
            for (int i = 0; i < numberOfInstructionsInLoop; ++i) {
                // Get the readiness from rob
                Instruction temp_instruction = instructionsVector->at(0);

                for (int j = 0; j < rob->size() ; ++j) {
                    if(j == temp_instruction.rs1 - 100) temp_instruction.rs1_ready = rob->at(j).ready;
                    if(j == temp_instruction.rs2 - 100) temp_instruction.rs2_ready = rob->at(j).ready;
                }

                instruction.push_back(temp_instruction); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).registerReadCycle++; // get the first instruction from the file
            }
        return instruction.empty();
    }
};
#endif //SUPERSCALAR_REGISTERREAD_H
