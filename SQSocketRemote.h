//
//  SQSocketRemote.h
//  
//
//  Created by Bradley Clayton on 7/30/12.
//  Copyright (c) 2012 Squarepolka. All rights reserved.
//

#ifndef SQSocketRemote_h
#define SQSocketRemote_h

#include "Arduino.h"

// Radio symbols for each device
const static int repeatBroadcastCount = 2;

static unsigned int command1On[] PROGMEM = {9160,304,844,300,840,300,848,880,264,304,844,872,264,304,844,880,264,300,840,876,272,296,844,304,848,292,848,876,268,292,856,292,844,880,264,880,264,300,848,292,844,880,264,880,264,880,276,868,316,248};

static unsigned int command2On[] PROGMEM = {9160,300,848,292,848,304,844,880,264,300,840,880,256,300,836,888,264,304,836,880,264,296,856,292,848,292,856,868,264,304,844,292,848,876,268,876,268,292,856,292,848,880,264,284,856,872,268,868,300,268};

static unsigned int command3On[] PROGMEM = {9160,300,840,308,840,300,848,868,268,304,844,880,264,304,836,876,268,300,848,872,264,292,848,300,848,292,848,876,268,304,844,292,856,872,272,880,264,292,844,292,844,880,264,880,264,300,852,868,316,248};

static unsigned int command4On[] PROGMEM = {9160,292,848,304,844,292,848,880,264,300,848,880,256,312,836,888,264,292,848,876,268,300,848,300,844,292,844,876,268,292,856,292,848,876,268,880,264,300,848,292,856,872,272,868,264,880,272,296,888,260};

static unsigned int commandAllOff[] PROGMEM = {9160,300,840,308,836,304,844,880,264,304,848,876,260,300,848,884,260,300,844,880,264,300,848,292,848,304,836,888,256,300,848,300,852,872,264,872,272,296,844,304,848,876,268,292,856,300,840,300,896,244};

class SQSocketRemote
{
public:
    SQSocketRemote(int txDataPin, int pttPin);
    ~SQSocketRemote();
    void turnDevice1On();
    void turnDevice2On();
    void turnDevice3On();
    void turnDevice4On();
    void turnAllDevicesOff();
    unsigned int timerDidOverflow();
    
    boolean isTransmitting;
    boolean hasTransmitted;
    
private:
    void sendCommand(unsigned int symbols[], int len, int repeatCount);
    
    unsigned int *_currentSymbols;
    unsigned int _currentSymbol;
    unsigned long _txStartTime;
    boolean _currentBitValue;
    int _symbolCount;
    int _repeatCount;
    int _symbolPosition;
    int _repeatPosition;
    int _txDataPin;
    int _pttPin; // HIGH when transmitting
};

#endif