//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_REGISTERREAD_H
#define SUPERSCALAR_REGISTERREAD_H
#include "vector"
#include "Instruction.h"
#include "ReorderBuffer.h"

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

    void execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> *rob) {

        if(!instructionsVector->empty()) {
            int instr_size = instruction.size();
            for (int i = 0; i < width - instr_size; ++i) {
                // Get the readiness from rob
                Instruction temp_instruction = instructionsVector->at(0);

                for (int j = 0; j < rob->size() ; ++j) {
                    if(rob->at(j).dest == temp_instruction.rs1) temp_instruction.rs1_ready = rob->at(j).ready;
                    if(rob->at(j).dest == temp_instruction.rs2) temp_instruction.rs2_ready = rob->at(j).ready;
                }

                instruction.push_back(temp_instruction); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).registerReadCycle++; // get the first instruction from the file
            }

    }
};
#endif //SUPERSCALAR_REGISTERREAD_H
