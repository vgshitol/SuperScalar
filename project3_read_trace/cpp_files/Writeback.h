//
// Created by Vishal Shitole on 12/10/18.
//

#ifndef SUPERSCALAR_WRITEBACK_H
#define SUPERSCALAR_WRITEBACK_H
#include "vector"
#include "Instruction.h"
#include "ReorderBuffer.h"

using namespace std;

class Writeback{
public:
    vector <Instruction> instruction;
    int width;

    bool execute(vector<Instruction> *instructionsVector, vector<Instruction> *issueQueueInstructions,
            vector<Instruction> *dispatchInstructions, vector<Instruction> *registerReadInstructions,
            vector<Instruction> *renameInstructions, vector<ReorderBuffer> * rob) {

        if(!instructionsVector->empty()) {
            int i = 0;
            int skipped_count = 0;
            int instr_size = instruction.size();
            int executeInstrSize = instructionsVector->size();
            while ((i < min((width - instr_size), executeInstrSize)) && !instructionsVector->empty()) {
                Instruction temp_instruction = instructionsVector->at(0);

                if (((instructionsVector->at(0).ExecuteCycle == 5) && (instructionsVector->at(0).op_code == 2))
                ||((instructionsVector->at(0).ExecuteCycle == 2) && (instructionsVector->at(0).op_code == 1))
                ||((instructionsVector->at(0).ExecuteCycle == 1) && (instructionsVector->at(0).op_code == 0))){
                    // process instruction, wake up the ready bits from everywhere. Execution, IssueQueue, RegisterRead, ROB
                    int destreg;
                    if(temp_instruction.dest >= 100 ) {
                        rob->at(temp_instruction.dest - 100).ready = true; // find rob tag in rob
                       destreg = rob->at(temp_instruction.dest - 100).dest;
                    } else {
                        destreg = temp_instruction.dest;
                    }


                    for (int j = 0; j < instructionsVector->size() ; ++j) {
                            if(instructionsVector->at(j).rs1 == temp_instruction.dest) {
                                instructionsVector->at(j).rs1_ready = true;
                                instructionsVector->at(j).rs1 = destreg;
                            }
                            if(instructionsVector->at(j).rs2 == temp_instruction.dest) {
                                instructionsVector->at(j).rs2 = destreg;
                                instructionsVector->at(j).rs2_ready = true;
                            }
                    }
                    for (int j = 0; j < issueQueueInstructions->size() ; ++j) {
                            if(issueQueueInstructions->at(j).rs1 == temp_instruction.dest) {
                                issueQueueInstructions->at(j).rs1 = destreg;
                                issueQueueInstructions->at(j).rs1_ready = true;
                            }
                            if(issueQueueInstructions->at(j).rs2 == temp_instruction.dest) {
                                issueQueueInstructions->at(j).rs2 = destreg;
                                issueQueueInstructions->at(j).rs2_ready = true;
                            }
                    }
                    for (int j = 0; j < dispatchInstructions->size() ; ++j) {
                            if(dispatchInstructions->at(j).rs1 == temp_instruction.dest) {
                                dispatchInstructions->at(j).rs1 = destreg;
                                dispatchInstructions->at(j).rs1_ready = true;
                            }
                            if(dispatchInstructions->at(j).rs2 == temp_instruction.dest) {
                                dispatchInstructions->at(j).rs2 = destreg;
                                dispatchInstructions->at(j).rs2_ready = true;
                            }
                    }
                    for (int j = 0; j < registerReadInstructions->size() ; ++j) {
                            if(registerReadInstructions->at(j).rs1 == temp_instruction.dest) {
                                registerReadInstructions->at(j).rs1 = destreg;
                                registerReadInstructions->at(j).rs1_ready = true;
                            }
                            if(registerReadInstructions->at(j).rs2 == temp_instruction.dest) {
                                registerReadInstructions->at(j).rs2 = destreg;
                                registerReadInstructions->at(j).rs2_ready = true;
                            }
                    }

                    for (int j = 0; j < renameInstructions->size() ; ++j) {
                        if(renameInstructions->at(j).rs1 == temp_instruction.dest) {
                            renameInstructions->at(j).rs1 = destreg;
                            renameInstructions->at(j).rs1_ready = true;
                        }
                        if(renameInstructions->at(j).rs2 == temp_instruction.dest) {
                            renameInstructions->at(j).rs2 = destreg;
                            renameInstructions->at(j).rs2_ready = true;
                        }
                    }




                    instruction.push_back(temp_instruction); // get the first instruction from the file
                    instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file

                } else {
                    instructionsVector->push_back(temp_instruction); // put the first instruction from the file to the last
                    instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                    skipped_count++;
                }
                i++;
            }
        }

            for (int i = 0; i < instruction.size(); ++i) {
                instruction.at(i).WriteBackCycle++; // get the first instruction from the file
            }

        return instruction.empty();

    }
};
#endif //SUPERSCALAR_WRITEBACK_H
