//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_ISSUEQUEUESTAGE_H
#define SUPERSCALAR_ISSUEQUEUESTAGE_H
#include "vector"
#include "Instruction.h"
#include "IssueQueue.h"

using namespace std;

class IssueQueueStage{
public:
    vector <Instruction> instruction;
    int width;
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        IssueQueueStage::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector, int acceptableExecuteRegisterWidth, int dispatch_width) {

        if(!instructionsVector->empty()) {
            for (int i = 0; i < width - instruction.size(); ++i) {

                Instruction temp_instruction = instructionsVector->at(0); // get the first instruction from the file

                // process the temp instruction and load it back to instruction set
                temp_instruction.issue_valid = true;

                instruction.push_back(temp_instruction); // loading the instruction in the instruction set
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).IssueQueueCycle++; // get the first instruction from the file
            }
    }
};
#endif //SUPERSCALAR_ISSUEQUEUESTAGE_H
