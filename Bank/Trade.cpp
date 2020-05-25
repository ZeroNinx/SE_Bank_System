#include "Trade.h"
#include "GradientsBtn.h"
using namespace std;

Trade::Trade()
{
	ui.setupUi(this);

	//设定窗口图标
	setWindowIcon(QIcon("Resources/icon.png"));

	//设定窗口文字
	ui.lbl_title->setText(qs("转账窗口"));

	//添加登录按钮
	GradientsBtn* btn_submit = new GradientsBtn(this,"转账");
	btn_submit->setGeometry(20, 190, 361, 41);
	btn_submit->raise();
	connect(btn_submit, &GradientsBtn::clicked, this, &Trade::btn_submit_click);

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
		ui.lbl_hint->setFont(font);
		ui.lbl_hint_2->setFont(font);
	}

	QFile fontfile3(qs("Resources/圆体/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_submit->setFont(font);
	}

	//限制输入浮点数
	QDoubleValidator* validator = new QDoubleValidator(0, 100, 2, this);
	ui.le_money->setValidator(validator);
}

//关闭按钮
void Trade::btn_close_click()
{
	this->close();
}

//执行按钮
void Trade::btn_submit_click()
{
	ui.le_money->clear();
	if (save)
		see(qs("存款"));
	else see(qs("取款"));
}


//拖拽操作
void Trade::mousePressEvent(QMouseEvent* event)
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

void Trade::mouseMoveEvent(QMouseEvent* event)
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

void Trade::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}