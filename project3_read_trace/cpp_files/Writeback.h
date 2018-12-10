//
// Created by Vishal Shitole on 12/10/18.
//

#ifndef SUPERSCALAR_WRITEBACK_H
#define SUPERSCALAR_WRITEBACK_H
#include "vector"
#include "Instruction.h"

using namespace std;

class Writeback{
public:
    vector <Instruction> instruction;
    int width;
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Writeback::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector) {

        if(!instructionsVector->empty()) {
            int i = 0;
            while (i < width - instruction.size()) {
                if (((instructionsVector->at(i).ExecuteCycle == 5) && (instructionsVector->at(i).op_code == 2))
                ||((instructionsVector->at(i).ExecuteCycle == 2) && (instructionsVector->at(i).op_code == 1))
                ||((instructionsVector->at(i).ExecuteCycle == 1) && (instructionsVector->at(i).op_code == 0))){
                    instruction.push_back(instructionsVector->at(i)); // get the first instruction from the file
                    instructionsVector->erase(instructionsVector->begin() + i); // erase the first instruction from the file
                }
                i++;
            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).WriteBackCycle++; // get the first instruction from the file
            }

    }
};
#endif //SUPERSCALAR_WRITEBACK_H
