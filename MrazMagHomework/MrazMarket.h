#pragma once
#include "LinkedQueue.h"
#include "interface.h"
#include "Customer.h"
#include "PQueue.h"
#include "Worker.h"
#include <vector>



struct MrazMarket : Store {

	LinkedQueue<Customer> customersToArive;
	std::vector<Customer> waitingForRestocking;
	PQueue<Worker> workersStocking;
	PQueue<Customer> customersByDeparture;

	int availableWorkers = 0;
	int banana = 0;
	int schweppes = 0;


	MrazMarket(MrazMarket const&) = delete;
	MrazMarket& operator=(MrazMarket const&) = delete;
	MrazMarket() = default;
	~MrazMarket() = default;

	int getSchweppes() const override;
	int getBanana() const override;
	void advanceTo(int minute) override; 
	void addClients(const Client* clients, int count) override;
	void init(int workerCount, int startBanana, int startSchweppes) override;

	void pushCustomerToArive(const Customer cust) ;
	void pushCustomerByDeparture(const Customer cust);
	void pushWorker(Worker worker);

};
