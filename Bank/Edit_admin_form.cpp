#include "Edit_admin_form.h"
#include "GradientsBtn.h"
#include "Index_admin.h"

using namespace std;
using namespace boost::beast::http;
using namespace boost::property_tree;

//���캯��
Edit_admin_form::Edit_admin_form(QWidget *p)
{
	parent = p;

	ui.setupUi(this);
	//�趨����ͼ��
	setWindowIcon(QIcon("Resources/icon.png"));

	//�趨��������
	ui.lbl_title->setText(qs("��������Ա"));

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
	GradientsBtn* btn_submit = new GradientsBtn(this, "����");
	btn_submit->setGeometry(25, 444, 506, 36);
	btn_submit->raise();
	connect(btn_submit, &GradientsBtn::clicked, this, &Edit_admin_form::btn_submit_click);

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

	QFile fontfile3(qs("Resources/Բ��/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_submit->setFont(font);
	}

	//��ʼ��Ȩ���б�
	perms[0] = ui.cb_1;
	perms[1] = ui.cb_2;
	perms[2] = ui.cb_3;
	perms[3] = ui.cb_4;
	perms[4] = ui.cb_5;
	perms[5] = ui.cb_6;
	perms[6] = ui.cb_7;
	perms[7] = ui.cb_8;
}

//�رհ�ť
void Edit_admin_form::btn_close_click()
{
	this->close();
}

//������ť
void Edit_admin_form::btn_submit_click()
{
	//���봦��
	if (ui.le_account->text().isEmpty() || ui.le_username->text().isEmpty() || ui.le_password->text().isEmpty() || ui.le_re_password->text().isEmpty())
	{
		see(qs("������������"));
		return;
	}
	if (ui.le_password->text() != ui.le_re_password->text())
	{
		see(qs("������������벻һ����"));
		return;
	}

	try
	{
		//��������
		Index_admin* p = (Index_admin*)parent;
		HttpConn* create = new HttpConn(p->host, p->port);

		//����JSON
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

		//�趨����
		create->build(verb::post, "/user", 11);
		create->request.set(field::cookie, p->cookie);
		create->request.set(field::content_type, "application/json");
		create->request.set(field::content_length, ss.str().length());
		create->request.body() = ss.str();

		//����
		create->connect();

		//����JSON
		stringstream resp_ss;
		resp_ss << create->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 4201)
		{
			see(qs("�����ɹ���"));
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

//��ק����
void Edit_admin_form::mousePressEvent(QMouseEvent* event)
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

void Edit_admin_form::mouseMoveEvent(QMouseEvent* event)
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

void Edit_admin_form::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}
