//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_DISPATCH_H
#define SUPERSCALAR_DISPATCH_H
#include "vector"
#include "Instruction.h"

using namespace std;

class Dispatch{
public:
    vector <Instruction> instruction;
    int width;
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Dispatch::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector, int acceptableIssueQueueRegisterWidth) {
        if(instructionsVector->empty() || (acceptableIssueQueueRegisterWidth == 0)){
            // Do Nothing
        }

        else if(!instructionsVector->empty()) {
            for (int i = 0; i < min(acceptableIssueQueueRegisterWidth,width); ++i) {
                instructionsVector->at(0).DispatchCycle++;
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                acceptable_width = i + 1;
            }
        }

        else if((acceptableIssueQueueRegisterWidth < width) && !instructionsVector->empty()) {
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).DispatchCycle++; // get the first instruction from the file
            }
        }
    }
};
#endif //SUPERSCALAR_DISPATCH_H
