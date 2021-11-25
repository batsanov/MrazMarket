#pragma once
#include "Customer.h"
#include "PQueue.h"
#include "Worker.h"

struct MrazMarket {

	PQueue<Customer> customersByArival;
	PQueue<Worker> workersStocking;
	PQueue<Customer> customersByDeparture;
	int availableWorkers = 0;
	int bananas = 0;
	int schweppes = 0;


	MrazMarket(MrazMarket const&) = delete;
	MrazMarket& operator=(MrazMarket const&) = delete;
	MrazMarket() = default;
	~MrazMarket() = default;

	void pushCustomerByArival(Customer cust);
	void pushCustomerByDeparture(Customer cust);
	void pushWorker(Worker worker);
	void printCustomers();
	
	static MrazMarket* createMarket();
	


	

};