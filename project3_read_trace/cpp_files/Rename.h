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
    int acceptable_width;

    int getAcceptable_width() const {
        return acceptable_width;
    }

    void setAcceptable_width(int acceptable_width) {
        Rename::acceptable_width = acceptable_width;
    }

    void execute(vector<Instruction> *instructionsVector, int acceptableRegisterReadRegisterWidth, vector<RMT> * rmt, vector<ReorderBuffer> * rob, int rob_size) {
        if(instructionsVector->empty() || (acceptableRegisterReadRegisterWidth == 0)){
            // Do Nothing
        }

        else if((acceptableRegisterReadRegisterWidth == width) && !instructionsVector->empty()) {
            for (int i = 0; i < acceptableRegisterReadRegisterWidth; ++i) {
                instructionsVector->at(0).renameCycle++;
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                acceptable_width = i + 1;
            }

            // Process the instructions in the Rename Stage.
            for (int j = 0; j < instruction.size() ; ++j) {
                if (instruction.at(j).rs1 != -1) {
                    // if renaming is to be done from Reorder Buffer
                    if (rmt->at(instruction.at(j).rs1).valid) {
                        // assign the register the value in reorder buffer
                        instruction.at(j).rs1 = rmt->at(instruction.at(j).rs1).rob_tag  + 100;

                    } else {
                        // Do nothing meaning take the value from the ARF register.
                    }
                }

                if(instruction.at(j).rs2 != -1){
                    if(rmt->at(instruction.at(j).rs2).valid){
                        // assign the register the value in reorder buffer
                        instruction.at(j).rs2 = rmt->at(instruction.at(j).rs2).rob_tag  + 100;

                    } else {
                        // Do nothing meaning take the value from the ARF register.
                    }
                }

                if(instruction.at(j).dest != -1){
                    // Go to rob table and update the entry there and set its valid bit and assign that value to rmt
                    ReorderBuffer rob_new(instruction.at(j).dest , instruction.at(j).pc );

                    if(rob->size() < rob_size){
                        rob->push_back(rob_new);
                    }

                    rmt->at(instruction.at(j).dest).valid = true;
                    rmt->at(instruction.at(j).dest).rob_tag = static_cast<int>(rob->size() - 1);
                    // assign the register the value in reorder buffer

                    instruction.at(j).dest = rmt->at(instruction.at(j).dest).rob_tag + 100;

                } else {
                    // Do nothing meaning its an instruction that doesnt need to write.
                    ReorderBuffer rob_new(instruction.at(j).dest , instruction.at(j).pc );

                    if(rob->size() < rob_size){
                        rob->push_back(rob_new);
                    }
                }

            }
        }
        else if((acceptableRegisterReadRegisterWidth < width) && !instructionsVector->empty()) {
            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).renameCycle++; // get the first instruction from the file
            }
        }

    }
};

#endif //SUPERSCALAR_RENAME_H
