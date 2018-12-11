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

    void execute(vector<Instruction> *instructionsVector, vector<ReorderBuffer> * rob) {

        int i = 0;
        while (!instructionsVector->empty()) {
                instruction.push_back(instructionsVector->at(0)); // get the first instruction from the file
                instructionsVector->erase(instructionsVector->begin()); // erase the first instruction from the file
                i++;

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
                          instruction.pop_back();
               // 0 fu{0} src{29,14} dst{-1} FE{0,1} DE{1,1} RN{2,1} RR{3,1} DI{4,1} IS{5,1} EX{6,1} WB{7,1} RT{8,1}
                cout << "0 ";
                cout << "fu{ " << temp_instruction.op_code << "} ";
                cout << "src{ " << temp_instruction.rs1 << "," << temp_instruction.rs2 << "} ";
                cout << "dst{ " << temp_instruction.dest << "} ";
                cout << "FE{0," << temp_instruction.fetchCycle << "} ";
                cout << "DE{1," << temp_instruction.decodeCycle << "} ";
                cout << "RN{2," << temp_instruction.renameCycle << "} ";
                cout << "RR{3," << temp_instruction.registerReadCycle << "} ";
                cout << "DI{4," << temp_instruction.DispatchCycle << "} ";
                cout << "IS{5," << temp_instruction.IssueQueueCycle << "} ";
                cout << "EX{6," << temp_instruction.ExecuteCycle << "} ";
                cout << "WB{7," << temp_instruction.WriteBackCycle << "} ";
                cout << "RT{8," << temp_instruction.RetireCycle << "} ";

                cout << endl;



                  count++;
            }
        }



    }
};
#endif //SUPERSCALAR_RETIRE_H
