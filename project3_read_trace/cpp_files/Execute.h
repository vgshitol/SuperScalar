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

    bool execute(vector<Instruction> *instructionsVector, vector<Instruction> *dispatchInstructions , int pipeline_width = 1) {

        if(!instructionsVector->empty()) {
            int skipped_count = 0;
            int instr_size = instruction.size();
            int i = 0;
            while ((i < min((width - instr_size),pipeline_width)) && !instructionsVector->empty() && (skipped_count < instructionsVector->size())) {
                Instruction temp_instruction = instructionsVector->at(0+skipped_count);
                // If instruction is ready to execute
                if (temp_instruction.issue_valid && temp_instruction.rs1_ready && temp_instruction.rs2_ready){
                    instruction.push_back(temp_instruction); // get the first instruction from the file
                    instructionsVector->erase(instructionsVector->begin() + skipped_count); // erase the first instruction from the file
                    ++i;
                } else {
                    skipped_count++;
                }
            }
        }

        wakeUpIssueQueue(instructionsVector,dispatchInstructions);


        return instruction.empty();
    }

    void wakeUpIssueQueue(vector<Instruction> *instructionsVector,vector<Instruction> *dispatchInstructions  ){
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

                for (int j = 0; j < dispatchInstructions->size(); ++j) {
                    if(dispatchInstructions->at(j).rs1 == instruction.at(i).dest){
                        dispatchInstructions->at(j).rs1_ready = true;
                    }

                    if(dispatchInstructions->at(j).rs2 == instruction.at(i).dest){
                        dispatchInstructions->at(j).rs2_ready = true;
                    }
                }
            }
        }
    }
};
#endif //SUPERSCALAR_EXECUTE_H
