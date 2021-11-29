#pragma once
#include <string>

struct Worker
{
	size_t minuteOfLeaving;
	ResourceType item;
	Worker(size_t minuteOfLeaving, ResourceType item) :minuteOfLeaving(minuteOfLeaving), item(item) {};

	size_t getReturnTime() { return minuteOfLeaving + 60; };
	const char* getResourceType() const
	{
		switch (item)
		{
		case ResourceType::banana:
			return "banana";
			break;
		case ResourceType::schweppes:
			return "schweppes";
			break;
		default: 
			return "";
			break;
		}
	};
	void announceLeaving() const
	{
		std::cout << "W " << minuteOfLeaving << " " << getResourceType() << std::endl;
	}
	void announceComingBack() const
	{
		std::cout << "D " << minuteOfLeaving+60 << " " << getResourceType() << std::endl;
	}
};