#include "Edit_user_form.h"
#include "GradientsBtn.h"
#include "Index.h"
#include "Index_user.h"
#include "Index_admin_user.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Edit_user_form::Edit_user_form(QWidget *p,User* u,int o)
{
	parent = p;
	user = u;
	opt = o;

	ui.setupUi(this);
	//�趨����ͼ��
	setWindowIcon(QIcon("Resources/icon.png"));

	//�趨��������
	ui.lbl_title->setText(qs("�༭��Ϣ"));

	//�趨����͸��
	setAttribute(Qt::WA_TranslucentBackground, true);

	//���ر�����
	setWindowFlags(Qt::FramelessWindowHint);

	//���ñ߿���Ӱ
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(7);
	effect->setColor(Qt::black);
	effect->setOffset(0, 0);
	ui.frame_window->setGraphicsEffect(effect);

	//��ӵ�¼��ť
	GradientsBtn* btn_submit = new GradientsBtn(this, "�༭");
	btn_submit->setGeometry(20, 300, 326, 36);
	btn_submit->raise();
	connect(btn_submit, &GradientsBtn::clicked, this, &Edit_user_form::btn_submit_click);

	//��������
	QFile fontfile(qs("Resources/Բ��/bold.TTF"));
	if (fontfile.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_title->setFont(font);

	}

	QFile fontfile2(qs("Resources/Բ��/medium.TTF"));
	if (fontfile2.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile2.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_name->setFont(font);
		ui.lbl_birthday->setFont(font);
		ui.lbl_password->setFont(font);
		ui.lbl_re_password->setFont(font);
		ui.lbl_sex->setFont(font);
		ui.rb_female->setFont(font);
		ui.rb_male->setFont(font);
	}

	QFile fontfile3(qs("Resources/Բ��/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_submit->setFont(font);
	}

	//��ȡ����
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

	//��ʼ������
	ui.le_name->setText(qs8(user->name));
	if (user->is_male)
		ui.rb_male->setChecked(true);
	else ui.rb_female->setChecked(true);
	ui.de_birthday->setDate(QDate::fromString(qs8(user->birthday), qs8("yyyy-MM-dd")));
}

//�رհ�ť
void Edit_user_form::btn_close_click()
{
	this->close();
}

//��¼��ť
void Edit_user_form::btn_submit_click()
{
	//������
	if (ui.le_name->text().isEmpty() || ui.le_password->text().isEmpty() || ui.le_re_password->text().isEmpty())
	{
		see(qs("������������"));
		return;
	}
	if(ui.le_password->text()!=ui.le_re_password->text())
	{
		see(qs("������������벻һ����"));
		return;
	}

	//��������
	ptree req;
	stringstream ss;
	req.put("account", user->username);
	req.put("name",ui.le_name->text().toStdString());
	req.put("password", ui.le_password->text().toStdString());
	req.put("sex", ui.rb_male->isChecked() ? 1 : 0);
	req.put("birth", ui.de_birthday->date().toString("yyyy-MM-dd").toStdString());
	write_json(ss,req);
	//see(qs(ss.str()));

	try
	{
		//��������
		HttpConn* edit = new HttpConn(host, port);
		edit->build(verb::put, "/client", 11);
		edit->request.set(field::content_type, "application/json");
		edit->request.set(field::content_length, ss.str().length());
		edit->request.set(field::cookie, cookie);
		edit->request.body() = ss.str();
		edit->connect();

		//����JSON
		stringstream resp_ss;
		resp_ss << edit->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 2101)
		{
			see(qs("�޸ĳɹ���"));

			user->birthday = ui.de_birthday->date().toString("yyyy-MM-dd").toStdString();
			user->name = ui.le_name->text().toStdString();
			user->is_male = ui.rb_male->isChecked() ? 1 : 0;

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


//��ק����
void Edit_user_form::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = true;
		//������ĳ�ʼλ��
		mouseStartPoint = event->globalPos();
		//mouseStartPoint = event->pos();
		//��ô��ڵĳ�ʼλ��
		windowTopLeftPoint = this->frameGeometry().topLeft();
	}
}

void Edit_user_form::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bDrag)
	{
		//�������ƶ��ľ���
		QPoint distance = event->globalPos() - mouseStartPoint;
		//QPoint distance = event->pos() - mouseStartPoint;
		//�ı䴰�ڵ�λ��
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