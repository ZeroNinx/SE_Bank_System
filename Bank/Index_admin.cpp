#include "Index_admin.h"

#include "Bank.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Index_admin::Index_admin(QWidget* p, string a) :parent(p), account(a)
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
	ffor(i, 0, 6)
	{
		shadow.push_back(new QGraphicsDropShadowEffect);
		shadow[i]->setBlurRadius(7);
		shadow[i]->setColor(Qt::black);
		shadow[i]->setOffset(0, 0);
	}
	ui.lv_members->setGraphicsEffect(shadow[0]);
	ui.le_search->setGraphicsEffect(shadow[1]);
	ui.btn_add_admin->setGraphicsEffect(shadow[2]);
	ui.btn_edit_admin->setGraphicsEffect(shadow[3]);
	ui.btn_remove_admin->setGraphicsEffect(shadow[4]);


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
		ui.lbl_state->setFont(font);
		ui.lbl_account->setFont(font);
		ui.lbl_username->setFont(font);
		ui.cb_1->setFont(font);
		ui.cb_2->setFont(font);
		ui.cb_3->setFont(font);
		ui.cb_4->setFont(font);
		ui.cb_5->setFont(font);
		ui.cb_6->setFont(font);
		ui.cb_7->setFont(font);
		ui.cb_8->setFont(font);
	}

	//初始化权限列表
	perms[0] = ui.cb_1;
	perms[1] = ui.cb_2;
	perms[2] = ui.cb_3;
	perms[3] = ui.cb_4;
	perms[4] = ui.cb_5;
	perms[5] = ui.cb_6;
	perms[6] = ui.cb_7;
	perms[7] = ui.cb_8;


	try
	{
		Bank* p = (Bank*)parent;
		HttpConn* init = new HttpConn(p->host, p->port);
		init->build(verb::get, "/users", 11);
		init->request.set(field::cookie, p->cookie);
		init->connect();
		//see(qs(init->response.body()));

		//解析JSON
		ptree root;
		stringstream ss;
		ss << init->response.body();
		read_json(ss, root);

		see(qs(ss.str()));

		int code = root.get<int>("code");
		if (code != 4001)
		{
			string msg = root.get<string>("msg");
			see(qs8(msg));
			return;
		}

		//初始化用户列表
		QStringList sl;

		//遍历管理员
		ptree data = root.get_child("data");
		BOOST_FOREACH(ptree::value_type& i, data)
		{
			//初始化管理员列表
			Admin admin = Admin(i.second);
			admins.push_back(admin);
			sl << qs8(admin.username);
		}

		//应用用户列表
		QStringListModel* slm = new QStringListModel(sl);
		ui.lv_members->setModel(slm);
		ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
		
	}
	catch (const std::exception& e)
	{
		see(e.what());
	}
}

//最小化按钮
void Index_admin::btn_minimize_click()
{
	this->showMinimized();
}

//关闭按钮
void Index_admin::btn_close_click()
{
	this->close();
	
	//显示父窗口并暂时置顶
	parent->show();
	parent->raise();
}

//成员列表选定
void Index_admin::lv_members_click(QModelIndex mi)
{
	//获取下标
	int index = mi.row();
	see(qs(to_string(index)));
	
	//获取用户名
	ui.le_account->setText(qs8(admins[index].account));
	ui.le_username->setText(qs8(admins[index].username));

	//获取权限
	ffor(i, 0, 7)
		perms[i]->setChecked(admins[index].perms[i]);
}

//增加管理员按钮
void Index_admin::btn_add_admin_click()
{
	
}

//拖拽操作
void Index_admin::mousePressEvent(QMouseEvent* event)
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

void Index_admin::mouseMoveEvent(QMouseEvent* event)
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

void Index_admin::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}