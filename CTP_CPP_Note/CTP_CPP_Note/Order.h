#pragma once
#include <string>
#include "Macro.h"

class Order
{
public:
	void setPrice(double price_)
	{
		price = price_;
	}

	void setSize(int size_)
	{
		size = size_;
	}

	void setOrderStatus(std::string orderStatus_)
	{
		orderStatus = orderStatus_;
	}

	double getPrice()
	{
		return price;
	}


private:
	double price;
	int size;
	std::string orderStatus;

};
SHARED_PTR(Order)