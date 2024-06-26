#include "intervalTree.h"

int main(){
    IntervalTree A(20);
    A.changeDefualtValue(0,10);
    A.changeDefualtValue(2,12);
    A.changeDefualtValue(3,8);
    A.changeDefualtValue(5,9);
    A.addContinueValue(0,500);
    return 0;
}