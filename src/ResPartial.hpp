//
//  ResPartial.hpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef ResPartial_hpp
#define ResPartial_hpp

#include <stdio.h>

#include "BasePartial.hpp"

#include "math.h"
#include "stdlib.h"

#include "SineTables.h"

class ResPartial : public BasePartial {

private:
    float fRec0[2];
    float fRec2[2];
    float fRec3[2];
    float fRec1[2];
    int fSamplingFreq = 44100;
    float fConst0 = (6.2831855f / fmin(1.92e+05f, fmax(1.0f, (float)fSamplingFreq)));
    //
    SineTable _sin;
    CosTable _cos;

    float* _freq;
    float* _decay;
    volatile float* _gain;

    float _cTemp1 = 0;
    float _cTemp2 = 0;
    float _cTemp3 = 0;

    float _dTemp2 = 0;
    float _dTemp3 = 0;

    inline void compute(const float** input, float** output) //int count,
    {

        const float* input3 = input[0];
        float* output0 = output[0];

        {

            fRec0[0] = ((_dTemp2 * fRec1[1]) + (_dTemp3 * fRec0[1]));
            fRec2[0] = ((_dTemp2 * fRec3[1]) + (_dTemp3 * fRec2[1]));
            fRec3[0] = ((_cTemp2 * fRec2[1]) + ((float)input3[0] + (_cTemp3 * fRec3[1])));
            fRec1[0] = ((_cTemp3 * fRec1[1]) + (fRec3[0] + (_cTemp2 * fRec0[1])));

            output0[0] += (*_gain * fRec0[0]);

            // post processing
            fRec0[1] = fRec0[0];
            fRec1[1] = fRec1[0];
            fRec2[1] = fRec2[0];
            fRec3[1] = fRec3[0];
        }
    }

public:
    ResPartial();

    void processParams(size_t s);

    void process(size_t s, const float* in_buffer, float* out_buffer);

    void updateFreq();

    void reset();

    bool gate = false;

    static const int pFreq = 0;
    static const int pDecay = 1;
    static const int pGain = 2;
};

#endif /* ResPartial_hpp */
