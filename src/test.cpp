#include <iostream>
#include <vector>
#include <string>

#include "TSimage2xy.h"

std::vector<float> char2floatConv(std::vector<int16_t> inData);

int main(int argc, char *argv[]){

    std::string fitsFileName = argv[1];
    int status=0, imgType;
    long naxes[2], fpixel=1;
    fitsfile *pcoFits;

    fits_open_image(&pcoFits, fitsFileName.c_str(), READONLY, &status);
    if (status) fits_report_error(stderr, status);

    fits_get_img_type(pcoFits, &imgType, &status);
    if (status) fits_report_error(stderr, status);

    fits_get_img_size(pcoFits, 2, &naxes[0], &status);
    if (status) fits_report_error(stderr, status);

    long numElems = naxes[0] * naxes[1];
    // std::cout << numElems << "\n";
    std::vector<int16_t> testImage(numElems, 0);

    fits_read_img(pcoFits, TSHORT, fpixel, numElems, 0, &testImage[0], 0, &status);
    if (status) fits_report_error(stderr, status);

    fits_close_file(pcoFits, &status);
    if (status) fits_report_error(stderr, status);

    simplexy_t test;
    std::vector<float> tI(testImage.begin(), testImage.end());
    test = tsImage2xy(tI, naxes[0], naxes[1]);

    // std::cout << "done with test\n";
    std::cout << std::flush << std::endl;

    for(int ii = 0; ii<test.npeaks; ii++){
        
        std::cout << test.x[ii] << ", " << test.y[ii] << ", " << test.flux[ii] << ", " << test.background[ii] << "\n";
        
    }

    return 0;

}


std::vector<float> char2floatConv(std::vector<int16_t> inData){

    std::vector<float> outData;
    outData.resize(inData.size());
    int count = 0;

    for(auto vecIt = inData.begin(); vecIt != inData.end(); vecIt += 2){
        auto i2 = std::next(vecIt, 1);
        // count ++;
        // unsigned char ba[] = {*vecIt, *(vecIt++)};
        // std::copy(reinterpret_cast<const char*>(&ba[0]),
        //           reinterpret_cast<const char*>(&ba[1]),
        //           reinterpret_cast<char*>(&outData[count]));
        outData[count] = ((unsigned short) *vecIt << 8) | (unsigned char) *(i2);
        count++;

    }

    // std::cout << "loop count: " << count << std::endl;
    return outData;
}