#include "Admin.h"

using namespace std;
using namespace boost::property_tree;

//¹¹Ôìº¯Êý
Admin::Admin(boost::property_tree::ptree JSON)
{
	try
	{
		username = JSON.get<string>("username");
		account = JSON.get<string>("account");
		ptree pm = JSON.get_child("perms");
		BOOST_FOREACH(ptree::value_type & i, pm)
			perms[i.second.data()] = true;
	}
	catch (const std::exception&e)
	{
		see(qs(e.what()));
	}
	
}
