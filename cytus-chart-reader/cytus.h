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

	void operator=(const double &rhs)
	{
		if (rhs < 0)
		{
			throw new note_error("Input time lower than 0.");
			version = 0;
		}
		else
		{
			version = 1;
		}
		tick = -INT_MAX-1;
		time = rhs;
		return;
	}

	void operator=(const int &rhs)
	{
		if (rhs < 0)
		{
			throw new note_error("Input time lower than 0.");
			version = 0;
		}
		else
		{
			version = 2;
		}
		time = -DBL_MAX;
		tick = rhs;
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

	bool operator<(const double &rhs) const
	{
		if (version != 1)
			throw new note_error("Incomparable time code: Different version of time code.");
		return time < rhs;
	}

	bool operator<(const int &rhs) const
	{
		if (version != 2)
			throw new note_error("Incomparable time code: Different version of time code.");
		return tick < rhs;
	}

	bool operator>(const TimeCode &rhs) const
	{
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
			throw new note_error("Invalid time code.");
		else if (version != rhs.version)
			throw new note_error("Incomparable time code: Different version of time code.");
		else switch (version)
		{
		case 1:
			return (time > rhs.time);
			break;

		case 2:
			return (tick > rhs.tick);
			break;

		default:
			throw new note_error("Invalid time code.");
			break;
		}
		return (time > rhs.time) || (tick > rhs.tick);
	}

	bool operator>(const double &rhs) const
	{
		if (version != 1)
			throw new note_error("Incomparable time code: Different version of time code.");
		return time > rhs;
	}

	bool operator>(const int &rhs) const
	{
		if (version != 2)
			throw new note_error("Incomparable time code: Different version of time code.");
		return tick > rhs;
	}

	bool operator==(const TimeCode &rhs) const
	{
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
			throw new note_error("Invalid time code.");
		else if (version != rhs.version)
			throw new note_error("Incomparable time code: Different version of time code.");
		else switch (version)
		{
		case 1:
			return (time == rhs.time);
			break;

		case 2:
			return (tick == rhs.tick);
			break;

		default:
			throw new note_error("Invalid time code.");
			break;
		}
		return (version == rhs.version) && (time == rhs.time) && (tick == rhs.tick);
	}

	bool operator==(const double &rhs) const
	{
		if (version != 1)
			throw new note_error("Incomparable time code: Different version of time code.");
		return time == rhs;
	}

	bool operator==(const int &rhs) const
	{
		if (version != 2)
			throw new note_error("Incomparable time code: Different version of time code.");
		return tick == rhs;
	}
};

struct Note
{
	int id;
	int type;
	int pageIndex;
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
	const Note &operator[](int request) const;
	Note &operator[](int request);
	const Note &at(int request) const;
	Note &at(int request);
	void sortID();
	virtual void sort();
	void link(int firstID, int secondID);
	void parse(const std::string &input);
	void save(const std::string &output);
};

Chart::Chart(const std::string &input)
{
	parse(input);
}

const Note &Chart::operator[](int request) const
{
	const_iterator it;
	for (it = this->begin(); (it->id != request) || (it != this->end()); ++it);
	if (it->id != request)
		throw new chart_error("Note not found.");
	return *it;
}

Note &Chart::operator[](int request)
{
	iterator it;
	for (it = this->begin(); (it->id != request) || (it != this->end()); ++it);
	if (it->id != request)
		throw new chart_error("Note not found.");
	return *it;
}

const Note &Chart::at(int request) const
{
	return Chart::operator[](request);
}

Note &Chart::at(int request)
{
	return Chart::operator[](request);
}

void Chart::sortID()
{
	iterator it;
	int i;
	for (it = this->begin(), i = 0; it != this->end(); ++it, ++i)
		it->id = i;
	return;
}

void Chart::sort()
{
	std::list<Note>::sort();
	Chart::sortID();
	return;
}

void Chart::link(int firstID, int secondID)
{
	//Still thinking...
}

void Chart::parse(const std::string &input)
{
	std::ifstream chartFile(input, std::ios::in);
	//Still thinking...
}

void Chart::save(const std::string &output)
{
	std::ofstream chartFile(output, std::ios::out);
	//Still thinking...
}
