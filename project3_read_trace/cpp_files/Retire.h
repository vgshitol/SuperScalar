//
// Created by Vishal Shitole on 12/10/18.
//

#ifndef SUPERSCALAR_RETIRE_H
#define SUPERSCALAR_RETIRE_H
#include "vector"
#include "Instruction.h"
#include "ReorderBuffer.h"
#include "RMT.h"
#include "iostream"

using namespace std;

class Retire{
public:
    vector <Instruction> instruction;
    int width;


    bool execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> * rob, vector<RMT> * rmt,
                 vector<Instruction> *executeInstructions, vector<Instruction> *issueQueueInstructions,
                 vector<Instruction> *dispatchInstructions, vector<Instruction> *registerReadInstructions,
                 vector<Instruction> * renameInstructions, vector<Instruction> *finishedInstruction ) {

        while (!instructionsVector->empty()) {

            instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
            instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
        }

        // Retire Register has some instructions that are stalled
        for (int i = 0; i < instruction.size(); ++i) {
            instruction.at(i).RetireCycle++; // get the first instruction from the file
        }

        // Retire Register is not empty and has instructions to retire.
        int retire_count = 0;
        while (!instruction.empty() && (rob->at(0).ready == 1) && !rob->empty() && retire_count < width) {
            retire_count++;

            if(rob->at(0).dest != -1){
                if(rmt->at(rob->at(0).dest).rob_tag == 100) rmt->at(rob->at(0).dest).valid = false;
            }

            // find the first instruction that contains the destination in rob 0.
            for (unsigned long inst_num = 0; inst_num < instruction.size(); ++inst_num) {

                if (instruction.at(inst_num).dest == 100) {
                    instruction.at(inst_num).dest = rob->at(0).dest;

                    for (int j = 0; j < instruction.size(); ++j) {
                            if(instruction.at(j).rs1 > 100) instruction.at(j).rs1--;
                            else if(instruction.at(j).rs1 == 100) instruction.at(j).rs1 = rob->at(0).dest;
                            if(instruction.at(j).rs2 > 100) instruction.at(j).rs2--;
                            else if(instruction.at(j).rs2 == 100) instruction.at(j).rs2 = rob->at(0).dest;
                            if(instruction.at(j).dest > 100) instruction.at(j).dest--;
                            else if(instruction.at(j).dest == 100) instruction.at(j).dest = rob->at(0).dest;


                    }

                    rob->erase(rob->begin());



                    for (int j = 0; j < executeInstructions->size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if(executeInstructions->at(j).rs1 == k + retire_count + 100) {
                                executeInstructions->at(j).rs1--;
                            }
                            if(executeInstructions->at(j).rs2 == k + retire_count + 100) {
                                executeInstructions->at(j).rs2--;
                            }
                            if(executeInstructions->at(j).dest == k + retire_count + 100) {
                                executeInstructions->at(j).dest--;
                            }
                        }

                    }

                    for (int j = 0; j < issueQueueInstructions->size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if(issueQueueInstructions->at(j).rs1 == k + retire_count + 100) {
                                issueQueueInstructions->at(j).rs1--;
                            }
                            if(issueQueueInstructions->at(j).rs2 == k + retire_count + 100) {
                                issueQueueInstructions->at(j).rs2--;
                            }
                            if(issueQueueInstructions->at(j).dest == k + retire_count + 100) {
                                issueQueueInstructions->at(j).dest--;
                            }
                        }
                    }

                    for (int j = 0; j < dispatchInstructions->size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if(dispatchInstructions->at(j).rs1 == k + retire_count + 100) {
                                dispatchInstructions->at(j).rs1--;
                            }
                            if(dispatchInstructions->at(j).rs2 == k + retire_count + 100) {
                                dispatchInstructions->at(j).rs2--;
                            }
                            if(dispatchInstructions->at(j).dest == k + retire_count + 100) {
                                dispatchInstructions->at(j).dest--;
                            }
                        }
                    }

                    for (int j = 0; j < registerReadInstructions->size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if(registerReadInstructions->at(j).rs1 == k + retire_count + 100) {
                                registerReadInstructions->at(j).rs1--;
                            }
                            if(registerReadInstructions->at(j).rs2 == k + retire_count + 100) {
                                registerReadInstructions->at(j).rs2--;
                            }
                            if(registerReadInstructions->at(j).dest == k + retire_count + 100) {
                                registerReadInstructions->at(j).dest--;
                            }
                        }
                    }

                    for (int j = 0; j < renameInstructions->size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if(renameInstructions->at(j).rs1 == k + retire_count + 100) {
                                renameInstructions->at(j).rs1--;
                            }
                            if(renameInstructions->at(j).rs2 == k + retire_count + 100) {
                                renameInstructions->at(j).rs2--;
                            }
                            if(renameInstructions->at(j).dest == k + retire_count + 100) {
                                renameInstructions->at(j).dest--;
                            }
                        }
                    }

                    for (int j = 0; j < rmt->size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if (rob->at(k).dest != -1){
                                if(rmt->at(rob->at(k).dest).rob_tag  == k + retire_count + 100) {
                                    rmt->at(rob->at(k).dest).rob_tag--;
                                }
                            }

                        }
                    }

                    finishedInstruction->push_back(instruction.at(inst_num)); // get the first instruction from the file
                    instruction.erase(instruction.begin()+ inst_num);

                }
            }
        }



        return instruction.empty();

    }
};
#endif //SUPERSCALAR_RETIRE_H
