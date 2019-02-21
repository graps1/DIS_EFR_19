#pragma once
#include "Arduino.h"

template <typename T>
class Descriptable
{
  private:
    String description;
    T content;
  public:
    Descriptable(String description) { this->description = description; }
    String getDescription() {return this->description;}
    T getContent() {return this->content;}
    void set(T content) {this->content = content;}
    Descriptable<T> &operator=(const T &other) { 
        set(other); 
        return *this;
    };
};