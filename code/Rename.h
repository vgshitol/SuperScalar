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
    bool renameComplete;
    bool lastCycle;

    bool execute(vector<Instruction> *instructionsVector, vector<RMT> * rmt, vector<ReorderBuffer> * rob, int rob_size, bool lastCycle = false) {

        this->lastCycle = lastCycle;
        int numberOfInstructionsInLoop;
        int instr_size = instruction.size();

        if(lastCycle) numberOfInstructionsInLoop = instructionsVector->size();
        else numberOfInstructionsInLoop = width - instr_size;


        if(!instructionsVector->empty() && instruction.empty()) {
            for (int i = 0; i < numberOfInstructionsInLoop; ++i) {
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                renameComplete = false;
            }
        }

        if(!renameComplete) {
            int robsize = rob->size();
            for (int i = 0; i < instruction.size() && (robsize <= rob_size - width); ++i) {
                // Process the instruction
                if (instruction.at(i).rs1 != -1) {
                    // if renaming is to be done from Reorder Buffer
                    if (rmt->at(instruction.at(i).rs1).valid) {
                        // assign the register the value in reorder buffer
                        instruction.at(i).rs1 = rmt->at(instruction.at(i).rs1).rob_tag;

                    } else {
                        // Do nothing meaning take the value from the ARF register.
                    }
                }

                if(instruction.at(i).rs2 != -1){
                    if(rmt->at(instruction.at(i).rs2).valid){
                        // assign the register the value in reorder buffer
                        instruction.at(i).rs2 = rmt->at(instruction.at(i).rs2).rob_tag;

                    } else {
                        // Do nothing meaning take the value from the ARF register.
                    }
                }

                if(instruction.at(i).dest != -1){
                    // Go to rob table and update the entry there and set its valid bit and assign that value to rmt
                    ReorderBuffer rob_new(instruction.at(i).dest , instruction.at(i).pc );

                    if(rob->size() < rob_size){
                        rob->push_back(rob_new);
                    }

                    rmt->at(instruction.at(i).dest).valid = true;
                    rmt->at(instruction.at(i).dest).rob_tag = static_cast<int>(rob->size() - 1) + 100;
                    // assign the register the value in reorder buffer

                    instruction.at(i).dest = rmt->at(instruction.at(i).dest).rob_tag;

                } else {
                    // Do nothing meaning its an instruction that doesnt need to write.
                    ReorderBuffer rob_new(instruction.at(i).dest , instruction.at(i).pc );

                    if(rob->size() < rob_size){
                        rob->push_back(rob_new);
                    }

                    instruction.at(i).dest = rob->size() - 1 + 100;
                }
                renameComplete = true;

            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).renameCycle++; // get the first instruction from the file
            }

        return instruction.empty();
    }
};

#endif //SUPERSCALAR_RENAME_H
