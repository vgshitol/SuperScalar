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
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Decode::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector, int acceptableRenameRegisterWidth) {
        if(instructionsVector->empty() || (acceptableRenameRegisterWidth == 0)){
            // Do Nothing
        }

        else if((acceptableRenameRegisterWidth == width) && !instructionsVector->empty()) {
            for (int i = 0; i < acceptableRenameRegisterWidth; ++i) {
                instructionsVector->at(0).decodeCycle++;
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                acceptable_width = i + 1;
            }
        }
        else if((acceptableRenameRegisterWidth < width) && !instructionsVector->empty()) {
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).decodeCycle++; // get the first instruction from the file
            }
        }
    }
};
#endif //SUPERSCALAR_DECODE_H
