#pragma once
#include "MacroDefine.h"
#include <string>


class Instrument
{
public:
	// setter
	void exchangeId(const std::string  exchangeId) { exchangeID_ = exchangeId; }
	void instrumentName(const std::string& instrumentName) { instrumentName_ = instrumentName; }
	void productClass(const std::string  productClass) { productClass_ = productClass; }
	void deliveryYear(const int deliveryYear) { deliveryYear_ = deliveryYear; }
	void deliveryMonth(const int deliveryMonth) { deliveryMonth_ = deliveryMonth; }
	void maxMarketOrderVolume(const int maxMarketOrderVolume) { maxMarketOrderVolume_ = maxMarketOrderVolume; }
	void minMarketOrderVolume(const int minMarketOrderVolume) { minMarketOrderVolume_ = minMarketOrderVolume; }
	void maxLimitOrderVolume(const int maxLimitOrderVolume) { maxLimitOrderVolume_ = maxLimitOrderVolume; }
	void minLimitOrderVolume(const int minLimitOrderVolume) { minLimitOrderVolume_ = minLimitOrderVolume; }
	void volumeMultiple(const int volumeMultiple) { volumeMultiple_ = volumeMultiple; }
	void priceTick(const double priceTick) { priceTick_ = priceTick; }
	void expireDate(const std::string& expireDate) { expireDate_ = expireDate; }
	void startDelivDate(const std::string& startDelivDate) { startDelivDate_ = startDelivDate; }
	void endDelivDate(const std::string& endDelivDate) { endDelivDate_ = endDelivDate; }
	void longMarginRatio(const double longMarginRatio) { longMarginRatio_ = longMarginRatio; }
	void shortMarginRatio(const double shortMarginRatio) { shortMarginRatio_ = shortMarginRatio; }
	void strikePrice(const double strikePrice) { strikePrice_ = strikePrice; }
	void optionsType(const std::string  optionsType) { optionsType_ = optionsType; }
	void underlyingMultiple(const double underlyingMultiple) { underlyingMultiple_ = underlyingMultiple; }
	void instrumentId(const std::string& instrumentID) { instrumentId_ = instrumentID; }
	void exchangeInstId(const std::string& exchangeInstId) { exchangeInstId_ = exchangeInstId; }
	void productId(const std::string& productId) { productID_ = productId; }
	void underlyingInstrId(const std::string& underlyingInstrId) { underlyingInstrID_ = underlyingInstrId; }

	// getter
	const std::string  exchangeId() const { return exchangeID_; }
	const std::string& instrumentName() const { return instrumentName_; }
	const std::string  productClass() const { return productClass_; }
	const int deliveryYear() const { return deliveryYear_; }
	const int deliveryMonth() const { return deliveryMonth_; }
	const int maxMarketOrderVolume() const { return maxMarketOrderVolume_; }
	const int minMarketOrderVolume() const { return minMarketOrderVolume_; }
	const int maxLimitOrderVolume() const { return maxLimitOrderVolume_; }
	const int minLimitOrderVolume() const { return minLimitOrderVolume_; }
	const int volumeMultiple() const { return volumeMultiple_; }
	const double priceTick() const { return priceTick_; }
	const std::string& expireDate() const { return expireDate_; }
	const std::string& startDelivDate() const { return startDelivDate_; }
	const std::string& endDelivDate() const { return endDelivDate_; }
	const double longMarginRatio() const { return longMarginRatio_; }
	const double shortMarginRatio() const { return shortMarginRatio_; }
	const double strikePrice() const { return strikePrice_; }
	const std::string  optionsType() const { return optionsType_; }
	const double underlyingMultiple() const { return underlyingMultiple_; }
	const std::string& instrumentId() const { return instrumentId_; }
	const std::string& exchangeInstId() const { return exchangeInstId_; }
	const std::string& productId() const { return productID_; }
	const std::string& underlyingInstrId() const { return underlyingInstrID_; }

private:
	std::string exchangeID_ = ""; //交易所代码
	std::string instrumentName_ = ""; //合约名称
	std::string productClass_ = ""; //产品类型
	int deliveryYear_  = 0;
	int deliveryMonth_ = 0;
	int maxMarketOrderVolume_ = 0;
	int minMarketOrderVolume_ = 0;
	int maxLimitOrderVolume_ = 0;
	int minLimitOrderVolume_ = 0;
	int volumeMultiple_ = 0;
	double priceTick_ = 0.0;
	std::string expireDate_ = "";
	std::string startDelivDate_ = "";
	std::string endDelivDate_ = "";
	double longMarginRatio_ = 0.0;
	double shortMarginRatio_ = 0.0;
	double strikePrice_ = 0.0;
	std::string optionsType_ = "";
	double underlyingMultiple_ = 0.0;
	std::string instrumentId_ = "";
	std::string exchangeInstId_ = "";
	std::string productID_ = "";
	std::string underlyingInstrID_ = "";
};

SHARED_PTR(Instrument)
