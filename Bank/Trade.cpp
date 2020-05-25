#include "Trade.h"
#include "GradientsBtn.h"
using namespace std;

Trade::Trade()
{
	ui.setupUi(this);

	//�趨����ͼ��
	setWindowIcon(QIcon("Resources/icon.png"));

	//�趨��������
	ui.lbl_title->setText(qs("ת�˴���"));

	//��ӵ�¼��ť
	GradientsBtn* btn_submit = new GradientsBtn(this,"ת��");
	btn_submit->setGeometry(20, 190, 361, 41);
	btn_submit->raise();
	connect(btn_submit, &GradientsBtn::clicked, this, &Trade::btn_submit_click);

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
		ui.lbl_hint->setFont(font);
		ui.lbl_hint_2->setFont(font);
	}

	QFile fontfile3(qs("Resources/Բ��/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_submit->setFont(font);
	}

	//�������븡����
	QDoubleValidator* validator = new QDoubleValidator(0, 100, 2, this);
	ui.le_money->setValidator(validator);
}

//�رհ�ť
void Trade::btn_close_click()
{
	this->close();
}

//ִ�а�ť
void Trade::btn_submit_click()
{
	ui.le_money->clear();
	if (save)
		see(qs("���"));
	else see(qs("ȡ��"));
}


//��ק����
void Trade::mousePressEvent(QMouseEvent* event)
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

void Trade::mouseMoveEvent(QMouseEvent* event)
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

void Trade::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}