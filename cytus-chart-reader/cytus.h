/**********************************************
*
*Define classes CNote and CChart
*Author: PretTyFish
*Email: 1250840403@qq.com
*
**********************************************/


#pragma once

#include <iostream>
#include <fstream>
using namespace std;

class Note
{
private:
	int id = -1;
	int type = -1;
	double time = 0;
	double position = 0;
	double length = 0;
	int linkFrom = -1;
	int linkTo = -1;

public:
	Note(int rdID, int rdType, double rdTime, double rdPos, double rdLen, double rdLinkFrom, double rdLinkTo);
	int getID();
	int getType();
	double getTime();
	double getPos();
	double getLen();
	int getLinkFrom();
	int getLinkTo();
	int writeID(int rdID);
	int writeType(int rdType);
	int writeTime(double rdTime);
	int writePos(double rdPos);
	int writeLen(double rdLen);
	int writeLinkFrom(int rdLinkFrom);
	int writeLinkTo(int rdLinkTo);

};

Note::Note(int rdID, int rdType, double rdTime, double rdPos, double rdLen, double rdLinkFrom, double rdLinkTo)
{
	writeID(rdID);
	writeType(rdType);
	writeTime(rdTime);
	writePos(rdPos);
	writeLen(rdLen);
	writeLinkFrom(rdLinkFrom);
	writeLinkTo(rdLinkTo);
}

int Note::getID()
{
	return id;
}

int Note::getType()
{
	return type;
}

double Note::getTime()
{
	return time;
}

double Note::getPos()
{
	return position;
}

double Note::getLen()
{
	return length;
}

int Note::getLinkFrom()
{
	return linkFrom;
}

int Note::getLinkTo()
{
	return linkTo;
}

int Note::writeID(int rdID)
{
	id = rdID;
	return 0;
}

int Note::writeType(int rdType)
{
	type = rdType;
	return 0;
}

int Note::writeTime(double rdTime)
{
	time = rdTime;
	return 0;
}

int Note::writePos(double rdPos)
{
	position = rdPos;
	return 0;
}

int Note::writeLen(double rdLen)
{
	length = rdLen;
	return 0;
}

int Note::writeLinkFrom(int rdLinkFrom)
{
	linkFrom = rdLinkFrom;
	return 0;
}

int Note::writeLinkTo(int rdLinkTo)
{
	linkTo = rdLinkTo;
	return 0;
}

class Chart
{
private:
	double bpm = 240;
	double pageSize = 1;
	double pageShift = 0;
	int type = 0;
	int noteCount = 0;

public:
	Chart();
};

Chart::Chart()
{
	//Still thinking...
}