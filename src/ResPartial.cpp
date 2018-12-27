//
//  ResPartial.cpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include "ResPartial.hpp"


ResPartial::ResPartial()
{
    reset();

    par<pGain>().setSmooth(.9997);
    par<pFreq>().setSmooth(0);
    par<pDecay>().setSmooth(0.);

    _busy = false;

    _freq = par<pFreq>().valuePtr(); //input[0];
    _decay = par<pDecay>().valuePtr(); //input[1];
    _gain = par<pGain>().valuePtr(); //input[2];
}

void ResPartial::processParams(size_t s)
{
    while (s--) {
        par<pFreq>().process();
        par<pDecay>().process();
        par<pGain>().process();
    }
}

void ResPartial::process(size_t s, const float* in_buffer, float* out_buffer)
{
    const float* b_in = in_buffer;
    float* b_out = out_buffer;

    int size = (int)s;
    while (size--) {
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

void ResPartial::updateFreq()
{
    _cTemp1 = (fConst0 * (float)*_freq); //input0);

    float s = _sin(_cTemp1);

    _cTemp2 = -s;
    _cTemp3 = _cos(_cTemp1);

    _dTemp2 = *_decay * s;
    _dTemp3 = *_decay * _cTemp3;
}

void ResPartial::reset()
{
    for (int i = 0; i < 2; i++) {
        fRec0[i] = 0;
        fRec1[i] = 0;
        fRec2[i] = 0;
        fRec3[i] = 0;
    }
}
