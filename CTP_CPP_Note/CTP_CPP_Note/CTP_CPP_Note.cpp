#include <iostream>
#include <filesystem>
#include <thread>

#include "Config.h"
#include "MarketApi.h"
#include "TradeApi.h"


int main() {
	std::string input;
	auto tradeApi = new TradeProxy::TradeApi();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	tradeApi->reqQryInstrument(Config::CFFEX);
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	//tradeApi->reqQryInstrument(Config::CZCE);
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	//tradeApi->reqQryInstrument(Config::DCE);
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	//tradeApi->reqQryInstrument(Config::INE);
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
	//tradeApi->reqQryInstrument(Config::SHFE);

	auto marketApi = new MarketProxy::MarketApi();
	std::this_thread::sleep_for(std::chrono::milliseconds(4000));

	

	std::vector<std::string> allConstracts = TradeProxy::ReferenceData::getContracts();
	const int numb = allConstracts.size();
	for (int i = 0; i < numb; ++i)
	{
		marketApi->subscribeMarketData(allConstracts[i]);
	}

	std::string instrumentId = "MO2401-P-5400";
	char side = '0';
	double price = 10;
	int size = 20;

	tradeApi->sendFakOrder(instrumentId, side, price, size);
	std::cin >> input;
	tradeApi->sendFokOrder(instrumentId, side, price, size);






	/*marketApi->subscribeMarketData("IO2403-P-3600");*/

	std::cin >> input;
    return 0;
}
