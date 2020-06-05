#include "Trade.h"
#include "GradientsBtn.h"
#include "Index_user.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Trade::Trade(QWidget* p,Account* a):account(a)
{
	ui.setupUi(this);
	Index_user* pr = (Index_user*)p;

	host = pr->host;
	port = pr->port;
	cookie = pr->cookie;


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
	if (ui.le_account->text().isEmpty() || ui.le_money->text().isEmpty())
	{
		see(qs("请输入完整!"));
		return;
	}
	if (stod(ui.le_money->text().toStdString()) >account->money)
	{
		see(qs("金额不足！!"));
		return;
	}

	try
	{
		HttpConn* trade = new HttpConn(host, port);

		ptree req;
		stringstream ss;
		req.put("accountId", account->id);
		req.put("anoAccountId", ui.le_account->text().toStdString());

		char money[50];
		sprintf(money, "%.2f", (stod(ui.le_money->text().toStdString())));

		req.put("money", money);
		write_json(ss, req);
		//see(qs(ss.str()));

		//设定参数
		trade->build(verb::put, "/transfer", 11);
		trade->request.set(field::cookie, cookie);
		trade->request.set(field::content_type, "application/json");
		trade->request.set(field::content_length, ss.str().length());
		trade->request.body() = ss.str();

		//连接
		trade->connect();

		//解析JSON
		stringstream resp_ss;
		resp_ss << trade->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 6001)
		{
			see(qs("转账成功！"));
			emit trade_success();
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