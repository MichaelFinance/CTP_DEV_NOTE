#include <iostream>
#include <filesystem>
#include <thread>

#include "Config.h"
#include "TradeApi.h"


int main() {
	auto tradeApi = new TradeProxy::TradeApi();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	tradeApi->reqQryInstrument(Config::CFFEX);

	std::string input;
	std::cin >> input;
    return 0;
}
