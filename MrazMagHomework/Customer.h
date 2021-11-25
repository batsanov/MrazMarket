#pragma once
#include <iostream>
#include<string>

struct Customer {
	static size_t counter;
	
	size_t minuteOfArival;
	size_t numberOfBananas;
	size_t numberOfSchweppes;
	size_t maxWaitingMinutes;
	size_t id;


	Customer() { id = counter++; minuteOfArival = numberOfBananas = numberOfSchweppes = maxWaitingMinutes = 0; };
	Customer(size_t arivalTime, size_t bananas, size_t schweppes, size_t waitingMins) :
		minuteOfArival(arivalTime), numberOfBananas(bananas), numberOfSchweppes(schweppes), maxWaitingMinutes(waitingMins),id(counter++) {};
	~Customer() = default;
	friend std::ostream& operator<<(std::ostream& out, const Customer& cust)
	{
		return out << cust.id << ". " << cust.minuteOfArival << " "  << cust.numberOfBananas << " " << cust.numberOfSchweppes << " " << cust.maxWaitingMinutes << std::endl;
	};
	friend std::istream& operator>>(std::istream& is, Customer& obj)
	{
		char buffer[128];

		is.getline(buffer, 128);
		int customerInformation[4];
		int i = 0, j = 0, currentParam = 0;

		while (buffer[i] != '\0')
		{

			if (buffer[i] == ' ')
			{
				customerInformation[j++] = currentParam;
				currentParam = 0;
			}
			else
			{
				currentParam = currentParam * 10 + buffer[i] - '0';
			}
			i++;
		}
		customerInformation[j] = currentParam;


		obj.minuteOfArival = customerInformation[0];
		obj.numberOfBananas = customerInformation[1];
		obj.numberOfSchweppes = customerInformation[2];
		obj.maxWaitingMinutes = customerInformation[3];

		return is;
	};

	int getLeavingTime() const
	{
		return (minuteOfArival + maxWaitingMinutes);
	};
	void announceLeaving(int minute, int bananas, int schweppes) const
	{
		std::cout << id << ". " << minute << " " << bananas << " " << schweppes << std::endl;
	};
};
