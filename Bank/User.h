#pragma once
#include "stdafx.h"
class User
{

public:
	User(boost::property_tree::ptree p);

	std::string name;
	int money;
	bool is_male;
	std::string birthday;
	bool active;

};

