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

    bool execute(vector<Instruction> *instructionsVector) {
        if(!instructionsVector->empty()) {
            int instr_size = instruction.size();
            for (int i = 0; i < width - instr_size; ++i) {
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
