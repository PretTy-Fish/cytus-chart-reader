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
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>

template <class T>
class augList : public std::list<T>
{
public:
	using reference = T&;
	using const_reference = const T&;

	const_reference operator[](const unsigned int _Pos) const
	{
		typename std::list<T>::const_iterator it;
		for (it = this->begin(); it->id != _Pos; ++it);
		return *it;
	}

	reference operator[](const unsigned int _Pos)
	{
		typename std::list<T>::iterator it;
		for (it = this->begin(); it->id != _Pos; ++it);
		return *it;
	}

	const_reference at(const unsigned int _Pos) const
	{
		if (this->size() <= _Pos)
		{
			throw new std::out_of_range("invalid list<T> subscript");
		}
		return operator[](_Pos);
	}

	reference at(const unsigned int _Pos)
	{
		if (this->size() <= _Pos)
		{
			throw new std::out_of_range("invalid list<T> subscript");
		}
		return operator[](_Pos);
	}
};

class cytus_error : public std::exception
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
	note_error(const std::string &error_message = "note error") noexcept
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
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
		{
			throw new note_error("Invalid time code.");
		}
		else if (version != rhs.version)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
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
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return time < rhs;
	}

	bool operator<(const int &rhs) const
	{
		if (version != 2)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return tick < rhs;
	}

	bool operator>(const TimeCode &rhs) const
	{
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
		{
			throw new note_error("Invalid time code.");
		}
		else if (version != rhs.version)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
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
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return time > rhs;
	}

	bool operator>(const int &rhs) const
	{
		if (version != 2)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return tick > rhs;
	}

	bool operator==(const TimeCode &rhs) const
	{
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
		{
			throw new note_error("Invalid time code.");
		}
		else if (version != rhs.version)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
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
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return time == rhs;
	}

	bool operator==(const int &rhs) const
	{
		if (version != 2)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return tick == rhs;
	}

	bool operator<=(const TimeCode &rhs) const
	{
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
		{
			throw new note_error("Invalid time code.");
		}
		else if (version != rhs.version)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		else switch (version)
		{
		case 1:
			return (time <= rhs.time);
			break;

		case 2:
			return (tick <= rhs.tick);
			break;

		default:
			throw new note_error("Invalid time code.");
			break;
		}
		return (time <= rhs.time) || (tick <= rhs.tick);
	}

	bool operator<=(const double &rhs) const
	{
		if (version != 1)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return time <= rhs;
	}

	bool operator<=(const int &rhs) const
	{
		if (version != 2)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return tick <= rhs;
	}

	bool operator>=(const TimeCode &rhs) const
	{
		if (!(((version == 1) || (version == 2)) && ((rhs.version == 1) || (rhs.version == 2))))
		{
			throw new note_error("Invalid time code.");
		}
		else if (version != rhs.version)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		else switch (version)
		{
		case 1:
			return (time >= rhs.time);
			break;

		case 2:
			return (tick >= rhs.tick);
			break;

		default:
			throw new note_error("Invalid time code.");
			break;
		}
		return (time >= rhs.time) || (tick >= rhs.tick);
	}

	bool operator>=(const double &rhs) const
	{
		if (version != 1)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return time >= rhs;
	}

	bool operator>=(const int &rhs) const
	{
		if (version != 2)
		{
			throw new note_error("Incomparable time code: Different version of time code.");
		}
		return tick >= rhs;
	}
};

struct Event
{
	TimeCode time;

	Event(TimeCode inTime)
		:time(inTime)
	{
	}

	void operator=(const Event &rhs)
	{
		time = rhs.time;
		return;
	}

	bool operator<(const Event &rhs) const
	{
		return time < rhs.time;
	}

	bool operator>(const Event &rhs) const
	{
		return time > rhs.time;
	}

	bool operator==(const Event &rhs) const
	{
		return time == rhs.time;
	}

	bool operator<=(const Event &rhs) const
	{
		return time <= rhs.time;
	}

	bool operator>=(const Event &rhs) const
	{
		return time >= rhs.time;
	}
};

