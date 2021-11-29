#pragma once
#include "MrazMarket.h"

struct EventHandler {
	static int getNextEvent(MrazMarket& market)  // return a -> 1 , b -> 2, c -> 3
	{
		int a = market.customersToArive.isEmpty() ? 1500 : market.customersToArive.first().arriveMinute;
		int b = market.customersByDeparture.isEmpty() ? 1500 : market.customersByDeparture.first().getLeavingTime();
		int c = market.workersStocking.isEmpty() ? 1500 : market.workersStocking.first().minuteOfLeaving + 60;
		if (a <= b && a <= c)
			return 1;
		else if (b <= a && b <= c)
			return 2;
		else if (c <= a && c <= b)
			return 3;
		return 0;
	};

	static void handleNextEvent(MrazMarket& market)
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
		}
	}


	static void firstCustomerBuy(MrazMarket& market, int time) {
		Customer* customer = &market.waitingForRestocking.back();  
		int wantedbanana = customer->banana;
		int wantedSchweppes = customer->schweppes;

		if (wantedbanana <= market.banana && wantedSchweppes <= market.schweppes)
		{
			market.banana -= wantedbanana;
			market.schweppes -= wantedSchweppes;
			customer->announceLeaving(time, wantedbanana, wantedSchweppes);
			market.waitingForRestocking.pop_back();
			
		}
		
	};


	static void returnWorker(MrazMarket& market) {
		const Worker* worker = &(market.workersStocking.first());
		worker->announceComingBack();
		if (worker->item == ResourceType::banana) market.banana += 100;
		else market.schweppes += 100;

		firstCustomerBuy(market, worker->minuteOfLeaving + 60);
		

		market.workersStocking.dequeue();
		market.availableWorkers++;

	}

	static bool removeIfWaiting(MrazMarket& market, Customer cust) {
		for (size_t i = 0; i < market.waitingForRestocking.size(); i++)
		{
			if (market.waitingForRestocking[i].id == cust.id)
			{
				market.waitingForRestocking.erase(market.waitingForRestocking.begin() + i);
				return true;
			}
		}
		return false;
	}

	static void leaveCustomer(MrazMarket& market) {

		const Customer* customer = &(market.customersByDeparture.first());
		int wantedbanana = customer->banana;
		int wantedSchweppes = customer->schweppes;
		if (removeIfWaiting(market, *customer))
		{
			if (wantedbanana <= market.banana && wantedSchweppes <= market.schweppes)
			{
				customer->announceLeaving(customer->getLeavingTime(), wantedbanana, wantedSchweppes);
				market.banana -= wantedbanana;
				market.schweppes -= wantedSchweppes;
			}
			if (wantedbanana <= market.banana && wantedSchweppes > market.schweppes)
			{
				customer->announceLeaving(customer->getLeavingTime(), wantedbanana, market.schweppes);
				market.banana -= wantedbanana;
				market.schweppes = 0;

			}
			else if (wantedbanana > market.banana && wantedSchweppes <= market.schweppes)
			{
				customer->announceLeaving(customer->getLeavingTime(), market.banana, wantedSchweppes);
				market.schweppes -= wantedSchweppes;
				market.banana = 0;
			}
			else
			{
				customer->announceLeaving(customer->getLeavingTime(), market.banana, market.schweppes);
				market.schweppes = 0;
				market.banana = 0;
			}
		}
		market.customersByDeparture.dequeue();
	}

	static void serveCustomer(MrazMarket& market)
	{
		const Customer* customer = &(market.customersToArive.first());
		int wantedbanana = customer->banana;
		int wantedSchweppes = customer->schweppes;
		if (wantedbanana <= market.banana && wantedSchweppes <= market.schweppes)
		{
		   
			market.banana -= wantedbanana;
			market.schweppes -= wantedSchweppes;
			customer->announceLeaving(customer->arriveMinute, wantedbanana, wantedSchweppes);
			market.customersByDeparture.dequeue();
		}
		else
		{
			market.waitingForRestocking.insert(market.waitingForRestocking.begin(), *customer);
			if (market.availableWorkers > 0)
			{
				if (wantedbanana <= market.banana && wantedSchweppes > market.schweppes)
				{
					if (!isBeingShippedBefore(market, ResourceType::schweppes, customer->getLeavingTime()))
					{
						sendWorker(market, customer->arriveMinute, ResourceType::schweppes);
						market.availableWorkers--;
					}
					
				}
				else if (wantedbanana > market.banana && wantedSchweppes <= market.schweppes)
				{
					if (!isBeingShippedBefore(market, ResourceType::banana,customer->getLeavingTime()))
					{
						sendWorker(market, customer->arriveMinute, ResourceType::banana);
						market.availableWorkers--;
					}
				}
				else
				{
					if (market.availableWorkers > 1)
					{
						if (!isBeingShippedBefore(market, ResourceType::banana, customer->getLeavingTime()))
						{
							sendWorker(market, customer->arriveMinute, ResourceType::banana);
							market.availableWorkers--;
						}
						if (!isBeingShippedBefore(market, ResourceType::schweppes, customer->getLeavingTime()))
						{
							sendWorker(market, customer->arriveMinute, ResourceType::schweppes);
							market.availableWorkers--;
						}
					}
					else
					{
						if (wantedbanana == wantedSchweppes)
						{

							if (!isBeingShippedBefore(market, ResourceType::banana, customer->getLeavingTime()))
							{
								sendWorker(market, customer->arriveMinute, ResourceType::banana);
								market.availableWorkers--;
							}
						}
						else
						{
							if (wantedbanana > wantedSchweppes)
							{
								if (!isBeingShippedBefore(market, ResourceType::banana, customer->getLeavingTime()))
								{
									sendWorker(market, customer->arriveMinute, ResourceType::banana);
									market.availableWorkers--;
								}
							}
							else
							{
								if (!isBeingShippedBefore(market, ResourceType::schweppes, customer->getLeavingTime()))
								{
									sendWorker(market, customer->arriveMinute, ResourceType::schweppes);
									market.availableWorkers--;
								}
							}
						}
					}
				}

			}
			
		}
		market.customersToArive.dequeue();
	}

	static bool isBeingShippedBefore(MrazMarket& market, ResourceType item, int time) {
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

	static void sendWorker(MrazMarket& market, int time,ResourceType item)
	{
		Worker worker(time, item);
		worker.announceLeaving();
		market.pushWorker(worker);
	};

};