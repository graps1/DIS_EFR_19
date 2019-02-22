#pragma once
#include "Arduino.h"

template <typename T>
class Descriptable
{
  private:
	String description, unit;
	T content;

  public:	
	Descriptable(String description, String unit)
	{
		this->description = description;
		this->unit = unit;
	}

	Descriptable(String description) : Descriptable(description, "") {}

	T getContent()
	{
		return this->content;
	}

	void set(T content)
	{
		this->content = content;
	}

	Descriptable<T> &operator=(const T &other)
	{
		set(other);
		return *this;
	}

	String str()
	{
		return description + " " + String(content) + unit;
	}
};