//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_REGISTERREAD_H
#define SUPERSCALAR_REGISTERREAD_H
#include "vector"
#include "Instruction.h"

using namespace std;

class RegisterRead{
public:
    vector <Instruction> instruction;
    int width;
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        RegisterRead::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector, int acceptableDispatchRegisterWidth) {

        if((acceptableDispatchRegisterWidth == width) && !instructionsVector->empty()) {
            for (int i = 0; i < acceptableDispatchRegisterWidth; ++i) {
                instructionsVector->at(0).registerReadCycle++;
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                acceptable_width = i + 1;
            }
        }

        else if((acceptableDispatchRegisterWidth < width) && !instructionsVector->empty()) {
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).registerReadCycle++; // get the first instruction from the file
            }
        }
    }
};
#endif //SUPERSCALAR_REGISTERREAD_H
