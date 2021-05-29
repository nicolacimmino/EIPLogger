
#ifndef __MIN_MAX_VAL_H__
#define __MIN_MAX_VAL_H__

#include <Arduino.h>
#include "config.h"

template <class T>
class MinMaxVal
{
private:
    T minValue;
    T maxValue;
    T value = NO_VALUE;

public:
    void set(T value);
    T get();
    T getMax();
    T getMin();
};

template class MinMaxVal<float>;
template class MinMaxVal<int>;

#endif