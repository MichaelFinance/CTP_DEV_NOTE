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
		//登录请求响应
		void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo,
			int nRequestID, bool bIsLast) override;
		///深度行情通知
		void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) override;

		void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
		//行情订阅响应
		void subscribeMarketData(const std::string& instrumendId);




	private:
		CThostFtdcMdApi*  ctpMdApi_;

	};

}
