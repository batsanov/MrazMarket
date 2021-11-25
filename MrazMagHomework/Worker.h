#pragma once
#include <string>
struct Worker
{
	size_t minuteOfLeaving;
	std::string item;
	Worker(size_t minuteOfLeaving, std::string item) :minuteOfLeaving(minuteOfLeaving), item(item) {};

	size_t getReturnTime() { return minuteOfLeaving + 60; };
	void announceLeaving() const
	{
		std::cout << "W " << minuteOfLeaving << " " << item << std::endl;
	}
	void announceComingBack() const
	{
		std::cout << "D " << minuteOfLeaving+60 << " " << item << std::endl;
	}
};