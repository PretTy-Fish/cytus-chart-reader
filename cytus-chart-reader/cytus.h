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

class CNote
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
	CNote(int rdID, int rdType, double rdTime, double rdPos, double rdLen, double rdLinkFrom, double rdLinkTo)
	{
		writeID(rdID);
		writeType(rdType);
		writeTime(rdTime);
		writePos(rdPos);
		writeLen(rdLen);
		writeLinkFrom(rdLinkFrom);
		writeLinkTo(rdLinkTo);
	}

	int getID()
	{
		return id;
	}

	int getType()
	{
		return type;
	}

	double getTime()
	{
		return time;
	}

	double getPos()
	{
		return position;
	}

	double getLen()
	{
		return length;
	}

	int getLinkFrom()
	{
		return linkFrom;
	}

	int getLinkTo()
	{
		return linkTo;
	}

	int writeID(int rdID)
	{
		id = rdID;
		return 0;
	}

	int writeType(int rdType)
	{
		type = rdType;
		return 0;
	}

	int writeTime(double rdTime)
	{
		time = rdTime;
		return 0;
	}

	int writePos(double rdPos)
	{
		position = rdPos;
		return 0;
	}

	int writeLen(double rdLen)
	{
		length = rdLen;
		return 0;
	}

	int writeLinkFrom(int rdLinkFrom)
	{
		linkFrom = rdLinkFrom;
		return 0;
	}

	int writeLinkTo(int rdLinkTo)
	{
		linkTo = rdLinkTo;
		return 0;
	}
};