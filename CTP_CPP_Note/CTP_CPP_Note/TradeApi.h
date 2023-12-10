#pragma once
#include <string>
#include <ThostFtdcTraderApi.h>
#include <vector>
#include "LockingQueue.h"
#include "Macro.h"
#include <functional>
#include "Order.h"




namespace TradeProxy
{
	namespace ReferenceData
	{
		std::vector<std::string> getContracts();
	}

	namespace TradeMessage
	{
		// different message 
		enum class TradeTaskName
		{
			OnRspQryInstrument,
			OnRtnTrade,
			OnRtnOrder,
			OnRspOrderInsert,
			OnErrRtnOrderInsert,
			OnRspSettlementInfoConfirm,
			OnRspQryInvestorPosition
		};

		class TradeTask
		{
		public:
			void* getMessage() const { return message; }
			TradeTaskName getName() const { return name; }

			void setMessage(void* const message_) { this->message = message_; }
			void setName(const TradeTaskName name_) { this->name = name_; }

		private:
			void* message;
			TradeTaskName name;
		};
		SHARED_PTR(TradeTask);
	}

	class TradeApi;
	SHARED_PTR(TradeApi)

	class TradeApi final : public CThostFtdcTraderSpi
	{
	public:
		static TradeApiPtr instance();



		void start();
		virtual ~TradeApi() = default;
		void OnFrontConnected() override;
		auto OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo,
		                       int nRequestID, bool bIsLast) -> void override;

		auto OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo,
		                    int nRequestID, bool bIsLast) -> void override;

		void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		/* CTP 下单步骤
		 * 1. 发送下单请求：ReqOrderInsert
		 * 2. CTP 对订单进行校验，
		 *		a.CTP校验通过：OnRtnOrder
		 *		b.CTP校验未通过：OnErrRtnOrderInsert， OnRspOrderInsert
		 * 3.交易所校验订单
		 *		a. 校验通过和未通过:OnRtnOrder
		 * 4. 成交汇报
		 *		a. 全部成家和部分成交：OnRtnOrder， OnRtnTrade
		 */

		 /* OrderStatus
		  * 0 全部成交
		  * 1 部分成交，订单还在交易所撮合队列中
		  * 3 未成交，订单还在交易所撮合队列中
		  * 5 已撤销
		  * a 未知 订单已提交交易所，未从交易所收到确认信息
		  */

		void OnRtnOrder(CThostFtdcOrderField* pOrder) override;

		// 报单录入请求响应，当执行ReqOrderInsert后有字段填写不对之类的CTP报错则通过此接口返回
		void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		// 报单录入错误回报
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) override;

		///成交通知
		virtual void OnRtnTrade(CThostFtdcTradeField* pTrade);

		void reqQryInstrument(const std::string& exchangeId, const std::string& instrumentId = "") const;

		void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		void sendFakOrder(std::string instrumentId, char side, double price, int size);


		void sendFokOrder(std::string instrumentId, char side, double price, int size);

	private:
		TradeApi();
		static std::mutex tradeInstanceMtx;
		static TradeApiPtr tradeInstance;

		CThostFtdcTraderApi* ctpTradeApi_;
		std::mutex commonMsgQueueMtx;

		// Thread for process different message
		std::shared_ptr<std::thread> workThread_;

		// locking queue for storing message
		LockingQueue<TradeMessage::TradeTaskPtr> tradeMsgQ;

		// function used to process
		void processTradeTaskThread();

		void processOrder(const TradeMessage::TradeTaskPtr& orderInfo) const;



	};
	

}
