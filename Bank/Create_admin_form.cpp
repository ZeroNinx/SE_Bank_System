#include "Create_admin_form.h"
#include "GradientsBtn.h"
#include "Index_admin.h"

using namespace std;
using namespace boost::beast::http;
using namespace boost::property_tree;

//���캯��
Create_admin_form::Create_admin_form(QWidget *p)
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
	connect(btn_submit, &GradientsBtn::clicked, this, &Create_admin_form::btn_submit_click);

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
}

//�رհ�ť
void Create_admin_form::btn_close_click()
{
	this->close();
}

//������ť
void Create_admin_form::btn_submit_click()
{
	string cookie = ((Index_admin*)parent)->cookie;
	see(qs(cookie));
}

//��ק����
void Create_admin_form::mousePressEvent(QMouseEvent* event)
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

void Create_admin_form::mouseMoveEvent(QMouseEvent* event)
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

void Create_admin_form::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}
