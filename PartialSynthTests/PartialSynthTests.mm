//
//  PartialSynthTests.m
//  PartialSynthTests
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#import <XCTest/XCTest.h>

#import "SinePartial.hpp"
#import "SineSynth.hpp"

@interface PartialSynthTests : XCTestCase

@end

@implementation PartialSynthTests

-(void)testBasic
{
    SinePartial sp;
    
    sp.set<SinePartial::pAmp>(0.5);
    sp.set<SinePartial::pFreq>(440);
    
    float* b = new float[512];
    
    sp.process(512, b);
    

    
    SineSynth s;
    
    SinePartialData d(1);
    d.amp[0] = 0.5;
    d.freq[0] = 440;
    
    s.setData(d);
    s.process(b, 512);
    
        delete[] b;
}

@end
