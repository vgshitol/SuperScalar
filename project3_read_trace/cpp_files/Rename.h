//
// Created by Vishal Shitole on 12/9/18.
//

#ifndef SUPERSCALAR_RENAME_H
#define SUPERSCALAR_RENAME_H
#include "vector"
#include "Instruction.h"
#include "RMT.h"
#include "ReorderBuffer.h"

using namespace std;

class Rename{
public:
    vector <Instruction> instruction;
    int width;

    bool execute(vector<Instruction> *instructionsVector, vector<RMT> * rmt, vector<ReorderBuffer> * rob, int rob_size) {

        if(!instructionsVector->empty() && (rob->size() < rob_size)) {
            int process_width = width - instruction.size();
            int instr_size = instruction.size();
            for (int i = 0; i < width - instr_size; ++i) {

                Instruction temp_instruction = instructionsVector->at(0);

                // Process the instruction
                if (temp_instruction.rs1 != -1) {
                    // if renaming is to be done from Reorder Buffer
                    if (rmt->at(temp_instruction.rs1).valid) {
                        // assign the register the value in reorder buffer
                        temp_instruction.rs1 = rmt->at(temp_instruction.rs1).rob_tag;

                    } else {
                        // Do nothing meaning take the value from the ARF register.
                    }
                }

                if(temp_instruction.rs2 != -1){
                    if(rmt->at(temp_instruction.rs2).valid){
                        // assign the register the value in reorder buffer
                        temp_instruction.rs2 = rmt->at(temp_instruction.rs2).rob_tag;

                    } else {
                        // Do nothing meaning take the value from the ARF register.
                    }
                }

                if(temp_instruction.dest != -1){
                    // Go to rob table and update the entry there and set its valid bit and assign that value to rmt
                    ReorderBuffer rob_new(temp_instruction.dest , temp_instruction.pc );

                    if(rob->size() < rob_size){
                        rob->push_back(rob_new);
                    }

                    rmt->at(temp_instruction.dest).valid = true;
                    rmt->at(temp_instruction.dest).rob_tag = static_cast<int>(rob->size() - 1) + 100;
                    // assign the register the value in reorder buffer

                    temp_instruction.dest = rmt->at(temp_instruction.dest).rob_tag;

                } else {
                    // Do nothing meaning its an instruction that doesnt need to write.
                    ReorderBuffer rob_new(temp_instruction.dest , temp_instruction.pc );

                    if(rob->size() < rob_size){
                        rob->push_back(rob_new);
                    }

                    temp_instruction.dest = rob->size() - 1 + 100;
                }

                instruction.push_back(temp_instruction); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).renameCycle++; // get the first instruction from the file
            }

        return instruction.empty();
    }
};

#endif //SUPERSCALAR_RENAME_H
