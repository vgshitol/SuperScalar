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

    bool execute(vector<Instruction> *instructionsVector, unsigned long int decodeReady = true) {
        // Move the Insrtuctions Forward to the next Stage


        if(!instructionsVector->empty() && decodeReady) {

            int instr_size = instruction.size();
            for (int i = 0; i < width - instr_size; ++i) {
                instructionsVector->at(0).fetchStart =  instructionsVector->at(0).timingCycle;
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file

            }

            //Stall the instructions.
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).fetchCycle++; // get the first instruction from the file
            }
        } else if (!decodeReady){
            for (int i = 0; i < instructionsVector->size(); ++i) {
                instructionsVector->at(i).timingCycle++; // get the first instruction from the file
            }
        }


        return instruction.empty();

    }
};


#endif //SUPERSCALAR_FETCH_H