struct Page : Event
{
	TimeCode endTime;
	int direction;

	void operator=(const Page &rhs)
	{
		Event::operator=(rhs);
		endTime = rhs.endTime;
		direction = rhs.direction;
		return;
	}
};

struct Tempo : Event
{
	int value;
	double bpm;

	void operator=(const Tempo &rhs)
	{
		Event::operator=(rhs);
		value = rhs.value;
		bpm = rhs.bpm;
		return;
	}

	void operator=(const int &rhs)
	{
		value = rhs;
		bpm = (double)(6E7 / rhs);
		return;
	}

	void operator=(const double &rhs)
	{
		bpm = rhs;
		value = (int)(6E7 / rhs);
		return;
	}

	bool operator<(const Tempo &rhs) const
	{
		if ((value > rhs.value) != (bpm < rhs.bpm))
		{
			throw new chart_error("Failed to compare two tempo.");
		}
		return (value > rhs.value) && (bpm < rhs.bpm);
	}

	bool operator<(const int &rhs) const
	{
		return value > rhs;
	}

	bool operator<(const double &rhs) const
	{
		return bpm < rhs;
	}

	bool operator>(const Tempo &rhs) const
	{
		if ((value < rhs.value) != (bpm > rhs.bpm))
		{
			throw new chart_error("Failed to compare two tempo.");
		}
		return (value < rhs.value) && (bpm > rhs.bpm);
	}

	bool operator>(const int &rhs) const
	{
		return value < rhs;
	}

	bool operator>(const double &rhs) const
	{
		return bpm > rhs;
	}

	bool operator==(const Tempo &rhs) const
	{
		if ((value == rhs.value) != (bpm == rhs.bpm))
		{
			throw new chart_error("Failed to compare two tempo.");
		}
		return (value == rhs.value) && (bpm == rhs.bpm);
	}

	bool operator==(const int &rhs) const
	{
		return value == rhs;
	}

	bool operator==(const double &rhs) const
	{
		return bpm == rhs;
	}

	bool operator<=(const Tempo &rhs) const
	{
		if ((value >= rhs.value) != (bpm <= rhs.bpm))
		{
			throw new chart_error("Failed to compare two tempo.");
		}
		return (value >= rhs.value) && (bpm <= rhs.bpm);
	}

	bool operator<=(const int &rhs) const
	{
		return value >= rhs;
	}

	bool operator<=(const double &rhs) const
	{
		return bpm <= rhs;
	}

	bool operator>=(const Tempo &rhs) const
	{
		if ((value <= rhs.value) != (bpm >= rhs.bpm))
		{
			throw new chart_error("Failed to compare two tempo.");
		}
		return (value <= rhs.value) && (bpm >= rhs.bpm);
	}

	bool operator>=(const int &rhs) const
	{
		return value <= rhs;
	}

	bool operator>=(const double &rhs) const
	{
		return bpm >= rhs;
	}
};

struct SpeedChange : Event
{
	int type;
	std::string args;

	void operator=(const SpeedChange &rhs)
	{
		Event::operator=(rhs);
		type = rhs.type;
		args = rhs.args;
		return;
	}
};

struct Note : Event
{
	int id;
	int type;
	int pageIndex;
	double position;
	TimeCode length;
	Note *linkFrom;
	Note *linkTo;

	Note(int inID, int inType, int inPageIndex, TimeCode inTime, double inPos, TimeCode inLength, Note &inLinkFrom, Note &inLinkTo)
		:id(inID),
		type(inType),
		pageIndex(inPageIndex),
		Event(inTime),
		position(inPos),
		length(inLength),
		linkFrom(&inLinkFrom),
		linkTo(&inLinkTo)
	{
	}

	void operator=(const Note &rhs)
	{
		Event::operator=(rhs);
		id = rhs.id;
		type = rhs.type;
		pageIndex = rhs.pageIndex;
		position = rhs.position;
		length = rhs.length;
		linkFrom = rhs.linkFrom;
		linkTo = rhs.linkTo;
		return;
	}
};

