#pragma once
#include "stdafx.h"
class Account
{
public:
	Account(boost::property_tree::ptree p);
	std::string id;
	double money;
	bool freeze;
};

