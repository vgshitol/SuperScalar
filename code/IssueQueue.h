//
// Created by Vishal Shitole on 12/3/18.
//

#ifndef SUPERSCALAR_ISSUEQUEUE_H
#define SUPERSCALAR_ISSUEQUEUE_H


class IssueQueue{
public:
    bool valid;
    int dest;
    bool rs1_ready;
    int rs1_tag_value;
    bool rs2_ready;
    int rs2_tag_value;

public:
    IssueQueue(bool valid = false, int dest = -1, bool rs1_ready = false, int rs1_tag_value = -1, bool rs2_ready = false, int rs2_tag_value = -1){
        this->valid = valid;
        this->dest = dest;
        this->rs1_ready = rs1_ready;
        this->rs1_tag_value = rs1_tag_value;
        this->rs2_ready = rs2_ready;
        this->rs2_tag_value = rs2_tag_value;
    }
};

#endif //SUPERSCALAR_ISSUEQUEUE_H
