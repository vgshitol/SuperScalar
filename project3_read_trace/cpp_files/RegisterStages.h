//
// Created by Vishal Shitole on 2018-12-19.
//

#ifndef SUPERSCALAR_REGISTERSTAGES_H
#define SUPERSCALAR_REGISTERSTAGES_H

#include "Instruction.h"
#include <vector>

using namespace std;

class RegisterStages{
private:
    vector <Instruction> instruction;
    bool lastCycle;
    int width;

public:
    const vector<Instruction> &getInstruction() const {
        return instruction;
    }

    void setInstruction(const vector<Instruction> &instruction) {
        RegisterStages::instruction = instruction;
    }

    bool isLastCycle() const {
        return lastCycle;
    }

    void setLastCycle(bool lastCycle) {
        RegisterStages::lastCycle = lastCycle;
    }

    int getWidth() const {
        return width;
    }

    void setWidth(int width) {
        RegisterStages::width = width;
    }

    bool registerInstructions(bool lastCycle = false){
        int numberOfInstructionsInLoop;
        int instr_size = instruction.size();

        setLastCycle(lastCycle);

        if(lastCycle && instructionsVector->size() < getWidth()) numberOfInstructionsInLoop = instructionsVector->size();
        else numberOfInstructionsInLoop = width - instr_size;

        if(!instructionsVector->empty() && instruction.empty()) {

            for (int i = 0; i < numberOfInstructionsInLoop; ++i) {
                instructionsVector->at(0).fetchStart += *stallCycle;
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }
    }
};
#endif //SUPERSCALAR_REGISTERSTAGES_H
