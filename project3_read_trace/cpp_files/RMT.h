//
// Created by Vishal Shitole on 12/3/18.
//

#ifndef SUPERSCALAR_RMT_H
#define SUPERSCALAR_RMT_H

class RMT{
public:
    bool valid;
    int rob_tag;

public:
    RMT(bool valid = false, int rob_tag = -1){
            this->valid = valid;
            this->rob_tag = rob_tag;
    }
};

#endif //SUPERSCALAR_RMT_H
