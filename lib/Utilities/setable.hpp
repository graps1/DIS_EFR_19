#pragma once
#include "Arduino.h"

namespace setable
{
template <typename T>
class Setable
{
  private:
    T current_val,
        new_val,
        incr, _max, _min;

  public:
    Setable(T current_val, T incr, T max, T min) { 
        this->current_val = current_val;
        this->new_val = current_val;
        this->incr = incr;
        this->_max = max;
        this->_min = min;
    }
    
    void addNewVal() { 
        this->new_val = this->new_val + this->incr > this->_max ? this->_min : this->new_val + this->incr; 
    }
    
    void subNewVal() {
        this->new_val = this->new_val - this->incr < this->_min ? this->_max : this->new_val - this->incr; 
    }
    
    void setCurrentVal(T val) { this->current_val = val; }
    T getCurrentVal() { return this->current_val; }
    T getNewVal() { return this->new_val; }
    T getIncr() { return this->incr; }
    T getMax() { return this->_max; }
    T getMin() { return this->_min; }
    bool upToDate() { return new_val == current_val; }
};

} // namespace setable