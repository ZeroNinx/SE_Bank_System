#pragma once
#include "stdafx.h"

//管理员类
class Admin
{
public:
	Admin(boost::property_tree::ptree JSON);
	std::string username;
	std::string account;
	std::map<std::string, bool> perms;
};

