#include "Index_account.h"
#include "Index.h"
#include "Index_admin.h"
#include "Edit_user_form.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//构造函数s
Index_account::Index_account(QWidget* p,User u,int opt) :parent(p), user(u)
{
	ui.setupUi(this);

	//设定窗口图标
	setWindowIcon(QIcon("Resources/icon.png"));

	//设定背景透明
	setAttribute(Qt::WA_TranslucentBackground, true);

	//隐藏标题栏
	setWindowFlags(Qt::FramelessWindowHint);

	//设置边框阴影
	vector<QGraphicsDropShadowEffect*> shadow;
	ffor(i, 0, 5)
	{
		shadow.push_back(new QGraphicsDropShadowEffect);
		shadow[i]->setBlurRadius(7);
		shadow[i]->setColor(Qt::black);
		shadow[i]->setOffset(0, 0);
	}
	ui.lv_members->setGraphicsEffect(shadow[0]);
	ui.le_search->setGraphicsEffect(shadow[1]);
	ui.btn_add_account->setGraphicsEffect(shadow[2]);
	ui.btn_freeze_account->setGraphicsEffect(shadow[3]);
	ui.btn_unfreeze_account->setGraphicsEffect(shadow[4]);
	ui.btn_remove_account->setGraphicsEffect(shadow[5]);

	QGraphicsDropShadowEffect* shadow_div = new QGraphicsDropShadowEffect;
	shadow_div->setBlurRadius(3);
	shadow_div->setColor(Qt::black);
	shadow_div->setOffset(0, 0);

	//设置毛玻璃效果
	QGraphicsBlurEffect* blur_window = new QGraphicsBlurEffect;
	blur_window->setBlurRadius(3);
	ui.frame_blur->setGraphicsEffect(blur_window);

	//检索字体
	QFile fontfile(qs("Resources/圆体/bold.TTF"));
	if (fontfile.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_title->setFont(font);
	}

	QFile fontfile2(qs("Resources/圆体/medium.TTF"));
	if (fontfile2.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile2.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_money->setFont(font);
		ui.lbl_state->setFont(font);

		ui.btn_freeze_account->setFont(font);
		ui.btn_unfreeze_account->setFont(font);
		ui.btn_add_account->setFont(font);
		ui.btn_remove_account->setFont(font);
	}


	//取得参数
	if (opt == 1)
	{
		Index* pr = (Index*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}
	else if (opt == 2)
	{
		Index_admin* pr = (Index_admin*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}

	//初始化用户列表
	init_user_list();
}

//初始化用户列表
void Index_account::init_user_list()
{
	accounts.clear();
	try
	{
		//获取账户列表
		HttpConn* get_account = new HttpConn(host, port);
		get_account->build(verb::get, "/account/" + user.username, 11);
		get_account->request.set(field::cookie, cookie);
		get_account->connect();
		//see(qs8(get_account->response.body()));

		//解析JSON
		ptree resp_json;
		stringstream resp_ss;
		resp_ss << get_account->response.body();
		read_json(resp_ss, resp_json);
		int code_a = resp_json.get<int>("code");
		if (code_a == 3001)
		{
			ptree data_a = resp_json.get_child("data");
			QStringList sl;
			BOOST_FOREACH(ptree::value_type & v, data_a)
			{
				Account a(v.second, user.username);
				accounts.push_back(a);
				sl << qs8(a.id);
			}
			QStringListModel* slm = new QStringListModel(sl);
			ui.lv_members->setModel(slm);
			ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
		}
		else
		{
			string msg = resp_json.get<string>("msg");
			see(qs8(msg));
		}

	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}

//列表选中
void Index_account::lv_members_click(QModelIndex mi)
{
	int index = mi.row();
	current_account_index = index;

	//显示账户
	Account acc = accounts[index];
	char money[50];
	sprintf(money, "%.2f", acc.money);
	ui.lbl_money_value->setText(qs8(money));
	if (acc.freeze)
	{
		ui.lbl_state_value->setText(qs("已冻结"));
		ui.lbl_state_value->setStyleSheet(qs("color:red;font:15px;"));
	}
	else
	{
		ui.lbl_state_value->setText(qs("正常"));
		ui.lbl_state_value->setStyleSheet(qs("color:green;font:15px;"));
	}
}

//添加账户
void Index_account::btn_add_account_click()
{
	try
	{
		HttpConn* add = new HttpConn(host, port);

		ptree req;
		stringstream ss;
		req.put("clientAccount", user.username);
		write_json(ss, req);

		//设定参数
		add->build(verb::post, "/account", 11);
		add->request.set(field::cookie, cookie);
		add->request.set(field::content_type, "application/json");
		add->request.set(field::content_length, ss.str().length());
		add->request.body() = ss.str();

		//连接
		add->connect();

		//解析JSON
		stringstream resp_ss;
		resp_ss << add->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 3201)
		{
			see(qs("创建成功！"));
			init_user_list();
		}
		else
		{
			string msg = resp.get<string>("msg");
			see(qs8(msg));
		}
	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}

//冻结账户
void Index_account::btn_freeze_account_click()
{
	if (current_account_index >= 0 && current_account_index < accounts.size())
	{
		try
		{
			HttpConn* remove = new HttpConn(host, port);
			remove->build(verb::delete_, "/account/"+accounts[current_account_index].id, 11);

			//设定参数
			remove->request.set(field::cookie, cookie);

			//连接
			remove->connect();

			//解析JSON
			stringstream resp_ss;
			resp_ss << remove->response.body();
			//see(qs8(resp_ss.str()));

			ptree resp;
			read_json(resp_ss, resp);
			int code = resp.get<int>("code");
			if (code == 3301)
			{
				see(qs("删除成功！"));
				init_user_list();
			}
			else
			{
				string msg = resp.get<string>("msg");
				see(qs8(msg));
			}
		}
		catch (const std::exception& e)
		{
			see(qs(e.what()));
		}
	}
}

//解冻账户
void Index_account::btn_unfreeze_account_click()
{

}

//删除账户
void Index_account::btn_remove_account_click()
{
	if (current_account_index >= 0 && current_account_index < accounts.size())
	{
		try
		{
			HttpConn* remove = new HttpConn(host, port);
			remove->build(verb::delete_, "/account/"+accounts[current_account_index].id, 11);

			//设定参数
			remove->request.set(field::cookie, cookie);

			//连接
			remove->connect();

			//解析JSON
			stringstream resp_ss;
			resp_ss << remove->response.body();
			//see(qs8(resp_ss.str()));

			ptree resp;
			read_json(resp_ss, resp);
			int code = resp.get<int>("code");
			if (code == 3301)
			{
				see(qs("删除成功！"));
				init_user_list();
			}
			else
			{
				string msg = resp.get<string>("msg");
				see(qs8(msg));
			}
		}
		catch (const std::exception& e)
		{
			see(qs(e.what()));
		}
	}
}


//最小化按钮
void Index_account::btn_minimize_click()
{
	this->showMinimized();
}

//关闭按钮
void Index_account::btn_close_click()
{
	this->close();

	//显示父窗口并暂时置顶
	parent->show();
	parent->raise();
}

//拖拽操作
void Index_account::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = true;
		//获得鼠标的初始位置
		mouseStartPoint = event->globalPos();
		//mouseStartPoint = event->pos();
		//获得窗口的初始位置
		windowTopLeftPoint = this->frameGeometry().topLeft();
	}
}

void Index_account::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bDrag)
	{
		//获得鼠标移动的距离
		QPoint distance = event->globalPos() - mouseStartPoint;
		//QPoint distance = event->pos() - mouseStartPoint;
		//改变窗口的位置
		this->move(windowTopLeftPoint + distance);
	}
}

void Index_account::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}