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

		ffor(i, 0, 7)
			perms[i] = 0;

		BOOST_FOREACH(ptree::value_type & i, pm)
		{
			ffor(index, 0, 7)
			{
				if (i.second.data() == all_perms[index])
					perms[index] = true;
			}
		}
	}
	catch (const std::exception&e)
	{
		see(qs(e.what()));
	}
	
}
