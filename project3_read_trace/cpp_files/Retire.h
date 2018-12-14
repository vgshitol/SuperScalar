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


    bool execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> * rob,
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

            // find the first instruction that contains the destination in rob 0.

            for (unsigned long inst_num = 0; inst_num < instruction.size(); ++inst_num) {
                if (instruction.at(inst_num).dest == 100) {
                    instruction.at(inst_num).dest = rob->at(0).dest;
                    rob->erase(rob->begin());

                    for (int j = 0; j < instruction.size(); ++j) {
                        for (int k = 0; k < rob->size(); ++k) {
                            if(instruction.at(j).rs1 == k + 100) {
                                instruction.at(j).rs1 = rob->at(k).dest;
                            }
                            if(instruction.at(j).rs2 == k + 100) {
                                instruction.at(j).rs2 = rob->at(k).dest;
                            }
                            if(instruction.at(j).dest == k + 100) {
                                instruction.at(j).dest = rob->at(k).dest;
                            }
                        }
                    }
                    finishedInstruction->push_back(instruction.at(inst_num)); // get the first instruction from the file
                    instruction.erase(instruction.begin()+ inst_num);
                }
            }

        int count_rob_ready = 0;
        if(!instruction.empty()){
            //decrement all the registers inorder to map them correctly instead of circular logic
            while ((count_rob_ready < rob->size()) && (rob->at(count_rob_ready).ready == 1) && !rob->empty() && count_rob_ready < width){
                // 0 fu{0} src{29,14} dst{-1} FE{0,1} DE{1,1} RN{2,1} RR{3,1} DI{4,1} IS{5,1} EX{6,1} WB{7,1} RT{8,1}

                // Go to each place from rename to rob and change the rob index to be erased to its actual name. rs1 and rs2 to be updated.
                for (int j = 0; j < executeInstructions->size(); ++j) {
                    for (int k = 0; k < rob->size(); ++k) {
                        if(executeInstructions->at(j).rs1 == k + 100) {
                            executeInstructions->at(j).rs1--;
                        }
                        if(executeInstructions->at(j).rs2 == k + 100) {
                            executeInstructions->at(j).rs2--;
                        }
                        if(executeInstructions->at(j).dest == k + 100) {
                            executeInstructions->at(j).dest--;
                        }
                    }

//                    if(executeInstructions->at(j).rs1 ==  100) {
//                        executeInstructions->at(j).rs1 = rob->at(0).dest;
//                    }
//                    if(executeInstructions->at(j).rs2 == 100) {
//                        executeInstructions->at(j).rs2 = rob->at(0).dest;
//                    }
//                    if(executeInstructions->at(j).dest == 100) {
//                        executeInstructions->at(j).dest = rob->at(0).dest;
//                    }
                }

                for (int j = 0; j < issueQueueInstructions->size(); ++j) {
                    for (int k = 1; k < rob->size(); ++k) {
                        if(issueQueueInstructions->at(j).rs1 == k + 100) {
                            issueQueueInstructions->at(j).rs1--;
                        }
                        if(issueQueueInstructions->at(j).rs2 == k + 100) {
                            issueQueueInstructions->at(j).rs2--;
                        }
                        if(issueQueueInstructions->at(j).dest == k + 100) {
                            issueQueueInstructions->at(j).dest--;
                        }
                    }
//                    if(issueQueueInstructions->at(j).rs1 ==  100) {
//                        issueQueueInstructions->at(j).rs1 = rob->at(0).dest;
//                    }
//                    if(issueQueueInstructions->at(j).rs2 == 100) {
//                        issueQueueInstructions->at(j).rs2 = rob->at(0).dest;
//                    }
//                    if(issueQueueInstructions->at(j).dest == 100) {
//                        issueQueueInstructions->at(j).dest = rob->at(0).dest;
//                    }

                }

                for (int j = 0; j < dispatchInstructions->size(); ++j) {
                    for (int k = 1; k < rob->size(); ++k) {
                        if(dispatchInstructions->at(j).rs1 == k + 100) {
                            dispatchInstructions->at(j).rs1--;
                        }
                        if(dispatchInstructions->at(j).rs2 == k + 100) {
                            dispatchInstructions->at(j).rs2--;
                        }
                        if(dispatchInstructions->at(j).dest == k + 100) {
                            dispatchInstructions->at(j).dest--;
                        }
                    }
//                    if(dispatchInstructions->at(j).rs1 ==  100) {
//                        dispatchInstructions->at(j).rs1 = rob->at(0).dest;
//                    }
//                    if(dispatchInstructions->at(j).rs2 == 100) {
//                        dispatchInstructions->at(j).rs2 = rob->at(0).dest;
//                    }
//                    if(dispatchInstructions->at(j).dest == 100) {
//                        dispatchInstructions->at(j).dest = rob->at(0).dest;
//                    }

                }

                for (int j = 0; j < registerReadInstructions->size(); ++j) {
                    for (int k = 1; k < rob->size(); ++k) {
                        if(registerReadInstructions->at(j).rs1 == k + 100) {
                            registerReadInstructions->at(j).rs1--;
                        }
                        if(registerReadInstructions->at(j).rs2 == k + 100) {
                            registerReadInstructions->at(j).rs2--;
                        }
                        if(registerReadInstructions->at(j).dest == k + 100) {
                            registerReadInstructions->at(j).dest--;
                        }
                    }
//                    if(registerReadInstructions->at(j).rs1 ==  100) {
//                        registerReadInstructions->at(j).rs1 = rob->at(0).dest;
//                    }
//                    if(registerReadInstructions->at(j).rs2 == 100) {
//                        registerReadInstructions->at(j).rs2 = rob->at(0).dest;
//                    }
//                    if(registerReadInstructions->at(j).dest == 100) {
//                        registerReadInstructions->at(j).dest = rob->at(0).dest;
//                    }
                }

                for (int j = 0; j < renameInstructions->size(); ++j) {
                    for (int k = 0; k < rob->size(); ++k) {
                        if(renameInstructions->at(j).rs1 == k + 100) {
                            renameInstructions->at(j).rs1--;
                        }
                        if(renameInstructions->at(j).rs2 == k + 100) {
                            renameInstructions->at(j).rs2--;
                        }
                        if(renameInstructions->at(j).dest == k + 100) {
                            renameInstructions->at(j).dest--;
                        }
                    }
//                    if(renameInstructions->at(j).rs1 ==  100) {
//                        renameInstructions->at(j).rs1 = rob->at(0).dest;
//                    }
//                    if(renameInstructions->at(j).rs2 == 100) {
//                        renameInstructions->at(j).rs2 = rob->at(0).dest;
//                    }
//                    if(renameInstructions->at(j).dest == 100) {
//                        renameInstructions->at(j).dest = rob->at(0).dest;
//                    }
                }
                count_rob_ready++;
            }
        }




            retire_count++;


        }

        return instruction.empty();

    }
};
#endif //SUPERSCALAR_RETIRE_H
