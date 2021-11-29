#include "MrazMarket.h"
#include "Customer.h"
#include <iostream>
#include "PQueue.h"
#include "EventHandler.h"

Store* createStore() {
	return new MrazMarket();
}

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
		market->pushCustomerToArive(customer);
		market->pushCustomerByDeparture(customer);
	}
}

void start(MrazMarket* market)
{
	
	while (!market->customersByDeparture.isEmpty())
	{
		EventHandler::handleNextEvent(*market);
	}
};

int main()
{
	MrazMarket* market = new MrazMarket();
	getInput(market);
	std::cout << std::endl;
	start(market);
	
	return 0;
}

// PROBLEMO LINK UP BETWEEN EVENTHANDLER AND MRAZMARKET
// TESTS 
// MANY WAITING CLIENTS LOGIC ERR