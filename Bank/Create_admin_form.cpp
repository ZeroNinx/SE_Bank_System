#include "Create_admin_form.h"
#include "GradientsBtn.h"
#include "Index_admin.h"

using namespace std;
using namespace boost::beast::http;
using namespace boost::property_tree;

//构造函数
Create_admin_form::Create_admin_form(QWidget *p)
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
	connect(btn_submit, &GradientsBtn::clicked, this, &Create_admin_form::btn_submit_click);

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
}

//关闭按钮
void Create_admin_form::btn_close_click()
{
	this->close();
}

//创建按钮
void Create_admin_form::btn_submit_click()
{
	string cookie = ((Index_admin*)parent)->cookie;
	see(qs(cookie));
}

//拖拽操作
void Create_admin_form::mousePressEvent(QMouseEvent* event)
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

void Create_admin_form::mouseMoveEvent(QMouseEvent* event)
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

void Create_admin_form::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}
