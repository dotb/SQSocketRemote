//
//  SQSocketRemote.cpp
//  
//
//  Created by Bradley Clayton on 7/30/12.
//  Copyright (c) 2012 Squarepolka. All rights reserved.
//


#include "SQSocketRemote.h"

SQSocketRemote::SQSocketRemote(int txDataPin, int pttPin)
{
    _txDataPin = txDataPin;
    _pttPin = pttPin;
    _symbolCount = 0;
    _repeatCount = 0;
    _symbolPosition = 0;
    _repeatPosition = 0;
    _currentBitValue = false;
    isTransmitting = false;
    hasTransmitted = false;
    pinMode(_txDataPin, OUTPUT);
    pinMode(_pttPin, OUTPUT);
}

SQSocketRemote::~SQSocketRemote()
{

}

void SQSocketRemote::turnDevice1On()
{
    sendCommand(command1On, sizeof(command1On) / sizeof(int), repeatBroadcastCount);
}
void SQSocketRemote::turnDevice2On()
{
    sendCommand(command2On, sizeof(command2On) / sizeof(int), repeatBroadcastCount);
}
void SQSocketRemote::turnDevice3On()
{
    sendCommand(command3On, sizeof(command3On) / sizeof(int), repeatBroadcastCount);
}
void SQSocketRemote::turnDevice4On()
{
    sendCommand(command4On, sizeof(command4On) / sizeof(int), repeatBroadcastCount);
}
void SQSocketRemote::turnAllDevicesOff()
{
    sendCommand(commandAllOff, sizeof(commandAllOff) / sizeof(int), repeatBroadcastCount);
}

unsigned int SQSocketRemote::timerDidOverflow()
{
    // If isTransmitting is populated and we still have items to deliver
    if (isTransmitting && _repeatPosition < _repeatCount)
    {
        unsigned long currentDuration = micros() - _txStartTime;
        //if (currentDuration > _symbols[_symbolPosition])
        if (currentDuration > _currentSymbol)
        {
            
            // Advance to the next symbol
            if (++_symbolPosition < _symbolCount)
            {
                // There are still symbols to be transmitted. Start delivery of the next bit.
                _currentBitValue = !_currentBitValue; // Flip the bit
            }
            else
            {
                // No more symbols, start the next repetition.
                _repeatPosition++;
                _symbolPosition = 0;
                _currentBitValue = false;
            }
            _currentSymbol = pgm_read_word_near(_currentSymbols + _symbolPosition);
            _txStartTime = micros(); // Start timing the next symbol
        }
        
        // Deliver the current symbol
        digitalWrite(_txDataPin, _currentBitValue);
        digitalWrite(_pttPin, HIGH);
        
        
        /* Return the time left for the current symbol.
         * A prescaler of 64 works out to 4 us per tick.
         * So if we want 400 us to pass before we are called again
         * we must return 100 ticks to the timer, i.e. divide by 4 */
        currentDuration = micros() - _txStartTime;
        //unsigned long timeLeft = _symbols[_symbolPosition] - currentDuration;
        unsigned long timeLeft = _currentSymbol - currentDuration;
        unsigned int timerPosition = 65535 - timeLeft / 4;
        /*Serial.print("The time left ");
        Serial.println(timeLeft);
        Serial.print("timerPosition ");
        Serial.println(timerPosition);*/
//        delay(1000);
        return timerPosition;
    }
    
    // Stop transmitting
    isTransmitting = false;
    hasTransmitted = true;
    digitalWrite(_pttPin, LOW);

    // There is nothing to transmit, so we return the largest timer value
    return 0;
}

void SQSocketRemote::sendCommand(unsigned int symbols[], int symbolCount, int repeatCount)
{
    if (!isTransmitting)
    {
        // Load the first symbol from program space
        _currentSymbols = symbols;
        _currentSymbol = pgm_read_word_near(_currentSymbols);
        _currentBitValue = false;
        _txStartTime = micros();
        _symbolCount = symbolCount;
        _repeatCount = repeatCount;
        _repeatPosition = 0;
        _symbolPosition = 0;
        isTransmitting = true;
    }
}