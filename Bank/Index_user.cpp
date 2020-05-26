#include "Index_user.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Index_user::Index_user(QWidget* p, User* u) :parent(p), user(u)
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
	ui.btn_edit->setGraphicsEffect(shadow[2]);
	ui.btn_trade->setGraphicsEffect(shadow[3]);

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
		ui.lbl_state->setFont(font);
		ui.btn_edit->setFont(font);
		ui.btn_trade->setFont(font);
	}

	//初始用户明细
	ui.lbl_welcome->setText(qs("欢迎！") + qs8(user->name) + qs("    用户类型：客户"));
	ui.le_name->setText(qs8(user->name));
	if (user->is_male)
		ui.rb_male->setChecked(true);
	else ui.rb_female->setChecked(true);

	//初始化用户列表
	QStringList sl;
	sl << qs("小明") << qs("小黄");
	QStringListModel* slm = new QStringListModel(sl);
	ui.lv_members->setModel(slm);
	ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//最小化按钮
void Index_user::btn_minimize_click()
{
	this->showMinimized();
}

//关闭按钮
void Index_user::btn_close_click()
{
	this->close();
	
	//显示父窗口并暂时置顶
	parent->show();
	parent->raise();
}

//转账按钮
void Index_user::btn_trade_click()
{
	trade_form->show();
	trade_form->raise();
}

//拖拽操作
void Index_user::mousePressEvent(QMouseEvent* event)
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


void Index_user::mouseMoveEvent(QMouseEvent* event)
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

void Index_user::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}