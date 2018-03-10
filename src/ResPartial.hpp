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

class ResPartial : public BasePartial {

private:
    float fRec0[2];
    float fRec2[2];
    float fRec3[2];
    float fRec1[2];
    int fSamplingFreq = 44100;
    float fConst0 = (6.2831855f / fmin(1.92e+05f, fmax(1.0f, (float)fSamplingFreq)));

    inline
    void compute(const float** input, float** output) //int count,
    {
        float input0 = par<pFreq>().value(); //input[0];
        float input1 = par<pDecay>().value(); //input[1];
        float input2 = par<pGain>().value(); //input[2];

        const float* input3 = input[0];
        float* output0 = output[0];

        {
            float fTemp0 = input1;
            float fTemp1 = (fConst0 * (float)input0);
            float fTemp2 = sinf(fTemp1);
            float fTemp3 = cosf(fTemp1);
            fRec0[0] = (fTemp0 * ((fTemp2 * fRec1[1]) + (fTemp3 * fRec0[1])));
            fRec2[0] = (fTemp0 * ((fTemp2 * fRec3[1]) + (fTemp3 * fRec2[1])));
            float fTemp4 = (0 - fTemp2);
            fRec3[0] = ((fTemp4 * fRec2[1]) + ((float)input3[0] + (fTemp3 * fRec3[1])));
            fRec1[0] = ((fTemp3 * fRec1[1]) + (fRec3[0] + (fTemp4 * fRec0[1])));
            output0[0] += (float)((float)input2 * fRec0[0]);
            // post processing
            fRec1[1] = fRec1[0];
            fRec3[1] = fRec3[0];
            fRec2[1] = fRec2[0];
            fRec0[1] = fRec0[0];
        }
    }

public:
    ResPartial()
    {
        reset();

        par<pGain>().setSmooth(.9997);
        par<pFreq>().setSmooth(0);
        par<pDecay>().setSmooth(0.98);

        _busy = false;
    }

    void processParams(size_t s)
    {
        while (s--) {
            par<pFreq>().process();
            par<pDecay>().process();
            par<pGain>().process();
        }
    }

    void process(size_t s, const float* in_buffer, float* out_buffer)
    {
        const float* b_in = in_buffer;
        float* b_out = out_buffer;

        while (s--) {
            par<pFreq>().process();
            par<pDecay>().process();
            par<pGain>().process();

            compute(&b_in, &b_out);
            b_in++;
            b_out++;


        }
        
        if (par<pGain>().value() < 0.000001)
            if (gate) {
                reset();
                _busy = false;
                gate = false;
            }
    }

    void reset()
    {
        for (int i = 0; i < 2; i++) {
            fRec0[i] = 0;
            fRec1[i] = 0;
            fRec2[i] = 0;
            fRec3[i] = 0;
        }
    }

    bool gate;

    static const int pFreq = 0;
    static const int pDecay = 1;
    static const int pGain = 2;
};

#endif /* ResPartial_hpp */
