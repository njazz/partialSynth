//
//  SinePartial.cpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include "SinePartial.hpp"

//float fastSinOsc(float f)
//{
//    return sinf(f);
//}

//template<>
//void SinePartial::set<SinePartial::pFreq>(float f)
//{
//    _freq.setValue(f);
//}
//
//template<>
//void SinePartial::set<SinePartial::pAmp>(float f)
//{
//    _amp.setValue(f);
//}


SinePartial::SinePartial()
{
    par<pFreq>().setSmooth(0);
}

void SinePartial::process(size_t s, float* buffer)
{
    float* b = buffer;
    while (s--) {
        par<pFreq>().process();
        par<pAmp>().process();

        *b += sinOsc(par<pFreq>().value()) * par<pAmp>().value();
        b++;

        if (par<pAmp>().value() < 0.000001)
            _busy = false;
    }
}
