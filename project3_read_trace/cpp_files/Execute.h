//
// Created by Vishal Shitole on 12/10/18.
//

#ifndef SUPERSCALAR_EXECUTE_H
#define SUPERSCALAR_EXECUTE_H
#include "vector"
#include "Instruction.h"

using namespace std;

class Execute{
public:
    vector <Instruction> instruction;
    int width;
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Execute::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector) {

        if(!instructionsVector->empty()) {
            int skipped_count = 0;
            int instr_size = instruction.size();
            for (int i = 0; (i < width - instr_size) && !instructionsVector->empty(); ++i) {
                Instruction temp_instruction = instructionsVector->at(0);
                // If instruction is ready to execute
                if (temp_instruction.issue_valid && temp_instruction.rs1_ready && temp_instruction.rs2_ready){
                    instruction.push_back(temp_instruction); // get the first instruction from the file
                    instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                } else {
                    instructionsVector->push_back(temp_instruction); // put the first instruction from the file to the last
                    instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                    skipped_count++;
                }
            }
        }

        // stall logic
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).ExecuteCycle++; // get the first instruction from the file
                if (((instruction.at(i).ExecuteCycle == 5) && (instruction.at(i).op_code == 2))
                    ||((instruction.at(i).ExecuteCycle == 2) && (instruction.at(i).op_code == 1))
                    ||((instruction.at(i).ExecuteCycle == 1) && (instruction.at(i).op_code == 0))){

                    // Wake up the IQ
                    for (int j = 0; j < instructionsVector->size(); ++j) {
                        if(instructionsVector->at(j).rs1 == instruction.at(i).dest){
                            instructionsVector->at(j).rs1_ready = true;
                        }

                        if(instructionsVector->at(j).rs2 == instruction.at(i).dest){
                            instructionsVector->at(j).rs2_ready = true;
                        }
                    }
                }
            }

    }
};
#endif //SUPERSCALAR_EXECUTE_H