class Chart : public augList<Note>
{
private:
	int version = 0;
	double tempo = 240.0;
	double pageSize = 1.0;
	double pageShift = 0.0;
	augList<Page> pageList;
	augList<Tempo> tempoList;
	augList<SpeedChange> eventList;
	const Note linkSentinal;

public:
	Chart();
	Chart(const std::string &input);
	void sortID();
	virtual void sort();
	void link(int firstID, int secondID);
	void link(std::vector<int> idList);
	void parse(const std::string &input);
	void save(const std::string &output);
};

Chart::Chart()
	:linkSentinal(-1,
		-1,
		-1,
		{ 0, -DBL_MAX, -INT_MAX - 1 },
		0.0,
		{ 0, -DBL_MAX, -INT_MAX - 1 },
		const_cast<Note &>(linkSentinal),
		const_cast<Note &>(linkSentinal))
{
}

Chart::Chart(const std::string &input)
	:linkSentinal(-1,
		-1,
		-1,
		{ 0, -DBL_MAX, -INT_MAX - 1 },
		0.0,
		{ 0, -DBL_MAX, -INT_MAX - 1 },
		const_cast<Note &>(linkSentinal),
		const_cast<Note &>(linkSentinal))
{
	parse(input);
}

void Chart::sortID()
{
	iterator it;
	int i;
	for (it = this->begin(), i = 0; it != this->end(); ++it, ++i)
	{
		it->id = i;
	}
	return;
}

void Chart::sort()
{
	std::list<Note>::sort();
	sortID();
	return;
}

void Chart::link(int firstID, int secondID)
{
	Note *first, *second;
	if (firstID == -1)
	{
		if (secondID == -1)
		{
			throw new chart_error("Input doesn't contain any note.");
			return;
		}
		else
		{
			try
			{
				second = &this->at(secondID);
			}
			catch (std::out_of_range &oor)
			{
				if (oor.what() == "invalid list<T> subscript")
				{
					throw new chart_error("Note (ID: " + std::to_string(secondID) + "does not exist.");
					return;
				}
				throw;
			}
			second->linkFrom = const_cast<Note *>(&linkSentinal);
			return;
		}
	}
	else if (secondID == -1)
	{
		if (firstID == -1)
		{
			throw new chart_error("Input doesn't contain any note.");
			return;
		}
		else
		{
			try
			{
				first = &this->at(firstID);
			}
			catch (std::out_of_range &oor)
			{
				if (oor.what() == "invalid list<T> subscript")
				{
					throw new chart_error("Note (ID: " + std::to_string(firstID) + "does not exist.");
					return;
				}
				throw;
			}
			first->linkFrom = const_cast<Note *>(&linkSentinal);
			return;
		}
	}
	else
	{
		bool chart_error_flag = false;
		try
		{
			first = &this->at(firstID);
		}
		catch (std::out_of_range &oor)
		{
			if (oor.what() == "invalid list<T> subscript")
			{
				throw new chart_error("Note (ID: " + std::to_string(firstID) + "does not exist.");
				chart_error_flag = true;
			}
			throw;
		}
		try
		{
			second = &this->at(secondID);
		}
		catch (std::out_of_range &oor)
		{
			if (oor.what() == "invalid list<T> subscript")
			{
				throw new chart_error("Note (ID: " + std::to_string(secondID) + "does not exist.");
				chart_error_flag = true;
			}
			throw;
		}
		if (chart_error_flag == true)
		{
			return;
		}
		first->linkTo = second;
		second->linkFrom = first;
		if (first->linkFrom == &linkSentinal)
		{
			first->type = 3;
		}
		else
		{
			first->type = 4;
		}
		second->type = 4;
		if (first->time > second->time)
		{
			throw new chart_error("Link (from ID: " + std::to_string(first->id) + ", to ID: " + std::to_string(second->id) + ") is backward in time.");
		}
		return;
	}
	return;
}

void Chart::link(std::vector<int> idList)
{
	for (std::vector<int>::iterator it = idList.begin(); it != idList.end();)
	{
		link(*it, *++it);
	}
	return;
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
