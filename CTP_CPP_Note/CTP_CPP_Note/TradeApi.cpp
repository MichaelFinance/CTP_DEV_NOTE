#include "TradeApi.h"
#include <string>
#include <filesystem>
#include "Config.h"
#include <iostream>

namespace TradeProxy
{
	static std::vector<std::string> contracts;
	std::vector<std::string> ReferenceData::getContracts()
	{
		return contracts;
	}
	
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
		ctpTradeApi_->RegisterFront(const_cast<char*>(Config::TRADE_FROND_ADDRESS.c_str()));
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
				std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
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


	void TradeApi::OnRtnOrder(CThostFtdcOrderField* pOrder)
	{
		std::cout << __FUNCTION__ << std::endl;


	}

	void TradeApi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	void TradeApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo)
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	void TradeApi::OnRtnTrade(CThostFtdcTradeField* pTrade)
	{
		std::cout << __FUNCTION__ << std::endl;
	}


	void TradeApi::reqQryInstrument(const std::string& exchangeId, const std::string& instrumentId) const
	{
		std::cout << __FUNCTION__ << std::endl;
		auto request = CThostFtdcQryInstrumentField();
		memset(&request, 0, sizeof(request));
		strcpy_s(request.ExchangeID, exchangeId.c_str());

		const int reqResult = ctpTradeApi_->ReqQryInstrument(&request, ++TradeProxyRequestId);
		std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
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
				contracts.emplace_back(pInstrument->InstrumentID);
			}
			else
			{
				std::cout << __FUNCTION__ << "pSettlementInfoConfirm nullptr";
			}

		}
	}

	void TradeApi::sendFakOrder(std::string instrumentId, char side, double price, int size)
	{
		auto order = CThostFtdcInputOrderField();
		memset(&order, 0x00, sizeof(order));
		strcpy_s(order.BrokerID, Config::BROKER_ID.c_str());
		strcpy_s(order.InvestorID, Config::USER_ID.c_str());
		strcpy_s(order.InstrumentID, instrumentId.c_str());
//		strcpy_s(order.UserID, Config::USER_ID.c_str());
		order.Direction = side;
		order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		order.TimeCondition = THOST_FTDC_TC_IOC; // 立即完成，否则撤销
		order.VolumeCondition = THOST_FTDC_VC_AV; ///任何数量
		order.MinVolume = 1;
		order.ContingentCondition = THOST_FTDC_CC_Immediately; //立即
		order.StopPrice = 0;
		order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //非强平
		order.IsAutoSuspend = 0;
		order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
		order.LimitPrice = price;
		order.VolumeTotalOriginal = size;
		order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		const int reqResult = ctpTradeApi_->ReqOrderInsert(&order, ++TradeProxyRequestId);
		std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
	}


	void TradeApi::sendFokOrder(std::string instrumentId, char side, double price, int size)
	{

		auto order = CThostFtdcInputOrderField();
		memset(&order, 0x00, sizeof(order));
		strcpy_s(order.BrokerID, Config::BROKER_ID.c_str());
		strcpy_s(order.InvestorID, Config::USER_ID.c_str());
		strcpy_s(order.InstrumentID, instrumentId.c_str());
		strcpy_s(order.UserID, Config::USER_ID.c_str());
		order.Direction = side;
		order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		order.TimeCondition = THOST_FTDC_TC_IOC; // 立即完成，否则撤销
		order.VolumeCondition = THOST_FTDC_VC_AV; ///任何数量
		order.MinVolume = 1;
		order.ContingentCondition = THOST_FTDC_CC_Immediately; //立即
		order.StopPrice = 0;
		order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //非强平
		order.IsAutoSuspend = 0;
		order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
		order.LimitPrice = price;
		order.VolumeTotalOriginal = size;
		order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		const int reqResult = ctpTradeApi_->ReqOrderInsert(&order, ++TradeProxyRequestId);
		std::cout << __FUNCTION__ << "|response: " << reqResult << std::endl;
	}

}

