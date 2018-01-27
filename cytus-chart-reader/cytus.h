/**********************************************
*
*File Name: cytus.h
*Description: Define structure Note and class Chart
*Author: PretTyFish
*Email: 1250840403@qq.com
*
**********************************************/

#pragma once

#include <string>
#include <list>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>

class cytus_error: public std::exception
{
public:
	cytus_error(const std::string &error_message = "cytus error") noexcept
		: exception(error_message.c_str())
	{
	}
};

class chart_error : public cytus_error
{
public:
	chart_error(const std::string &error_message = "chart error") noexcept
		: cytus_error(error_message)
	{
	}
};

class note_error : public chart_error
{
public:
	note_error(const std::string &error_message = "ntoe error") noexcept
		: chart_error(error_message)
	{
	}
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
			throw new note_error("Input time lower than 0.");
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
			throw new note_error("Input time lower than 0.");
			version = 0;
		}
		else
		{
			version = 2;
		}
		tick = rhs;
		return;
	}

	void operator=(const TimeCode &rhs)
	{
		switch (rhs.version)
		{
		case 1:
			operator=(rhs.time);
			break;

		case 2:
			operator=(rhs.tick);
			break;

		default:
			throw new note_error("Invalid time code.");
			break;
		}
		return;
	}

	bool operator<(const TimeCode &rhs) const
	{
		if(!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
			throw new note_error("Invalid time code.");
		else if (version != rhs.version)
			throw new note_error("Incomparable time code: Different version of time code.");
		else switch (version)
		{
		case 1:
			return (time < rhs.time);
			break;

		case 2:
			return (tick < rhs.tick);
			break;

		default:
			throw new note_error("Invalid time code.");
			break;
		}
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

class Chart: public std::list<Note>
{
private:
	int version = 0;
	double tempo = 240.0;
	double pageSize = 1.0;
	double pageShift = 0.0;

public:
	Chart(const std::string &input);
	virtual void sort();
	void parse(const std::string &input);
};

Chart::Chart(const std::string &input)
{
	parse(input);
}

void Chart::sort()
{
	std::list<Note>::sort();
}

void Chart::parse(const std::string &input)
{
	std::ifstream chartFile(input, std::ios::in);
	//Still thinking...
}
