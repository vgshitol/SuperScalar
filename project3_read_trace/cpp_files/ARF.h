//
// Created by Vishal Shitole on 12/3/18.
//

#ifndef SUPERSCALAR_ARF_H
#define SUPERSCALAR_ARF_H

class ARF{
private:
    int value;

public:
    ARF(int value = -1){
            this->value = value;

    }

    int getValue(){
        return this->value;
    }

    void setValue(int value = 0){
        this->value = value;
    }


};

#endif //SUPERSCALAR_ARF_H
