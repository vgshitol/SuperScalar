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

    void execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> * rob,
            vector<Instruction> *executeInstructions, vector<Instruction> *issueQueueInstructions,
            vector<Instruction> *dispatchInstructions, vector<Instruction> *registerReadInstructions,
            vector<Instruction> * renameInstructions) {

        while (!instructionsVector->empty()) {
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
        }

        // Retire Register has some instructions that are stalled
        for (int i = 0; i < instruction.size(); ++i) {
            instruction.at(i).RetireCycle++; // get the first instruction from the file
        }

        // Retire Register is not empty and has instructions to retire.
        if(!instruction.empty()) {
            int count = 0;
            while (!instruction.empty() && (rob->at(instruction.at(0).dest - 100).ready == 1) && (count < width)){
                  Instruction temp_instruction = instruction.at(0);
               // 0 fu{0} src{29,14} dst{-1} FE{0,1} DE{1,1} RN{2,1} RR{3,1} DI{4,1} IS{5,1} EX{6,1} WB{7,1} RT{8,1}
               unsigned long decodeTime = temp_instruction.fetchCycle + temp_instruction.instructionNumber;
               unsigned long renameTime = temp_instruction.decodeCycle + decodeTime;
               unsigned long rrTime = temp_instruction.renameCycle + renameTime;
               unsigned long diTime = temp_instruction.registerReadCycle + rrTime;
               unsigned long iqTime = temp_instruction.DispatchCycle + diTime;
               unsigned long exTime = temp_instruction.IssueQueueCycle + iqTime;
               unsigned long wbTime = temp_instruction.ExecuteCycle + exTime;
               unsigned long rtTime = temp_instruction.WriteBackCycle + wbTime;
                cout << temp_instruction.instructionNumber << " ";
                cout << "fu{ " << temp_instruction.op_code << "} ";
                cout << "src{ " << temp_instruction.rs1 << "," << temp_instruction.rs2 << "} ";
                cout << "dst{ " << rob->at(temp_instruction.dest - 100).dest << "} ";
                cout << "FE{" << temp_instruction.instructionNumber << "," << temp_instruction.fetchCycle << "} ";
                cout << "DE{" << decodeTime <<"," << temp_instruction.decodeCycle << "} ";
                cout << "RN{"<< renameTime <<"," << temp_instruction.renameCycle << "} ";
                cout << "RR{"<< rrTime <<"," << temp_instruction.registerReadCycle << "} ";
                cout << "DI{"<< diTime <<"," << temp_instruction.DispatchCycle << "} ";
                cout << "IS{"<< iqTime <<"," << temp_instruction.IssueQueueCycle << "} ";
                cout << "EX{"<< exTime <<"," << temp_instruction.ExecuteCycle << "} ";
                cout << "WB{"<< wbTime <<"," << temp_instruction.WriteBackCycle << "} ";
                cout << "RT{"<< rtTime <<"," << temp_instruction.RetireCycle << "} ";

                cout << endl;

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
                }

                for (int j = 0; j < issueQueueInstructions->size(); ++j) {
                    for (int k = 0; k < rob->size(); ++k) {
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
                }

                for (int j = 0; j < dispatchInstructions->size(); ++j) {
                    for (int k = 0; k < rob->size(); ++k) {
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
                }

                for (int j = 0; j < registerReadInstructions->size(); ++j) {
                    for (int k = 0; k < rob->size(); ++k) {
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
                }


                instruction.erase(instruction.begin());
                rob->erase(rob->begin());
                count++;
            }
        }



    }
};
#endif //SUPERSCALAR_RETIRE_H
