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
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Fetch::acceptable_width = acceptable_width;
    }



    void execute(vector<Instruction> *instructionsVector) {
        // Move the Insrtuctions Forward to the next Stage
         if(!instructionsVector->empty()) {
             int instr_size = instruction.size();
            for (int i = 0; i < width - instr_size; ++i) {
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }
        //Stall the instructions.
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).fetchCycle++; // get the first instruction from the file
            }

    }
};


#endif //SUPERSCALAR_FETCH_H
