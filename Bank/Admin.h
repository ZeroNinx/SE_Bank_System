#pragma once
#include "stdafx.h"

//����Ա��
class Admin
{
public:
	Admin(boost::property_tree::ptree JSON);
	std::string username;
	std::string account;
	std::map<std::string, bool> perms;
};

