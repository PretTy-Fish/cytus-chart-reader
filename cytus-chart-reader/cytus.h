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
using namespace std;

typedef struct SNote
{
	int id;
	int type;
	double time;
	double position;
	double length;
	struct SNote *linkFrom;
	struct SNote *linkTo;
	struct SNote *prev;
	struct SNote *next;
}Note;

class Chart
{
private:
	double bpm = 240;
	double pageSize = 1;
	double pageShift = 0;
	int type = 0;
	int noteCount = 0;
	Note *list;

public:
	Chart();
};

Chart::Chart()
{
	//Still thinking...
}