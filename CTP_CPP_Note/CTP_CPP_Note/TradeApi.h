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

		///����֪ͨ
		void OnRtnOrder(CThostFtdcOrderField* pOrder) override;

		///����¼��������Ӧ
		void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		///����¼�����ر�
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) override;

		///�ɽ�֪ͨ
		virtual void OnRtnTrade(CThostFtdcTradeField* pTrade);

		void reqQryInstrument(const std::string& exchangeId, const std::string& instrumentId = "") const;

		void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		void sendFakOrder(std::string instrumentId, char side, double price, int size);


		void sendFokOrder(std::string instrumentId, char side, double price, int size);





	private:
		CThostFtdcTraderApi* ctpTradeApi_;
	};
}
