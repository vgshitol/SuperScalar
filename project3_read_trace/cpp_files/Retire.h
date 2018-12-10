//
// Created by Vishal Shitole on 12/10/18.
//

#ifndef SUPERSCALAR_RETIRE_H
#define SUPERSCALAR_RETIRE_H
#include "vector"
#include "Instruction.h"
#include "ReorderBuffer.h"
#include "iostream"

using namespace std;

class Retire{
public:
    vector <Instruction> instruction;
    int width;
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Retire::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> * rob) {

        int i = 0;
        while (!instructionsVector->empty()) {
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                i++;
          //      acceptable_width = i;

        }

        // Retire Register is not empty and has instructions to retire.
        if(!instruction.empty()) {
            int count = 0;
            while ((rob->at(instruction.at(0).dest - 100).ready == 1) && (count < width)){
                  instruction.pop_back();
                  cout << " Threw out the instructions";
                  count++;
            }
        }

        // Retire Register has some instructions that are stalled
        for (int i = 0; i < instruction.size(); ++i) {
            instruction.at(i).RetireCycle++; // get the first instruction from the file
        }

    }
};
#endif //SUPERSCALAR_RETIRE_H
