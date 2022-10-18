#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

#include "TSimage2xy.h"

std::vector<float> char2floatConv(std::vector<int16_t> inData);
void testIndex(std::vector<float> imgData);

template<typename T> double getAverage(std::vector<T> const& v);
template<typename TT> std::vector<TT> getHalfCol(std::vector<TT> const& dataVec, int offset, int numRows, int width);

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
    testIndex(tI);
    test = tsImage2xy(tI, naxes[0], naxes[1]);

    // std::cout << "done with test\n";
    std::cout << std::flush << std::endl;

    for(int ii = 0; ii<test.npeaks; ii++){
        
        std::cout << test.x[ii] << ", " << test.y[ii] << ", " << test.flux[ii] << ", " << test.background[ii] << "\n";
        
    }

    return 0;

}

void testIndex(std::vector<float> imgData){

    int width = 1024;
    int indNumRows = 512;
    int colOffset = 0;

    // std::vector<float> halfCol(512, 0);

    // for(int ii = 0; ii < indNumRows; ii++){
        
    //     indArray = ii * 1024 + colOffset;
    //     halfCol[ii] = imgData[indArray];
    //     // std::cout << halfCol[ii] << "\n";
    // }

    std::vector<int> inddata(width, 0);
    std::iota(inddata.begin(), inddata.end(), 0);
    
    std::vector<float> halfCol = getHalfCol(imgData, colOffset, indNumRows, width);

    double avg = getAverage(halfCol);
    std::cout << avg << "\n";
}

template<typename TT>
std::vector<TT> getHalfCol(std::vector<TT> const& dataVec, int offset, int numRows, int width){

    std::vector<TT> halfCol(numRows, 0);
    // std::vector<TT>::iterator it;
    // auto itData = dataVec.begin();
    int indArray = 0;
    int ii = 0;

    for(auto itData = dataVec.begin(); itData != dataVec.end(); std::advance(itData, width), ii++){
        halfCol[ii] = *itData;
    }

    // for(int ii = 0; ii < numRows; ii++){
    //     indArray = ii * width + offset;
    //     halfCol[ii] = dataVec[indArray];
    // }

    return halfCol;
}

template<typename T>
double getAverage(std::vector<T> const& v) {
    if (v.empty()) {
        return 0;
    }
 
    return std::reduce(v.begin(), v.end(), 0.0) / v.size();
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