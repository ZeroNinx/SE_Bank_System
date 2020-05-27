#pragma once
#include "stdafx.h"

//管理员类
class Admin
{
public:
	Admin(boost::property_tree::ptree JSON);
	std::string username;
	std::string account;
	bool perms[8];

private:

	//所有权限
	const std::string all_perms[8] =
	{ 
		"cl_ge",
		"cl_up",
		"cl_ad",
		"cl_del",
		"ac_ad",
		"ac_ge",
		"ac_fre",
		"ac_del" 
	};
};

