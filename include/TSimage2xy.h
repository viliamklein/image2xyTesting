#ifndef TSIMAGE2XY
#define TSIMAGE2XY

#include <iostream>
#include <iomanip>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>

extern "C" {
#include "os-features.h"
#include "image2xy-files.h"
#include "image2xy.h"
#include "log.h"
#include "errors.h"
#include "ioutils.h"
#include "cfitsutils.h"
}

simplexy_t tsImage2xy(std::vector<float> image, int ww, int hh);

#endif