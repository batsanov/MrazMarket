#include "MrazMarket.h"
#include "Customer.h"
#include <iostream>
#include "PQueue.h"
#include "EventHandler.h"

size_t Customer::counter = 0;
void getInput(MrazMarket* market) {
	int numberOfWorkers, numberOfCustomers;
	std::cin >> numberOfWorkers >> numberOfCustomers;
	market->availableWorkers = numberOfWorkers;
	std::cin.ignore();
	for (size_t i = 0; i < numberOfCustomers; i++)
	{
		Customer customer;
		std::cin >> customer;
		market->pushCustomerByArival(customer);
		market->pushCustomerByDeparture(customer);
	}
}

void start(MrazMarket* market)
{
	EventHandler handler;
	while (!market->customersByDeparture.isEmpty())
	{
		handler.handleNextEvent(*market);
	}
};

int main()
{
	MrazMarket* market = market->createMarket();
	getInput(market);
	std::cout << std::endl;
	start(market);
	
	
	return 0;
}

