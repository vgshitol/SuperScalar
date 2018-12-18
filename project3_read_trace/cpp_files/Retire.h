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

        while (!instructionsVector->empty() && instruction.size() < rob->size()) {

            instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
            instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
        }

        // Retire Register has some instructions that are stalled
        for (int i = 0; i < instruction.size(); ++i) {
            instruction.at(i).RetireCycle++; // get the first instruction from the file
        }

        // Retire Register is not empty and has instructions to retire.
        int retire_count = 0;
        while (!instruction.empty() && (rob->at(0).ready == 1) && !rob->empty() && (retire_count < width)) {

            // find the first instruction that contains the destination in rob 0.
            for (unsigned long inst_num = 0; (inst_num < instruction.size()) && (retire_count < width); inst_num++) {

                if (instruction.at(inst_num).dest == 100) {
                    instruction.at(inst_num).dest = rob->at(0).dest;

                    retire_count++;

                    if(rob->at(0).dest != -1){
                        if(rmt->at(rob->at(0).dest).rob_tag == 100) rmt->at(rob->at(0).dest).valid = false;
                    }

                    for (int j = 0; j < instruction.size(); ++j) {
                            if(instruction.at(j).rs1 > 100) instruction.at(j).rs1--;
                            else if(instruction.at(j).rs1 == 100) instruction.at(j).rs1 = rob->at(0).dest;
                            if(instruction.at(j).rs2 > 100) instruction.at(j).rs2--;
                            else if(instruction.at(j).rs2 == 100) instruction.at(j).rs2 = rob->at(0).dest;
                            if(instruction.at(j).dest > 100) instruction.at(j).dest--;
                            else if(instruction.at(j).dest == 100) instruction.at(j).dest = rob->at(0).dest;
                    }

                    for (int j = 0; j < rmt->size(); ++j) {
                        if((rmt->at(j).rob_tag  > 100) && rmt->at(j).valid) {
                            rmt->at(j).rob_tag--;
                        }
                    }

                    for (int j = 0; j < executeInstructions->size(); ++j) {
                        if(executeInstructions->at(j).rs1 > 100) executeInstructions->at(j).rs1--;
                        else if(executeInstructions->at(j).rs1 == 100) executeInstructions->at(j).rs1 = rob->at(0).dest;
                        if(executeInstructions->at(j).rs2 > 100) executeInstructions->at(j).rs2--;
                        else if(executeInstructions->at(j).rs2 == 100) executeInstructions->at(j).rs2 = rob->at(0).dest;
                        if(executeInstructions->at(j).dest > 100) executeInstructions->at(j).dest--;
                        else if(executeInstructions->at(j).dest == 100) executeInstructions->at(j).dest = rob->at(0).dest;
                    }

                    for (int j = 0; j < issueQueueInstructions->size(); ++j) {
                        if(issueQueueInstructions->at(j).rs1 > 100) issueQueueInstructions->at(j).rs1--;
                        else if(issueQueueInstructions->at(j).rs1 == 100) issueQueueInstructions->at(j).rs1 = rob->at(0).dest;
                        if(issueQueueInstructions->at(j).rs2 > 100) issueQueueInstructions->at(j).rs2--;
                        else if(issueQueueInstructions->at(j).rs2 == 100) issueQueueInstructions->at(j).rs2 = rob->at(0).dest;
                        if(issueQueueInstructions->at(j).dest > 100) issueQueueInstructions->at(j).dest--;
                        else if(issueQueueInstructions->at(j).dest == 100) issueQueueInstructions->at(j).dest = rob->at(0).dest;
                    }

                    for (int j = 0; j < dispatchInstructions->size(); ++j) {
                        if(dispatchInstructions->at(j).rs1 > 100) dispatchInstructions->at(j).rs1--;
                        else if(dispatchInstructions->at(j).rs1 == 100) dispatchInstructions->at(j).rs1 = rob->at(0).dest;
                        if(dispatchInstructions->at(j).rs2 > 100) dispatchInstructions->at(j).rs2--;
                        else if(dispatchInstructions->at(j).rs2 == 100) dispatchInstructions->at(j).rs2 = rob->at(0).dest;
                        if(dispatchInstructions->at(j).dest > 100) dispatchInstructions->at(j).dest--;
                        else if(dispatchInstructions->at(j).dest == 100) dispatchInstructions->at(j).dest = rob->at(0).dest;
                    }

                    for (int j = 0; j < registerReadInstructions->size(); ++j) {
                        if(registerReadInstructions->at(j).rs1 > 100) registerReadInstructions->at(j).rs1--;
                        else if(registerReadInstructions->at(j).rs1 == 100) registerReadInstructions->at(j).rs1 = rob->at(0).dest;
                        if(registerReadInstructions->at(j).rs2 > 100) registerReadInstructions->at(j).rs2--;
                        else if(registerReadInstructions->at(j).rs2 == 100) registerReadInstructions->at(j).rs2 = rob->at(0).dest;
                        if(registerReadInstructions->at(j).dest > 100) registerReadInstructions->at(j).dest--;
                        else if(registerReadInstructions->at(j).dest == 100) registerReadInstructions->at(j).dest = rob->at(0).dest;
                    }

                    for (int j = 0; j < renameInstructions->size(); ++j) {
                        if(renameInstructions->at(j).rs1 > 100) renameInstructions->at(j).rs1--;
                        else if(renameInstructions->at(j).rs1 == 100) renameInstructions->at(j).rs1 = rob->at(0).dest;
                        if(renameInstructions->at(j).rs2 > 100) renameInstructions->at(j).rs2--;
                        else if(renameInstructions->at(j).rs2 == 100) renameInstructions->at(j).rs2 = rob->at(0).dest;
                        if(renameInstructions->at(j).dest > 100) renameInstructions->at(j).dest--;
                        else if(renameInstructions->at(j).dest == 100) renameInstructions->at(j).dest = rob->at(0).dest;
                    }

                    rob->erase(rob->begin());
                    finishedInstruction->push_back(instruction.at(inst_num)); // get the first instruction from the file
                    instruction.erase(instruction.begin()+ inst_num);

                }
            }
        }



        return instruction.empty();

    }
};
#endif //SUPERSCALAR_RETIRE_H
