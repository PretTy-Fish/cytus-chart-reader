#pragma once

#include <string>
#include <list>
#include <vector>
#include <climits>
#include <cfloat>
#include <stdexcept>
#include <iostream>
#include <fstream>

#ifndef _AUG_LIST_
#define _AUG_LIST_

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

#endif // !_AUG_LIST_

namespace Cytus
{
	class cytus_error : public std::runtime_error
	{
	public:
		cytus_error(const std::string &error_message = "cytus error") noexcept
			: runtime_error(error_message.c_str())
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

	struct EventBase
	{
		double time;

		EventBase(double inTime)
			:time(inTime)
		{
		}

		void operator=(const EventBase &rhs)
		{
			time = rhs.time;
			return;
		}

		bool operator<(const EventBase &rhs) const
		{
			return time < rhs.time;
		}

		bool operator>(const EventBase &rhs) const
		{
			return time > rhs.time;
		}

		bool operator==(const EventBase &rhs) const
		{
			return time == rhs.time;
		}

		bool operator<=(const EventBase &rhs) const
		{
			return time <= rhs.time;
		}

		bool operator>=(const EventBase &rhs) const
		{
			return time >= rhs.time;
		}
	};

	struct Note : EventBase
	{
		int id;
		double position;
		double length;
		Note *linkFrom;
		Note *linkTo;

		Note(int inID, double inTime, double inPos, double inLength, Note &inLinkFrom, Note &inLinkTo)
			:id(inID),
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
			position = rhs.position;
			length = rhs.length;
			linkFrom = rhs.linkFrom;
			linkTo = rhs.linkTo;
			return;
		}
	};

	class Chart
	{
	private:
		augList<Note> noteList;
		const Note linkSentinal;

	protected:
		double tempo = 240.0;
		double pageSize = 1.0;
		double pageShift = 0.0;

	public:
		Chart();
		Chart(const std::string &input);
		virtual void sortNoteID();
		virtual void sortNote();
		virtual void linkNote(int firstID, int secondID);
		virtual void linkNote(std::list<int> idList);
		void parse(const std::string &input);
		void save(const std::string &output);
	};

	Chart::Chart()
		:linkSentinal(-1,
			-DBL_MAX,
			0.0,
			-DBL_MAX,
			const_cast<Note &>(linkSentinal),
			const_cast<Note &>(linkSentinal))
	{
	}

	Chart::Chart(const std::string &input)
		:linkSentinal(-1,
			-DBL_MAX,
			0.0,
			-DBL_MAX,
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
		else if (first->time > second->time)
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
			if (firstID != -1)
			{
				first->linkTo = second;
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

	namespace aug
	{
		struct Page : EventBase
		{
			double endTime;
			int direction;

			void operator=(const Page &rhs)
			{
				EventBase::operator=(rhs);
				endTime = rhs.endTime;
				direction = rhs.direction;
				return;
			}
		};

		struct Tempo : EventBase
		{
			double bpm;

			void operator=(const Tempo &rhs)
			{
				EventBase::operator=(rhs);
				bpm = rhs.bpm;
				return;
			}

			void operator=(const double &rhs)
			{
				bpm = rhs;
				return;
			}

			// Note:
			// Comparison between two tempos needs to be done by comparing .bpm other than directly compare two Tempo structures.
			// Comparison operators are preserved for sorting purpose.
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

			Note(int inID, int inType, int inPageIndex, double inTime, double inPos, double inLength, Note &inLinkFrom, Note &inLinkTo)
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

		class Chart : public Cytus::Chart
		{
		private:
			augList<Note> noteList;
			const Note linkSentinal;

		protected:
			augList<Page> pageList;
			augList<Tempo> tempoList;

		public:
			Chart();
			Chart(const std::string &input);
			virtual void sortNoteID();
			virtual void sortNote();
			virtual void linkNote(int firstID, int secondID);
			virtual void linkNote(std::list<int> idList);
		};

		Chart::Chart()
			:linkSentinal(-1,
				-1,
				-1,
				-DBL_MAX,
				0.0,
				-DBL_MAX,
				const_cast<Note &>(linkSentinal),
				const_cast<Note &>(linkSentinal))
		{
		}

		Chart::Chart(const std::string &input)
			:linkSentinal(-1,
				-1,
				-1,
				-DBL_MAX,
				0.0,
				-DBL_MAX,
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
					throw new chart_error("Input doesn't contain any note.");
					return;
				}
			}
			else if (first->time > second->time)
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
	}
}