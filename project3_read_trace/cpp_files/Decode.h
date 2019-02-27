//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_DECODE_H
#define SUPERSCALAR_DECODE_H
#include "vector"
#include "Instruction.h"

using namespace std;

class Decode{
public:
    vector <Instruction> instruction;
    int width;
    bool lastCycle;

    bool execute(vector<Instruction> *instructionsVector, bool lastCycle = false) {

        this->lastCycle = lastCycle;

        int numberOfInstructionsInLoop;
        int instr_size = instruction.size();

        if(lastCycle) numberOfInstructionsInLoop = instructionsVector->size();
        else numberOfInstructionsInLoop = width - instr_size;

       if(!instructionsVector->empty() && instruction.empty()) {

            for (int i = 0; i < numberOfInstructionsInLoop; ++i) {
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }

        for (int i = 0; i < instruction.size(); ++i) {
            instruction.at(i).decodeCycle++; // get the first instruction from the file
        }

        return instruction.empty();
    }
};
#endif //SUPERSCALAR_DECODE_H
