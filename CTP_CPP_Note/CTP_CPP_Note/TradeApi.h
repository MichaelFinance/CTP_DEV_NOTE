#pragma once
#include <string>
#include <ThostFtdcTraderApi.h>
#include <vector>

namespace TradeProxy
{
	namespace ReferenceData
	{
		std::vector<std::string> getContracts();
	}


	class TradeApi final : public CThostFtdcTraderSpi
	{
	public:
		virtual ~TradeApi() = default;
		TradeApi();
		void OnFrontConnected() override;
		auto OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo,
		                       int nRequestID, bool bIsLast) -> void override;

		auto OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo,
		                    int nRequestID, bool bIsLast) -> void override;

		void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		///报单通知
		void OnRtnOrder(CThostFtdcOrderField* pOrder) override;

		///报单录入请求响应
		void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		///报单录入错误回报
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) override;

		///成交通知
		virtual void OnRtnTrade(CThostFtdcTradeField* pTrade);

		void reqQryInstrument(const std::string& exchangeId, const std::string& instrumentId = "") const;

		void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		void sendFakOrder(std::string instrumentId, char side, double price, int size);


		void sendFokOrder(std::string instrumentId, char side, double price, int size);





	private:
		CThostFtdcTraderApi* ctpTradeApi_;
	};
}
