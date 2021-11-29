#pragma once
#include <iostream>
#include "interface.h"


struct Customer : Client {
	static size_t counter;
	size_t id;


	Customer() { id = counter++; arriveMinute = banana = schweppes = maxWaitTime = 0; };
	Customer(int arivalTime, int  banana, int schweppes, int waitingMins) {
		arriveMinute = arivalTime;
		banana = banana;
		schweppes = schweppes;
		maxWaitTime = waitingMins;
		id = counter++;
	};
	~Customer() = default;
	
	friend std::ostream& operator<<(std::ostream& out, const Customer& cust)
	{
		return out << cust.id << ". " << cust.arriveMinute << " "  << cust.banana << " " << cust.schweppes << " " << cust.maxWaitTime << std::endl;
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


		obj.arriveMinute = customerInformation[0];
		obj.banana = customerInformation[1];
		obj.schweppes = customerInformation[2];
		obj.maxWaitTime = customerInformation[3];

		return is;
	};

	int getLeavingTime() const
	{
		return (arriveMinute + maxWaitTime);
	};
	void announceLeaving(int minute, int banana, int schweppes) const
	{
		std::cout << id << ". " << minute << " " << banana << " " << schweppes << std::endl;
	};
};
