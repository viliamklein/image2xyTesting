#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

#include "TSimage2xy.h"

std::vector<float> char2floatConv(std::vector<int16_t> inData);
void testIndex(std::vector<float> imgData);
void saveFits(std::vector<float> const& dataVec, std::string fname);

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

    saveFits(halfCol, "test.fits");
    // std::cout << avg << "\n";
}

void saveFits(std::vector<float> const& dataVec, std::string fname){

    // std::string fitsFileName = argv[1];
    int status=0, imgType;
    long naxes[2], fpixel[2];
    fitsfile *pcoFits;
    // void * imgPtr = (float *) &dataVec[0];

    naxes[0] = 1024;
    naxes[1] = 1024;
    fpixel[0] = 1;
    fpixel[1] = 1;

    fits_create_file(&pcoFits, fname.c_str(), &status);

    fits_create_img(pcoFits, FLOAT_IMG, 2, &naxes[0], &status);

    // fits_write_pix(pcoFits, TFLOAT, &fpixel, 1024*1024, (float *) &dataVec[0], &status);
    fits_write_pix(pcoFits, TFLOAT, &fpixel[0], dataVec.size(), (float *) &dataVec[0], &status);

    fits_close_file(pcoFits, &status);

}

template<typename TT>
std::vector<TT> getHalfCol(std::vector<TT> const& dataVec, int offset, int numRows, int width){

    std::vector<TT> halfColTop(numRows, 0);
    std::vector<TT> halfColBottom(numRows, 0);
    std::vector<TT> avgRemoved = dataVec;

    std::vector<double> avgT(numRows, 0);
    std::vector<double> avgB(numRows, 0);

    int ii;

    for(int offset = 0; offset < numRows; offset++){

        for(auto itData = dataVec.begin(), ii = 0; itData < dataVec.begin() + numRows*width; std::advance(itData, width), ii++){
            halfColTop[ii] = *(itData + offset);
            halfColBottom[ii] = *(itData + dataVec.size()/2 + offset);
        }

        std::vector<TT> halfColTopNoHeader(&halfColTop[10], &halfColTop[numRows-1]);
        avgT[offset] = getAverage(halfColTopNoHeader);
        avgB[offset] = getAverage(halfColBottom);

        for(auto itData = avgRemoved.begin(), ii = 0; itData < avgRemoved.begin() + numRows*width; std::advance(itData, width), ii++){
            // avgRemoved[ii] = *(itData + offset);
            // avgRemoved[ii] = *(itData + dataVec.size()/2 + offset);

            *(itData + offset) -= avgT[offset];
            *(itData + dataVec.size()/2 + offset) -= avgB[offset];
        }

    }

    // std::vector<TT> halfColTopNoHeader(&halfColTop[10], &halfColTop[ii]);
    // avgT = getAverage(halfColTopNoHeader);
    // avgB = getAverage(halfColBottom);

    // for(auto itData = dataVec.begin(); itData < dataVec.begin() + (width/2)*width; std::advance(itData, width), ii++){
    //     halfCol[ii] = *itData;
    // }
    // double avg = getAverage(halfCol);

    return avgRemoved;
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