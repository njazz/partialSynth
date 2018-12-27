//
//  SineSynth.cpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include "SineSynth.hpp"

SineSynth::SineSynth()
{
    for (int i = 0; i < maxNumberOfPartials; i++)
        partials.push_back(new SinePartial);
}
void SineSynth::setData(SinePartialData& data)
{
    muteActivePartials();

    std::vector<int> freeIdx = freePartialIndices();

    int fi = (int)freeIdx.size();
    int s = (data.size < fi) ? (int)data.size : fi;

    for (int i = 0; i < s; i++) {
        int idx = freeIdx[i];
        if (idx > 1024)
            continue;
        partials[idx]->set<SinePartial::pAmp>(data.amp[i]);
        partials[idx]->set<SinePartial::pFreq>(data.freq[i]);
        partials[idx]->setBusy(true);
    }
};

void SineSynth::muteActivePartials()
{
    for (int i = 0; i < maxNumberOfPartials; i++) {
        if (partials[i]->busy()) {
            partials[i]->set<SinePartial::pAmp>(0);
        }
    }
}

void SineSynth::process(float* buffer, size_t s)
{

    for (int i = 0; i < s; i++)
        buffer[i] = 0.;

    for (SinePartial* sp : activePartials())
        sp->process(s, buffer);
};
