#include "TradeApi.h"
#include <string>
#include <filesystem>
#include "Config.h"
#include <iostream>

namespace TradeProxy
{
	namespace 
	{
	static int TradeProxyRequestId = 0;
	}
	TradeApi::TradeApi()
	{
		auto currentPath = std::filesystem::current_path();
		currentPath += Config::TRADE_FLOW;
		auto flowPath = std::filesystem::create_directories(currentPath);
		ctpTradeApi_ = CThostFtdcTraderApi::CreateFtdcTraderApi(currentPath.string().c_str());
		ctpTradeApi_->RegisterSpi(this);
		ctpTradeApi_->RegisterFront(const_cast<char*>(Config::FROND_ADDRESS.c_str()));
		ctpTradeApi_->SubscribePrivateTopic(THOST_TERT_RESTART);
		ctpTradeApi_->SubscribePublicTopic(THOST_TERT_RESTART);
		ctpTradeApi_->Init();
	}

	void TradeApi::OnFrontConnected()
	{
		std::cout << __FUNCTION__ << std::endl;

		CThostFtdcReqAuthenticateField reqAuthenticate = { 0 };
		memset(&reqAuthenticate, 0, sizeof(reqAuthenticate));
		strcpy_s(reqAuthenticate.BrokerID, Config::BROKER_ID.c_str());
		strcpy_s(reqAuthenticate.UserID, Config::USER_ID.c_str());
		strcpy_s(reqAuthenticate.UserProductInfo, Config::USER_PRODUCT_INFO.c_str());
		strcpy_s(reqAuthenticate.AuthCode, Config::AUTH_CODE.c_str());
		strcpy_s(reqAuthenticate.AppID, Config::APP_ID.c_str());
		const int reqResult = ctpTradeApi_->ReqAuthenticate(&reqAuthenticate, ++TradeProxyRequestId);

		if (reqResult != 0)
			std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
		else
			std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
	}

	void TradeApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
	{
		std::cout << __FUNCTION__ << std::endl;
		if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
		{
			std::cout << "pRspInfo@" __FUNCTION__ "|ErrorID: " << pRspInfo->ErrorID << "|ErrorMsg: " << pRspInfo->ErrorMsg;
		}
		else
		{
			if (pRspAuthenticateField != nullptr)
			{
				std::cout << __FUNCTION__ << "AppID: " << pRspAuthenticateField->AppID
					<< "|AppType: " << pRspAuthenticateField->AppType
					<< "|BrokerID: " << pRspAuthenticateField->BrokerID
					<< "|UserProductInfo: " << pRspAuthenticateField->UserProductInfo;


				auto reqUserLogin = CThostFtdcReqUserLoginField();
				memset(&reqUserLogin, 0, sizeof(reqUserLogin));
				strcpy_s(reqUserLogin.BrokerID, Config::BROKER_ID.c_str());
				strcpy_s(reqUserLogin.UserID, Config::USER_ID.c_str());
				strcpy_s(reqUserLogin.Password, Config::PASSWORD.c_str());
				const int reqResult = ctpTradeApi_->ReqUserLogin(&reqUserLogin, ++TradeProxyRequestId);

				if (reqResult != 0)
					std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
				else
					std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;

			}
			else
			{
				std::cout << __FUNCTION__ << "pRspAuthenticateField nullptr";
			}
		}
	}

	void TradeApi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
	{
		std::cout << __FUNCTION__ << std::endl;
		if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
		{
			std::cout << "pRspInfo@" __FUNCTION__ "|ErrorID: " << pRspInfo->ErrorID << "|ErrorMsg: " << pRspInfo->ErrorMsg;
		}
		else
		{
			std::cout << __FUNCTION__ << "|Login success, LoginTime: " << pRspUserLogin->LoginTime << "|BrokerID: " <<
				pRspUserLogin->BrokerID
				<< "|UserID: " << pRspUserLogin->UserID;

			CThostFtdcSettlementInfoConfirmField myreq = CThostFtdcSettlementInfoConfirmField();
			memset(&myreq, 0, sizeof(myreq));
			strcpy_s(myreq.BrokerID, Config::BROKER_ID.c_str());
			strcpy_s(myreq.InvestorID, Config::USER_ID.c_str());

			const int reqResult = ctpTradeApi_->ReqSettlementInfoConfirm(&myreq, ++TradeProxyRequestId);

			if (reqResult != 0)
				std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
			else
				std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;;
		}
	}

	void TradeApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
	{
		std::cout << __FUNCTION__ << std::endl;

		if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
		{
			std::cout << "pRspInfo@" __FUNCTION__ "|ErrorID: " << pRspInfo->ErrorID << "|ErrorMsg: " << pRspInfo->ErrorMsg;
		}
		else
		{
			if (pSettlementInfoConfirm != nullptr)
			{
				std::cout << "Success to confirm settlement information, |BrokerID: " << pSettlementInfoConfirm->BrokerID
					<< "|ConfirmDate: " << pSettlementInfoConfirm->ConfirmDate
					<< "|ConfirmTime: " << pSettlementInfoConfirm->ConfirmTime
					<< "|InvestorID: " << pSettlementInfoConfirm->InvestorID
					<< "|SettlementID: " << pSettlementInfoConfirm->SettlementID;
			}
			else
			{
				std::cout << __FUNCTION__ << "pSettlementInfoConfirm nullptr";
			} 
			
		}
	}

	void TradeApi::reqQryInstrument(const std::string& exchangeId, const std::string& instrumentId) const
	{
		std::cout << __FUNCTION__ << std::endl;
		auto request = CThostFtdcQryInstrumentField();
		memset(&request, 0, sizeof(request));
		strcpy_s(request.ExchangeID, exchangeId.c_str());

		const int reqResult = ctpTradeApi_->ReqQryInstrument(&request, ++TradeProxyRequestId);
		std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;;
	}
	void TradeApi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
	{
		std::cout << __FUNCTION__ << std::endl;

		if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
		{
			std::cout << "pRspInfo@" __FUNCTION__ "|ErrorID: " << pRspInfo->ErrorID << "|ErrorMsg: " << pRspInfo->ErrorMsg;
		}
		else
		{
			if (pInstrument != nullptr)
			{
				std::cout << "|InstrumentID: " << pInstrument->InstrumentID
					<< "|UnderlyingInstrID: " << pInstrument->UnderlyingInstrID
					<< "|ExchangeID: " << pInstrument->ExchangeID
					<< std::endl;
			}
			else
			{
				std::cout << __FUNCTION__ << "pSettlementInfoConfirm nullptr";
			}

		}
	}
}

