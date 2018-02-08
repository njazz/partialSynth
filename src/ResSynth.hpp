//
//  SineSynth.hpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef SineSynth_hpp
#define SineSynth_hpp

#include <stdio.h>

#include "ResPartial.hpp"
#include <vector>

typedef struct ResPartialData {
    float* freq = 0;
    float* decay = 0;
    float* gain = 0;
    size_t size = 0;

    ResPartialData(size_t s)
    {
        freq = new float[s];
        decay = new float[s];
        gain = new float[s];
        size = s;
    }
    ~ResPartialData()
    {
        //        if (freq)
        //            delete[] freq;
        //        if (amp)
        //            delete[] amp;
    }

} ResPartialData;

class ResSynth {
    std::vector<ResPartial*> partials;

    const std::vector<int> freePartialIndices()
    {
        std::vector<int> ret;
        for (int i = 0; i < 1024; i++) {
            if (!partials[i]->busy())
                ret.push_back(i);
        }
        return ret;
    }

    const std::vector<ResPartial*> activePartials()
    {
        std::vector<ResPartial*> ret;
        for (int i = 0; i < 1024; i++) {
            if (partials[i]->busy())
                ret.push_back(partials[i]);
        }
        return ret;
    }

public:
    ResSynth()
    {
        for (int i = 0; i < 1024; i++)
            partials.push_back(new ResPartial);
    }

    void setData(ResPartialData data)
    {
        muteActivePartials();

        std::vector<int> freeIdx = freePartialIndices();

        int fi = (int)freeIdx.size();
        int s = (data.size < fi) ? (int)data.size : fi;

        for (int i = 0; i < s; i++) {
            int idx = freeIdx[i];
            if (idx > 1024)
                continue;
            partials[idx]->set<ResPartial::pGain>(data.gain[i]);
            partials[idx]->set<ResPartial::pDecay>(data.decay[i]);
            partials[idx]->set<ResPartial::pFreq>(data.freq[i]);
            partials[idx]->setBusy(true);
        }
    };

    void muteActivePartials()
    {
        for (int i = 0; i < 1024; i++) {
            if (partials[i]->busy()) {
                partials[i]->set<ResPartial::pGain>(0);
            }
        }
    }

    void process(const float* in_buffer, float* out_buffer, size_t s)
    {

        //        for (int i = 0; i < s; i++)
        //            out_buffer[i] = 0.;

        float* buf = new float[s];
        for (int i = 0; i < s; i++) {
            buf[i] = in_buffer[i];
            out_buffer[i] = 0.;
        }

        for (ResPartial* sp : activePartials())
            sp->process(s, buf, out_buffer);

        delete[] buf;

        //        for (int i=0;i<1024;i++)
        //            partials[i].process(s,buffer);
    };
};

#endif /* SineSynth_hpp */
