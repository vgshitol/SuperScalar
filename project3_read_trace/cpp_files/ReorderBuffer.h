//
// Created by Vishal Shitole on 12/3/18.
//

#ifndef SUPERSCALAR_REORDERBUFFER_H
#define SUPERSCALAR_REORDERBUFFER_H

class ReorderBuffer{
public:
    int dest;
    bool ready;
    bool exc;
    bool miss;
    unsigned long pc;

public:
    ReorderBuffer(int dest = -1, unsigned long pc = 0, bool ready = false, bool exc = false, bool miss = false ){
        this->dest = dest;
        this->ready = ready;
        this->exc = exc;
        this->miss = miss;
        this->pc = pc;
    }


    void makeEntry(int dest = -1, unsigned long pc = 0, bool ready = false, bool exc = false, bool miss = false){
        this->dest = dest;
        this->ready = ready;
        this->exc = exc;
        this->miss = miss;
        this->pc = pc;
    }
};

#endif //SUPERSCALAR_REORDERBUFFER_H
