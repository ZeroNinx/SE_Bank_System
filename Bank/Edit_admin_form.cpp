#include "Edit_admin_form.h"
#include "GradientsBtn.h"
#include "Index_admin.h"

using namespace std;
using namespace boost::beast::http;
using namespace boost::property_tree;

//构造函数
Edit_admin_form::Edit_admin_form(QWidget *p)
{
	parent = p;

	ui.setupUi(this);
	//设定窗口图标
	setWindowIcon(QIcon("Resources/icon.png"));

	//设定窗口文字
	ui.lbl_title->setText(qs("创建管理员"));

	//设定背景透明
	setAttribute(Qt::WA_TranslucentBackground, true);

	//隐藏标题栏
	setWindowFlags(Qt::FramelessWindowHint);

	//设置边框阴影
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(7);
	effect->setColor(Qt::black);
	effect->setOffset(0, 0);
	ui.frame_window->setGraphicsEffect(effect);

	//添加登录按钮
	GradientsBtn* btn_submit = new GradientsBtn(this, "创建");
	btn_submit->setGeometry(25, 444, 506, 36);
	btn_submit->raise();
	connect(btn_submit, &GradientsBtn::clicked, this, &Edit_admin_form::btn_submit_click);

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
		ui.lbl_account->setFont(font);
		ui.lbl_username->setFont(font);
		ui.lbl_password->setFont(font);
		ui.lbl_re_password->setFont(font);
		ui.cb_1->setFont(font);
		ui.cb_2->setFont(font);
		ui.cb_3->setFont(font);
		ui.cb_4->setFont(font);
		ui.cb_5->setFont(font);
		ui.cb_6->setFont(font);
		ui.cb_7->setFont(font);
		ui.cb_8->setFont(font);
	}

	QFile fontfile3(qs("Resources/圆体/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_submit->setFont(font);
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
}

//关闭按钮
void Edit_admin_form::btn_close_click()
{
	this->close();
}

//创建按钮
void Edit_admin_form::btn_submit_click()
{
	//输入处理
	if (ui.le_account->text().isEmpty() || ui.le_username->text().isEmpty() || ui.le_password->text().isEmpty() || ui.le_re_password->text().isEmpty())
	{
		see(qs("请输入完整！"));
		return;
	}
	if (ui.le_password->text() != ui.le_re_password->text())
	{
		see(qs("两次输入的密码不一样！"));
		return;
	}

	try
	{
		//建立连接
		Index_admin* p = (Index_admin*)parent;
		HttpConn* create = new HttpConn(p->host, p->port);

		//构造JSON
		ptree root;
		stringstream ss;

		root.put("account", ui.le_account->text().toStdString());
		root.put("username", ui.le_username->text().toStdString());
		root.put("password", ui.le_password->text().toStdString());

		ptree pms;
		ffor(i, 0, 7)
		{
			if (perms[i]->isChecked())
			{
				ptree perm;
				perm.put("", perm_list[i]);

				pms.push_back(make_pair("", perm));
			}
		}
		root.put_child("perms", pms);
		write_json(ss, root);

		//设定参数
		create->build(verb::post, "/user", 11);
		create->request.set(field::cookie, p->cookie);
		create->request.set(field::content_type, "application/json");
		create->request.set(field::content_length, ss.str().length());
		create->request.body() = ss.str();

		//连接
		create->connect();

		//解析JSON
		stringstream resp_ss;
		resp_ss << create->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 4201)
		{
			see(qs("创建成功！"));
			emit create_complete();
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

//拖拽操作
void Edit_admin_form::mousePressEvent(QMouseEvent* event)
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

void Edit_admin_form::mouseMoveEvent(QMouseEvent* event)
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

void Edit_admin_form::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}
