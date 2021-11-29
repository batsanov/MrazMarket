#include "MrazMarket.h"
#include "EventHandler.h"

int MrazMarket::getSchweppes() const
{
	return schweppes;
}

int MrazMarket::getBanana() const
{
	return banana;
}

void MrazMarket::advanceTo(int minute)
{
	
	while (customersToArive.first().arriveMinute <= minute || customersByDeparture.first().getLeavingTime() <= minute || workersStocking.first().minuteOfLeaving + 60 <= minute)
	{
		EventHandler::handleNextEvent(*this);
	}
}

void MrazMarket::addClients(const Client* clients, int count)
{
	for (size_t i = 0; i < count; i++)
	{
		Customer temp(clients[i].arriveMinute, clients[i].banana, clients[i].schweppes, clients[i].maxWaitTime);
		pushCustomerToArive(temp);
		pushCustomerByDeparture(temp);
	}
}

void MrazMarket::init(int workerCount, int startBanana, int startSchweppes)
{
	availableWorkers = workerCount;
	banana = startBanana;
	schweppes = startSchweppes;
}



void MrazMarket::pushCustomerToArive(const Customer cust)
{
	customersToArive.enqueue(cust);
}

void MrazMarket::pushCustomerByDeparture(const Customer cust)
{
	customersByDeparture.insert(cust, cust.getLeavingTime());
}

void MrazMarket::pushWorker(Worker worker)
{
	workersStocking.insert(worker, worker.minuteOfLeaving);
}




