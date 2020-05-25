#include "User.h"

using namespace std;
using namespace boost::property_tree;
User::User(ptree p)
{
	try
	{
		name = p.get<string>("name");
		//money = 
		is_male = p.get<bool>("sex");
		birthday = p.get<string>("birth");
		//active = 
	}
	catch (const std::exception&e)
	{
		see(qs(e.what()));
	}
}