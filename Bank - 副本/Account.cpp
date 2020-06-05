#include "Account.h"

using namespace std;
using namespace boost::property_tree;

Account::Account(boost::property_tree::ptree p,string u):username(u)
{
	id = p.get<string>("id");
	freeze = p.get<bool>("freeze");
	money = p.get<double>("money");
}
