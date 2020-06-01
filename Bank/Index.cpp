#include "Index.h"
#include "Bank.h"
#include "Edit_user_form.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//构造函数s
Index::Index(QWidget* p, string u) :parent(p), username(u)
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
	ffor(i, 0, 8)
	{
		shadow.push_back(new QGraphicsDropShadowEffect);
		shadow[i]->setBlurRadius(7);
		shadow[i]->setColor(Qt::black);
		shadow[i]->setOffset(0, 0);
	}
	ui.lv_members->setGraphicsEffect(shadow[0]);
	ui.le_search->setGraphicsEffect(shadow[1]);
	ui.btn_add_user->setGraphicsEffect(shadow[2]);
	ui.btn_remove_user->setGraphicsEffect(shadow[3]);
	ui.btn_edit_user->setGraphicsEffect(shadow[4]);
	ui.btn_add_account->setGraphicsEffect(shadow[5]);
	ui.btn_freeze_account->setGraphicsEffect(shadow[6]);
	ui.btn_unfreeze_account->setGraphicsEffect(shadow[7]);
	ui.btn_remove_account->setGraphicsEffect(shadow[8]);

	QGraphicsDropShadowEffect* shadow_div = new QGraphicsDropShadowEffect;
	shadow_div->setBlurRadius(3);
	shadow_div->setColor(Qt::black);
	shadow_div->setOffset(0, 0);
	ui.lbl_state_div->setGraphicsEffect(shadow_div);

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
		ui.lbl_welcome->setFont(font);
		ui.lbl_name->setFont(font);
		ui.lbl_money->setFont(font);
		ui.lbl_sex->setFont(font);
		ui.rb_male->setFont(font);
		ui.rb_female->setFont(font);
		ui.lbl_birthday->setFont(font);
		ui.lbl_money->setFont(font);
		ui.lbl_state->setFont(font);

		ui.btn_add_user->setFont(font);
		ui.btn_edit_user->setFont(font);
		ui.btn_remove_user->setFont(font);
		ui.btn_freeze_account->setFont(font);
		ui.btn_unfreeze_account->setFont(font);
		ui.btn_add_account->setFont(font);
		ui.btn_remove_account->setFont(font);
	}


	//初始化状态栏
	ui.lbl_welcome->setText(qs("欢迎！") + qs8(username) + qs("    用户类型：管理员"));

	//取得参数
	Bank* pr = (Bank*)parent;
	host = pr->host;
	port = pr->port;
	cookie = pr->cookie;

	//初始化用户列表
	init_user_list();
}

//初始化用户列表
void Index::init_user_list()
{
	users.clear();

	try
	{
		//建立连接
		Bank* p = (Bank*)parent;
		HttpConn* init = new HttpConn(p->host, p->port);
		init->build(verb::get,"/clients",11);
		init->request.set(field::cookie, p->cookie);
		init->connect();

		//解析JSON
		ptree root;
		stringstream ss;
		ss << init->response.body();
		read_json(ss, root);

		int code = root.get<int>("code");
		if (code == 2001)
		{
			ptree data = root.get_child("data");

			//添加用户列表
			QStringList sl;
			BOOST_FOREACH(ptree::value_type & v, data)
			{
				User u(v.second);
				users.push_back(u);
				sl << qs8(u.name);
			}

			QStringListModel* slm = new QStringListModel(sl);
			ui.lv_members->setModel(slm);
			ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
		}
		else
		{
			string msg = root.get<string>("msg");
			see(qs8(msg));
		}

	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}

//列表选中
void Index::lv_members_click(QModelIndex mi)
{
	int index = mi.row();
	current_index = index;
	User u = users[index];
	ui.le_name->setText(qs8(u.name));
	if (u.is_male)
		ui.rb_male->setChecked(true);
	else ui.rb_female->setChecked(true);
}

//添加客户
void Index::btn_add_user_click()
{
	Edit_user_form* add = new Edit_user_form(this, NULL, 1, verb::post);
	connect(add, &Edit_user_form::edit_complete, this, &Index::init_user_list);
	add->show();
}

//删除客户
void Index::btn_remove_user_click()
{
	try
	{
		Bank* p = (Bank*)parent;
		HttpConn* remove = new HttpConn(p->host, p->port);

		//设定参数
		remove->build(verb::delete_, "/client/"+users[current_index].username, 11);
		remove->request.set(field::cookie, p->cookie);

		//连接
		remove->connect();

		//解析JSON
		stringstream resp_ss;
		resp_ss << remove->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 2301)
		{
			see(qs("删除成功！"));
			init_user_list();
			close();
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

//编辑客户
void Index::btn_edit_user_click()
{
	Edit_user_form* edit = new Edit_user_form(this, &users[current_index], 1, verb::put);
	connect(edit, &Edit_user_form::edit_complete, this, &Index::init_user_list);
	edit->show();
}

//添加账户
void Index::btn_add_account_click() 
{

}

//冻结账户
void Index::btn_freeze_account_click()
{

}

//解冻账户
void Index::btn_unfreeze_account_click()
{

}

//删除账户
void Index::btn_remove_account_click()
{

}


//最小化按钮
void Index::btn_minimize_click()
{
	this->showMinimized();
}

//关闭按钮
void Index::btn_close_click()
{
	this->close();
	
	//显示父窗口并暂时置顶
	parent->show();
	parent->raise();
}

//拖拽操作
void Index::mousePressEvent(QMouseEvent* event)
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

void Index::mouseMoveEvent(QMouseEvent* event)
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

void Index::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}