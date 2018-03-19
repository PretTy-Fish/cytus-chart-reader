/***********************************************
*
*	File Name: cytus.h
*	Description: Define the essentials for chart info for Cytus II
*	Author: PretTyFish
*	Email: 1250840403@qq.com
*
***********************************************/

#pragma once

#include <string>
#include <list>
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>

#ifndef _AUG_LIST_
#define _AUG_LIST_

template <class T>
class augList : public std::list<T>
{
public:
	using reference = T & ;
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

#endif // !_AUG_LIST_

namespace Cytus2
{
	class cytus2_error : public std::exception
	{
	public:
		cytus2_error(const std::string &error_message = "cytus2 error") noexcept
			: exception(error_message.c_str())
		{
		}
	};

	class chart_error : public cytus2_error
	{
	public:
		chart_error(const std::string &error_message = "chart error") noexcept
			: cytus2_error(error_message)
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

	struct EventBase
	{
		int tick;

		EventBase(int inTime)
			:tick(inTime)
		{
		}

		void operator=(const EventBase &rhs)
		{
			tick = rhs.tick;
			return;
		}

		bool operator<(const EventBase &rhs) const
		{
			return tick < rhs.tick;
		}

		bool operator>(const EventBase &rhs) const
		{
			return tick > rhs.tick;
		}

		bool operator==(const EventBase &rhs) const
		{
			return tick == rhs.tick;
		}

		bool operator<=(const EventBase &rhs) const
		{
			return tick <= rhs.tick;
		}

		bool operator>=(const EventBase &rhs) const
		{
			return tick >= rhs.tick;
		}
	};

	struct Page : EventBase
	{
		int endTick;
		int direction;

		void operator=(const Page &rhs)
		{
			EventBase::operator=(rhs);
			endTick = rhs.endTick;
			direction = rhs.direction;
			return;
		}
	};

	struct Tempo : EventBase
	{
		int value;

		void operator=(const Tempo &rhs)
		{
			EventBase::operator=(rhs);
			value = rhs.value;
			return;
		}

		void operator=(const int &rhs)
		{
			value = rhs;
			return;
		}

		// Note:
		// Comparison between two tempos needs to be done by comparing .value other than directly compare two Tempo structures.
		// Comparison operators are preserved for sorting purpose.
	};

	struct Event : EventBase
	{
		int type;
		std::string args;

		void operator=(const Event &rhs)
		{
			EventBase::operator=(rhs);
			type = rhs.type;
			args = rhs.args;
			return;
		}
	};

	struct Note : EventBase
	{
		int id;
		int type;
		int pageIndex;
		double position;
		double length;
		Note *linkFrom;
		Note *linkTo;

		Note(int inID, int inType, int inPageIndex, int inTime, double inPos, double inLength, Note &inLinkFrom, Note &inLinkTo)
			:id(inID),
			type(inType),
			pageIndex(inPageIndex),
			EventBase(inTime),
			position(inPos),
			length(inLength),
			linkFrom(&inLinkFrom),
			linkTo(&inLinkTo)
		{
		}

		void operator=(const Note &rhs)
		{
			EventBase::operator=(rhs);
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

	class Chart
	{
	protected:
		augList<Page> pageList;
		augList<Tempo> tempoList;
		augList<Event> eventList;
		augList<Note> noteList;
		const Note linkSentinal;

	public:
		Chart();
		Chart(const std::string &input);
		void sortNoteID();
		virtual void sortNote();
		void linkNote(int firstID, int secondID);
		void linkNote(std::list<int> idList);
		void parse(const std::string &input);
		void save(const std::string &output);
	};

	Chart::Chart()
		:linkSentinal(-1,
			-1,
			-1,
			-1,
			0.0,
			-1,
			const_cast<Note &>(linkSentinal),
			const_cast<Note &>(linkSentinal))
	{
	}

	Chart::Chart(const std::string &input)
		:linkSentinal(-1,
			-1,
			-1,
			-1,
			0.0,
			-1,
			const_cast<Note &>(linkSentinal),
			const_cast<Note &>(linkSentinal))
	{
		parse(input);
	}

	void Chart::sortNoteID()
	{
		augList<Note>::iterator it;
		int i;
		for (it = this->noteList.begin(), i = 0; it != this->noteList.end(); ++it, ++i)
		{
			it->id = i;
		}
		return;
	}

	void Chart::sortNote()
	{
		noteList.sort();
		sortNoteID();
		return;
	}

	void Chart::linkNote(int firstID, int secondID)
	{
		Note *first, *second;
		try
		{
			switch (firstID)
			{
			case -1:
				first = &const_cast<Note &>(linkSentinal);
				break;

			default:
				try
				{
					first = &this->noteList.at(firstID);
				}
				catch (std::out_of_range &oor)
				{
					if (oor.what() == "invalid list<T> subscript")
					{
						throw new chart_error("Note (ID: " + std::to_string(firstID) + ") does not exist.");
					}
					throw;
				}
				break;
			}
			switch (secondID)
			{
			case -1:
				second = &const_cast<Note &>(linkSentinal);
				break;

			default:
				try
				{
					second = &this->noteList.at(secondID);
				}
				catch (std::out_of_range &oor)
				{
					if (oor.what() == "invalid list<T> subscript")
					{
						throw new chart_error("Note (ID: " + std::to_string(secondID) + ") does not exist.");
					}
					throw;
				}
				break;
			}
		}
		catch (chart_error)
		{
			throw;
			return;
		}
		if (secondID == -1)
		{
			if (firstID == -1)
			{
				throw new chart_error("Input does not contain any note.");
				return;
			}
		}
		else if (first->tick > second->tick)
		{
			throw new chart_error("Link (from ID: " + std::to_string(first->id) + ", to ID: " + std::to_string(second->id) + ") is backward in time.");
			return;
		}
		else if (firstID == secondID)
		{
			throw new chart_error("Note (ID: " + std::to_string(firstID) + ") cannot be linked to itself.");
			return;
		}
		else
		{
			second->linkFrom = first;
			if (firstID == -1)
			{
				second->type = 3;
			}
			else
			{
				second->type = 4;
				first->linkTo = second;
				if (first->linkFrom == &linkSentinal)
				{
					first->type = 3;
				}
				else
				{
					first->type = 4;
				}
			}
		}
		return;
	}

	void Chart::linkNote(std::list<int> idList)
	{
		idList.sort();
		for (std::list<int>::iterator it = idList.begin(); it != idList.end();)
		{
			linkNote(*it, *++it);
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
}