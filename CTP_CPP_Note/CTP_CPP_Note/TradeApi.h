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

		/* CTP �µ�����
		 * 1. �����µ�����ReqOrderInsert
		 * 2. CTP �Զ�������У�飬
		 *		a.CTPУ��ͨ����OnRtnOrder
		 *		b.CTPУ��δͨ����OnErrRtnOrderInsert�� OnRspOrderInsert
		 * 3.������У�鶩��
		 *		a. У��ͨ����δͨ��:OnRtnOrder
		 * 4. �ɽ��㱨
		 *		a. ȫ���ɼҺͲ��ֳɽ���OnRtnOrder�� OnRtnTrade
		 */

		 /* OrderStatus
		  * 0 ȫ���ɽ�
		  * 1 ���ֳɽ����������ڽ�������϶�����
		  * 3 δ�ɽ����������ڽ�������϶�����
		  * 5 �ѳ���
		  * a δ֪ �������ύ��������δ�ӽ������յ�ȷ����Ϣ
		  */

		void OnRtnOrder(CThostFtdcOrderField* pOrder) override;

		// ����¼��������Ӧ����ִ��ReqOrderInsert�����ֶ���д����֮���CTP������ͨ���˽ӿڷ���
		void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

		// ����¼�����ر�
		void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) override;

		///�ɽ�֪ͨ
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
