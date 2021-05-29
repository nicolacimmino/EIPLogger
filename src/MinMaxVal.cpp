
#include "MinMaxVal.h"

template <class T>
void MinMaxVal<T>::set(T value)
{
    if (this->value == NO_VALUE)
    {
        this->minValue = value;
        this->maxValue = value;
    }

    this->value = value;

    this->minValue = min(this->value, this->minValue);
    this->maxValue = max(this->value, this->maxValue);
}

template <class T>
T MinMaxVal<T>::get()
{
    return this->value;
}

template <class T>
T MinMaxVal<T>::getMax()
{
    return this->maxValue;
}

template <class T>
T MinMaxVal<T>::getMin()
{
    return this->minValue;
}
