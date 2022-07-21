/*
 # This file is part of the Astrometry.net suite.
 # Licensed under a 3-clause BSD style license - see LICENSE
 */
#include "TSimage2xy.h"

simplexy_t tsImage2xy(std::vector<float> image, int ww, int hh){

    simplexy_t sparams;
    simplexy_t* params = &sparams;
    memset(params, 0, sizeof(simplexy_t));

    simplexy_fill_in_defaults(params);
    params->image = (float *) malloc(ww * hh * sizeof(float));

    params->dpsf = 3;
    params->maxsize = 512;
    params->dlim = 30;
    params->plim = 5;

    params->image = &image[0];
    params->nx = ww;
    params->ny = hh;

    int downsample = 2;

    if ( image2xy_run(params, downsample, 0) ){
        std::cout << "error" << std::endl;
    }

    return sparams;

}