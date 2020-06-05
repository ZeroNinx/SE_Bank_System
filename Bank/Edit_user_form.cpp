#include "Edit_user_form.h"
#include "GradientsBtn.h"
#include "Index.h"
#include "Index_user.h"
#include "Index_admin_user.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Edit_user_form::Edit_user_form(QWidget* p, User* u, int o, verb m)
{
	parent = p;
	user = u;
	opt = o;
	method = m;

	ui.setupUi(this);
	//设定窗口图标
	setWindowIcon(QIcon("Resources/icon.png"));

	//设定窗口文字
	if (method == verb::put)
		ui.lbl_title->setText(qs("编辑信息"));
	else ui.lbl_title->setText(qs("创建客户"));

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
	GradientsBtn* btn_submit = new GradientsBtn(this, method == verb::put ? "编辑" : "创建");
	btn_submit->setGeometry(25 , 315, 316, 36);
	btn_submit->raise();
	connect(btn_submit, &GradientsBtn::clicked, this, &Edit_user_form::btn_submit_click);

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
		ui.lbl_username->setFont(font);
		ui.lbl_name->setFont(font);
		ui.lbl_birthday->setFont(font);
		ui.lbl_password->setFont(font);
		ui.lbl_re_password->setFont(font);
		ui.lbl_sex->setFont(font);
		ui.rb_female->setFont(font);
		ui.rb_male->setFont(font);
	}

	QFile fontfile3(qs("Resources/圆体/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//载入，设置字体
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_submit->setFont(font);
	}

	//获取连接
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

	//初始化参数
	if (method == verb::put)
	{
		ui.le_username->setText(qs8(user->username));
		ui.le_username->setReadOnly(true);
		ui.le_name->setText(qs8(user->name));
		if (user->is_male)
			ui.rb_male->setChecked(true);
		else ui.rb_female->setChecked(true);
		ui.de_birthday->setDate(QDate::fromString(qs8(user->birthday), qs8("yyyy-MM-dd")));
	}
}

//关闭按钮
void Edit_user_form::btn_close_click()
{
	this->close();
}

//登录按钮
void Edit_user_form::btn_submit_click()
{
	//输入检查
	if (ui.le_username->text().isEmpty()|| ui.le_name->text().isEmpty() || ui.le_password->text().isEmpty() || ui.le_re_password->text().isEmpty()||(!ui.rb_female->isChecked()&&!ui.rb_male->isChecked()))
	{
		see(qs("请输入完整！"));
		return;
	}
	if(ui.le_password->text()!=ui.le_re_password->text())
	{
		see(qs("两次输入的密码不一样！"));
		return;
	}

	//生成数据
	ptree req;
	stringstream ss;
	req.put("account", ui.le_username->text().toStdString());
	req.put("name",ui.le_name->text().toStdString());
	req.put("password", ui.le_password->text().toStdString());
	req.put("sex", ui.rb_male->isChecked() ? 1 : 0);
	req.put("birth", ui.de_birthday->date().toString("yyyy-MM-dd").toStdString());
	write_json(ss,req);
	//see(qs(ss.str()));

	try
	{
		//发送请求
		HttpConn* edit = new HttpConn(host, port);
		edit->build(method, "/client", 11);
		edit->request.set(field::content_type, "application/json");
		edit->request.set(field::content_length, ss.str().length());
		edit->request.set(field::cookie, cookie);
		edit->request.body() = ss.str();
		edit->connect();

		//解析JSON
		stringstream resp_ss;
		resp_ss << edit->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");

		//修改
		if (code == 2101&&method == verb::put)
		{
			see(qs("修改成功！"));

			user->birthday = ui.de_birthday->date().toString("yyyy-MM-dd").toStdString();
			user->name = ui.le_name->text().toStdString();
			user->is_male = ui.rb_male->isChecked() ? 1 : 0;
			emit edit_complete();
			close();
		}
		else if (code == 2201)
		{

			see(qs("创建成功！"));
			emit edit_complete();
			close();
		}
		else
		{
			string msg = resp.get<string>("msg");
			see(qs8(msg));
		}
	}
	catch (const std::exception&e)
	{
		see(qs(e.what()));
	}
}


//拖拽操作
void Edit_user_form::mousePressEvent(QMouseEvent* event)
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

void Edit_user_form::mouseMoveEvent(QMouseEvent* event)
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

void Edit_user_form::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}