//
//  BasePartial.hpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef BasePartial_hpp
#define BasePartial_hpp

#include <stdio.h>

class PartialParameter {
    float _value = 0;
    float _previous = 0;
    float _smooth = 0.98;
    float _nsmooth = .02;

public:
    inline void process()
    {
        _previous = _previous * _smooth + _value * _nsmooth;

    }

    inline void process(size_t s)
    {
        while (s--) {
            process();
        }
    }

    void setValue(float v) { _value = v; process();}
    float value() { return _previous; }

    void setSmooth(float v)
    {
        _smooth = v;
        _nsmooth = 1 - v;
    }
    float smooth() { return _smooth; }
};

class BasePartial {
protected:
    bool _busy = false;

public:
    void process(size_t s) {}

    bool busy() { return _busy; }
    void setBusy(bool b) {_busy = b;}
};

#endif /* BasePartial_hpp */
