#pragma once
#include <string>
#include <ThostFtdcTraderApi.h>

namespace TradeProxy
{
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

		void reqQryInstrument(const std::string& exchangeId, const std::string& instrumentId = "") const;

		void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;





	private:
		CThostFtdcTraderApi* ctpTradeApi_;
	};
}
