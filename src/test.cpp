#include <iostream>
#include <vector>

#include "TSimage2xy.h"

int main(void){

    simplexy_t test;
    std::vector<float> tI(100, 0);
    test = tsImage2xy(tI, 10, 10);

    std::cout << "done with test\n";

    return 0;

}