#pragma once
#include "stdafx.h"
class User
{

public:
	User(boost::property_tree::ptree p);

	std::string username;
	std::string name;
	bool is_male;
	std::string birthday;

};

