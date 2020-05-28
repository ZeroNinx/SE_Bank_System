#include "Edit_user_form.h"
#include "Index.h"
#include "Index_user.h"
#include "Index_admin_user.h"

Edit_user_form::Edit_user_form(QWidget *p,int o):parent(p),opt(o)
{
	ui.setupUi(this);
	if (opt == 0)
	{
		Index_user* pr = (Index_user*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}
	else if (opt == 1)
	{
		Index* pr = (Index*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}
	else if (opt == 2)
	{
		Index_admin_user* pr = (Index_admin_user*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}
}

