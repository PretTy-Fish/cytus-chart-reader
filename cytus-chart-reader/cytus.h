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
#include <algorithm>
#include <vector>
using namespace std;

template <typename T> struct Note
{
	int id;
	int type;
	T time;
	double position;
	double length;
	Note *linkFrom;
	Note *linkTo;

	bool operator<(const Note &rhs) const
	{
		return (time < rhs.time);
	}
};

class Chart
{
private:
	double bpm = 240;
	double pageSize = 1;
	double pageShift = 0;
	int version = 1;
	int noteCount = 0;
	template <typename T> vector<Note<T>> noteList;

public:
	Chart();
};

Chart::Chart()
{
	//Still thinking...
}