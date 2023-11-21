#include "MarketApi.h"

#include <filesystem>
#include <iostream>
#include "Config.h"
#include "Instrument.h"

namespace MarketProxy
{
	namespace
	{
		static int MarketProxyRequestId = 0;

	}

	MarketApi::MarketApi()
	{
		auto currentPath = std::filesystem::current_path();
		currentPath += Config::TRADE_FLOW;
		auto flowPath = std::filesystem::create_directories(currentPath);
		ctpMdApi_ = CThostFtdcMdApi::CreateFtdcMdApi(currentPath.string().c_str());
		ctpMdApi_->RegisterSpi(this);
		ctpMdApi_->RegisterFront(const_cast<char*>(Config::MARKET_FROND_ADDRESS.c_str()));
		ctpMdApi_->Init();
	}

	void MarketApi::OnFrontConnected()
	{
		std::cout << __FUNCTION__ << std::endl;
		std::cout << "Market api connected successfully" << std::endl;

		CThostFtdcReqUserLoginField requestLogin = CThostFtdcReqUserLoginField();
		memset(&requestLogin, 0, sizeof(requestLogin));
		strcpy_s(requestLogin.BrokerID, Config::BROKER_ID.c_str());
		strcpy_s(requestLogin.UserID, Config::USER_ID.c_str());
		strcpy_s(requestLogin.Password, Config::PASSWORD.c_str());
		int reqResult = ctpMdApi_->ReqUserLogin(&requestLogin, ++MarketProxyRequestId);
		if (reqResult != 0)
			std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
		else
			std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
	}

	void MarketApi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo,
	                               int nRequestID, bool bIsLast)
	{
		std::cout << __FUNCTION__ << std::endl;
		if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
		{
			std::cout << "pRspInfo@" __FUNCTION__ "|ErrorID: " << pRspInfo->ErrorID << "|ErrorMsg: " << pRspInfo->
				ErrorMsg;
		}
		else
		{
			std::cout << __FUNCTION__ << "|Login success, LoginTime: " << pRspUserLogin->LoginTime << "|BrokerID: " <<
				pRspUserLogin->BrokerID
				<< "|UserID: " << pRspUserLogin->UserID;
		}
	}
	std::string instType(std::string instrumentId)
	{
		if (instrumentId.find("-c-") || instrumentId.find("-p-"))
			return "opt";
		else
			return "underlying";
	}
	void MarketApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
	{
		std::cout << pDepthMarketData->InstrumentID << " Bid: " << pDepthMarketData->BidPrice1
			<< " " << pDepthMarketData->BidVolume1 << " Ask: " << pDepthMarketData->AskPrice1
			<< " Bid: " << pDepthMarketData->AskVolume1 << std::endl;

		//Instrument* instPtrTem = new  Instrument();
		//instPtrTem->exchangeId(pDepthMarketData->ExchangeID);


		InstrumentPtr instPtr= std::make_shared<Instrument>();
		instPtr->exchangeId(pDepthMarketData->ExchangeID);
		instPtr->instrumentName(pDepthMarketData->InstrumentID);
		instPtr->productClass(instType(pDepthMarketData->ExchangeID));
		instPtr->exchangeId(pDepthMarketData->ExchangeID);
		//delete instPtrTem
	}
	void MarketApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
	{
		if (true) {}
	}
	void MarketApi::subscribeMarketData(const std::string& instrumendId)
	{
		char* buffer = (char*)instrumendId.c_str();
		char* myreq[1] = { buffer };
		int reqResult = ctpMdApi_->SubscribeMarketData(myreq, 1);

		if (reqResult != 0)
			std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
		else
			std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
	}
}
