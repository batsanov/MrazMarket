#include "MrazMarket.h"


void MrazMarket::pushCustomerByArival(Customer cust)
{
	customersByArival.insert(cust, cust.minuteOfArival);
}

void MrazMarket::pushCustomerByDeparture(Customer cust)
{
	customersByDeparture.insert(cust, cust.getLeavingTime());
}

void MrazMarket::pushWorker(Worker worker)
{
	workersStocking.insert(worker, worker.minuteOfLeaving);
}

void MrazMarket::printCustomers()
{
	customersByArival.testPrint();
}

MrazMarket* MrazMarket::createMarket()
{
	
		static MrazMarket market;
		return &market;

}
