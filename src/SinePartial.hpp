//
//  SinePartial.hpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef SinePartial_hpp
#define SinePartial_hpp

#include <stdio.h>

#include "BasePartial.hpp"

#include "math.h"

#include <stdlib.h>

//float fastSinOsc(float f);

class SinePartial : public BasePartial {
    
    unsigned long _counter = 0;
    float _sampleRate = 44100;

    PartialParameter _param[2];
    
    template<int I>
    inline
    PartialParameter& par()
    {
        return _param[I];
    };

    //inline
    float sinOsc(float f)
    {
        float ret = sinf(float(_counter)/(_sampleRate/f)*3.1415*2);
        _counter ++;

        return ret;

    }
    
public:
    SinePartial()
    {
        par<pFreq>().setSmooth(0);
    }
    void process(size_t s, float* buffer)
    {
        float* b = buffer;
        while (s--) {
            par<pFreq>().process();
            par<pAmp>().process();

//            if (par<pAmp>().value()<0.000001)
//            {
//                continue;
//            }
            
            *b += sinOsc(par<pFreq>().value()) * par<pAmp>().value();
            b++;

            if (par<pAmp>().value()<0.000001)
                _busy = false;
        }
    }
    
    template<int I>
    void set(float f)
    {
        par<I>().setValue(f);
    };
    
    static const int pFreq = 0;
    static const int pAmp = 1;
    
};

#endif /* SinePartial_hpp */
