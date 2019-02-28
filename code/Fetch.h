//
// Created by Vishal Shitole on 12/8/18.
//

#ifndef SUPERSCALAR_FETCH_H
#define SUPERSCALAR_FETCH_H

#include "vector"
#include "Instruction.h"

using namespace std;

class Fetch {
public:
    vector <Instruction> instruction;
    int width;
    bool lastCycle;

    bool execute(vector<Instruction> *instructionsVector,  unsigned long * stallCycle = 0, bool lastCycle = false) {
        // Move the Insrtuctions Forward to the next Stage
        this->lastCycle = lastCycle;
        int numberOfInstructionsInLoop;
        int instr_size = instruction.size();

        if(lastCycle && instructionsVector->size() < width) numberOfInstructionsInLoop = instructionsVector->size();
        else numberOfInstructionsInLoop = width - instr_size;


        if(!instructionsVector->empty() && instruction.empty()) {

            int instr_size = instruction.size();
                for (int i = 0; i < numberOfInstructionsInLoop; ++i) {
                    instructionsVector->at(0).fetchStart +=  *stallCycle;
                    instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                    instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                }

            // fetch cannot stall
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).fetchCycle++; // get the first instruction from the file
            }
        }else {
            *stallCycle += 1;

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).fetchStart++; // get the first instruction from the file
            }

        }



        return instruction.empty();

    }
};


#endif //SUPERSCALAR_FETCH_H
