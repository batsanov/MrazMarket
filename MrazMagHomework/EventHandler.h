#include "MrazMarket.h"

struct EventHandler {
	int getNextEvent(MrazMarket& market)  // return a -> 1 , b -> 2, c -> 3
	{
		int a = market.customersByArival.isEmpty() ? 1500 : market.customersByArival.first().minuteOfArival;
		int b = market.customersByDeparture.isEmpty() ? 1500 : market.customersByDeparture.first().getLeavingTime();
		int c = market.workersStocking.isEmpty() ? 1500 : market.workersStocking.first().minuteOfLeaving + 60;
		if (a < b && a < c)
			return 1;
		else if (b < a && b < c)
			return 2;
		else if (c < a && c < b)
			return 3;

	};

	void handleNextEvent(MrazMarket& market)
	{
		switch (getNextEvent(market))
		{
		case 1:
			serveCustomer(market);
			break;
		case 2:
			leaveCustomer(market);
			break;
		case 3:
			returnWorker(market);
			break;
		default:
			break;
		}
	}


	void firstCustomerBuy(MrazMarket& market, int time) {
		const Customer* customer = &(market.customersByDeparture.first());
		int wantedBananas = market.customersByDeparture.first().numberOfBananas;
		int wantedSchweppes = market.customersByDeparture.first().numberOfSchweppes;

		if (wantedBananas <= market.bananas && wantedSchweppes <= market.schweppes)
		{
			market.bananas -= wantedBananas;
			market.schweppes -= wantedSchweppes;
			customer->announceLeaving(time, wantedBananas, wantedSchweppes);
			market.customersByDeparture.dequeue();
		}
		
	};


	void returnWorker(MrazMarket& market) {                        
		const Worker* worker = &(market.workersStocking.first());
		worker->announceComingBack();
		if (worker->item == "banana") market.bananas += 100;
		else market.schweppes += 100;
		
		firstCustomerBuy(market, worker->minuteOfLeaving + 60);       // искам да обслужа този с най-малко id
		market.workersStocking.dequeue();
		market.availableWorkers++;

	}

	void leaveCustomer(MrazMarket& market) {

		const Customer* customer = &(market.customersByDeparture.first());
		int wantedBananas = market.customersByDeparture.first().numberOfBananas;
		int wantedSchweppes = market.customersByDeparture.first().numberOfSchweppes;
		
		if (wantedBananas <= market.bananas && wantedSchweppes > market.schweppes)
		{
			customer->announceLeaving(customer->getLeavingTime(), wantedBananas ,market.schweppes);
			market.bananas -= wantedBananas;
			market.schweppes = 0;
			
		}
		else if (wantedBananas > market.bananas && wantedSchweppes <= market.schweppes)
		{
			customer->announceLeaving(customer->getLeavingTime(), market.bananas, wantedSchweppes);
			market.schweppes -= wantedSchweppes;
			market.bananas = 0;
		}
		else
		{
			customer->announceLeaving(customer->getLeavingTime(), market.bananas, market.schweppes);
			market.schweppes = 0;
			market.bananas = 0;
		}
		market.customersByDeparture.dequeue();
	}

	void serveCustomer(MrazMarket& market)
	{
		const Customer* customer = &(market.customersByArival.first());
		int wantedBananas = market.customersByArival.first().numberOfBananas;
		int wantedSchweppes = market.customersByArival.first().numberOfSchweppes;
		if (wantedBananas <= market.bananas && wantedSchweppes <= market.schweppes)
		{
		   
			market.bananas -= wantedBananas;
			market.schweppes -= wantedSchweppes;
			customer->announceLeaving(customer->minuteOfArival, wantedBananas, wantedSchweppes);
			market.customersByDeparture.dequeue();
		}
		else
		{
			if (market.availableWorkers > 0)
			{
				if (wantedBananas <= market.bananas && wantedSchweppes > market.schweppes)
				{
					if (!isBeingShippedBefore(market, "schweppes", customer->getLeavingTime()))
					{
						sendWorker(market, customer->minuteOfArival, "schweppes");
						market.availableWorkers--;
					}
					
				}
				else if (wantedBananas > market.bananas && wantedSchweppes <= market.schweppes)
				{
					if (!isBeingShippedBefore(market,"banana",customer->getLeavingTime()))
					{
						sendWorker(market, customer->minuteOfArival, "banana");
						market.availableWorkers--;
					}
				}
				else
				{
					if (market.availableWorkers > 1)
					{
						if (!isBeingShippedBefore(market, "banana", customer->getLeavingTime()))
						{
							sendWorker(market, customer->minuteOfArival, "banana");
							market.availableWorkers--;
						}
						if (!isBeingShippedBefore(market, "schweppes", customer->getLeavingTime()))
						{
							sendWorker(market, customer->minuteOfArival, "schweppes");
							market.availableWorkers--;
						}
					}
					else
					{
						if (wantedBananas == wantedSchweppes)
						{

							if (!isBeingShippedBefore(market, "banana", customer->getLeavingTime()))
							{
								sendWorker(market, customer->minuteOfArival, "banana");
								market.availableWorkers--;
							}
						}
						else
						{
							if (wantedBananas > wantedSchweppes)
							{
								if (!isBeingShippedBefore(market, "banana", customer->getLeavingTime()))
								{
									sendWorker(market, customer->minuteOfArival, "banana");
									market.availableWorkers--;
								}
							}
							else
							{
								if (!isBeingShippedBefore(market, "schweppes", customer->getLeavingTime()))
								{
									sendWorker(market, customer->minuteOfArival, "schweppes");
									market.availableWorkers--;
								}
							}
						}
					}
				}

			}
		}
		market.customersByArival.dequeue();
	}

	bool isBeingShippedBefore(MrazMarket& market, std::string item, int time) {  // тромаво
		PQueue<Worker> temp;
		bool flag = false;
		while (!market.workersStocking.isEmpty())
		{
			if (market.workersStocking.first().minuteOfLeaving + 60 > time)
			{
				break;
			}
			else if (item == market.workersStocking.first().item)
			{
				flag = true;
				break;
			}
			else
			{
				temp.insert(market.workersStocking.first(), market.workersStocking.first().minuteOfLeaving);
				market.workersStocking.dequeue();                                                                 
			}
		}
		
		while (!temp.isEmpty())
		{
			market.workersStocking.insert(temp.first(), temp.first().minuteOfLeaving);
			temp.dequeue();
		}
	     
		return flag;
	};

	void sendWorker(MrazMarket& market, int time,std::string item)
	{
		Worker worker(time, item);
		worker.announceLeaving();
		market.pushWorker(worker);
	};

};