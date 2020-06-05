#include "Bank.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//初始化，构造函数
Bank::Bank(QFrame* parent) : QFrame(parent)
{
	ui.setupUi(this);
	
	//设定窗口图标
	setWindowIcon(QIcon("Resources/icon.png"));

	//设定背景透明
	setAttribute(Qt::WA_TranslucentBackground, true);

	//隐藏标题栏
	setWindowFlags(Qt::FramelessWindowHint);

	//加入选择按钮
	btn_select = new ScrollBtn(ui.frame_window);
	btn_select->setGeometry(205, 322, 56, 27);
	btn_select->raise();
	connect(btn_select, &ScrollBtn::move_over, this, &Bank::btn_select_move_over);

	//加入登录按钮
	btn_login = new GradientsBtn(ui.frame_window,"登录");
	btn_login->setGeometry(30, 360, 406, 41);
	btn_login->raise();
	connect(btn_login, &GradientsBtn::clicked, this, &Bank::btn_login_click);

	//管理员特殊入口

	//通关密语
	password_queue.push(Qt::Key_Up);
	password_queue.push(Qt::Key_Up);
	password_queue.push(Qt::Key_Down);
	password_queue.push(Qt::Key_Down);
	password_queue.push(Qt::Key_Left);
	password_queue.push(Qt::Key_Right);
	password_queue.push(Qt::Key_Left);
	password_queue.push(Qt::Key_Right);
	password_queue.push(Qt::Key_B);
	password_queue.push(Qt::Key_A);

	ui.lbl_img_index->installEventFilter(this);

	//设置边框阴影
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(6);
	effect->setColor(Qt::black);
	effect->setOffset(0, 0);
	ui.frame_window->setGraphicsEffect(effect);

	//设置字体
	QFile fontfile(qs("Resources/圆体/bold.TTF"));
	if (fontfile.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_username->setFont(font);
		ui.lbl_password->setFont(font);
	}

	QFile fontfile2(qs("Resources/圆体/medium.TTF"));
	if (fontfile2.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile2.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_user->setFont(font);
		ui.lbl_admin->setFont(font);
	}

	QFile fontfile3(qs("Resources/圆体/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_login->setFont(font);
	}

	//清空输入框
	ui.le_username->clear();
	ui.le_password->clear();
}


//最小化按钮
void Bank::btn_minimize_click()
{
	this->showMinimized();
}

//关闭按钮
void Bank::btn_close_click()
{
	this->close();
}

//登录状态选择器
void Bank::btn_select_move_over()
{
	is_admin = !is_admin;
}

//登录按钮
void Bank::btn_login_click()
{
	if (dev)
		target = "/login/dev";
	else
	{
		//输入检查
		if (ui.le_username->text().isEmpty() || ui.le_password->text().isEmpty())
		{
			see(qs("请输入完整！"));
			return;
		}
		if (is_admin)
			target = "/login/user";
		else target = "/login/client";
	}

	try
	{
		//创建连接
		HttpConn login(host, port);
		login.build(verb::post, target, 11);

		//登录请求
		if (!dev)
		{
			ptree login_json;
			stringstream login_ss;
			login_json.put("account", ui.le_username->text().toStdString());
			login_json.put("password", ui.le_password->text().toStdString());
			write_json(login_ss, login_json);

			login.request.set(field::content_length, login_ss.str().length());
			login.request.body() = login_ss.str();
		}

		//Request
		//设定参数
		login.request.set(field::content_type, "application/json");

		//通过套接字发送request
		login.connect();

		//Response
		response<string_body> resp = login.response;

		//for (auto i = resp.begin(); i != resp.end(); i++)
		//	see(qs(i->value().to_string()));

		//输出返回的数据
		//see(resp.body().c_str());

		//分析返回值
		stringstream resp_ss;
		ptree resp_json;
		resp_ss << resp.body();
		read_json(resp_ss, resp_json);

		//寻找状态码
		int code;
		if (resp_json.find("code") == resp_json.not_found())
		{
			see(qs("连接错误！"));
			return;
		}
		code = resp_json.get<int>("code");

		//解析状态码
		if (code != 1001)
		{
			string msg = resp_json.get<string>("msg");
			see(qs8(msg));
			return;
		}
		else
		{
			//取得cookie
			cookie = resp[field::set_cookie].to_string().substr(0, 43);
			//see(qs(cookie));

			if (dev)//开发者
			{
				if (is_admin)
				{
					Index_admin* index = new Index_admin(this, "");
					index->show();
					this->hide();
				}
				else
				{
					Index_admin_user* index = new Index_admin_user(this, "");
					index->show();
					this->hide();
				}
			}
			else
			{
				ptree data = resp_json.get_child("data");

				//管理员
				if (is_admin)
				{
					//获取参数
					string username = data.get<string>("username");
					

					//登陆成功，创建下一个窗口
					Index* index = new Index(this, username);
					index->show();
					this->hide();
				}
				else//客户
				{
					//初始化客户
					User* user = new User(data);

					//登陆成功，创建下一个窗口
					Index_user* index = new Index_user(this, user);
					index->show();
					this->hide();
				}
			}
		}
	}
	catch(exception e)
	{
		see(e.what());
	}
}

//事件过滤器
bool Bank::eventFilter(QObject* watched, QEvent* event)
{
	//管理员密道
	if (watched == ui.lbl_img_index)
	{	
		//捕获按键输入
		if (event->type() == QEvent::KeyRelease)
		{
			QKeyEvent* key_event = static_cast<QKeyEvent*>(event);

			//格式化按键队列
			while(key_queue.size()>=10)
				key_queue.pop();
			key_queue.push(key_event->key());
			if (key_queue.size() == 10)
			{
				bool correct = true;
				
				//比较密码
				ffor(cnt, 1, 10)
				{
					int current_key = key_queue.front();
					int password_key = password_queue.front();
					key_queue.pop();
					password_queue.pop();
					if (current_key != password_key)
						correct = false;
					key_queue.push(current_key);
					password_queue.push(password_key);
				}
				if (correct)//正确，解锁开发者模式
				{
					see(qs("恭喜解锁开发者模式！"));
					ui.lbl_user->setText(qs("客户管理"));
					ui.lbl_admin->setText(qs("管理员管理"));
					ui.le_username->setText(qs("开发者"));
					ui.le_password->setEchoMode(QLineEdit::Normal);
					ui.le_password->setText(qs("天下の一"));
					dev = true;
				}
			}

			//see(qs(to_string(key_event->key())));
		}
	}


	return QWidget::eventFilter(watched, event);
}

//拖拽操作(重写鼠标事件)
void Bank::mousePressEvent(QMouseEvent* event)
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

void Bank::mouseMoveEvent(QMouseEvent* event)
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

void Bank::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}