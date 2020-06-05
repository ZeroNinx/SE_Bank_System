#include "User.h"

using namespace std;
using namespace boost::property_tree;
User::User(ptree p)
{
	try
	{
		username = p.get<string>("account");
		name = p.get<string>("name");
		is_male = p.get<bool>("sex");
		birthday = p.get<string>("birth");
	}
	catch (const std::exception&e)
	{
		see(qs(e.what()));
	}
}