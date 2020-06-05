#pragma once
#include "stdafx.h"
class Account
{
public:
	Account(boost::property_tree::ptree p,std::string u);
	std::string id;
	std::string username;
	double money;
	bool freeze;
};

