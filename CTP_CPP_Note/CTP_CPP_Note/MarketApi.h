#pragma once
#include <ThostFtdcMdApi.h>
#include <string>

namespace MarketProxy
{
	class MarketApi final : public CThostFtdcMdSpi
	{
	public:
		virtual ~MarketApi() = default;

		MarketApi();
		void OnFrontConnected() override;
		//��¼������Ӧ
		void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo,
			int nRequestID, bool bIsLast) override;
		///�������֪ͨ
		void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override;

		void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
		//���鶩����Ӧ
		void subscribeMarketData(const std::string& instrumendId);




	private:
		CThostFtdcMdApi*  ctpMdApi_;

	};

}
