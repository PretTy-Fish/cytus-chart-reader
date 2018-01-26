/**********************************************
*
*File Name: cytus.h
*Description: Define structure Note and class Chart
*Author: PretTyFish
*Email: 1250840403@qq.com
*
**********************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <list>
using namespace std;

class cytus_error: public exception
{
private:
	char const* what_arg;

public:
	cytus_error(const char* error_message)
		: what_arg(error_message)
	{}
	const char* what() const { return what_arg; }
};

struct TimeCode
{
	int version;
	double time;
	int tick;

	void operator=(const double &rhs)
	{
		tick = -DBL_MAX;
		if (rhs < 0)
		{
			throw new cytus_error("Input time lower than 0.");
			version = 0;
		}
		else
		{
			version = 1;
		}
		time = rhs;
		return;
	}
	void operator=(const int &rhs)
	{
		time = -DBL_MAX;
		if (rhs < 0)
		{
			throw new cytus_error("Input time lower than 0.");
			version = 0;
		}
		else
		{
			version = 2;
		}
		tick = rhs;
		return;
	}
	bool operator<(const TimeCode &rhs) const
	{
		if(!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
			throw new cytus_error("Invalid time code.");
		else if (version != rhs.version)
			throw new cytus_error("Incomparable time code: Different version of time code.");
		return (time < rhs.time) || (tick < rhs.tick);
	}
};

struct Note
{
	int id;
	int type;
	TimeCode time;
	double position;
	double length;
	Note *linkFrom;
	Note *linkTo;

	bool operator<(const Note &rhs) const
	{
		return (time < rhs.time);
	}
};

class Chart: public list<Note>
{
private:
	int version = 0;
	double bpm = 240;
	double pageSize = 1;
	double pageShift = 0;

public:
	Chart();
	virtual void sort();
};

Chart::Chart()
{
	//Still thinking...
}

void Chart::sort()
{
	list<Note>::sort();
}
