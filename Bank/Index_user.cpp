#include "Index_user.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Index_user::Index_user(QWidget* p, User* u) :parent(p), user(u)
{
	ui.setupUi(this);

	//�趨����ͼ��
	setWindowIcon(QIcon("Resources/icon.png"));

	//�趨����͸��
	setAttribute(Qt::WA_TranslucentBackground, true);

	//���ر�����
	setWindowFlags(Qt::FramelessWindowHint);

	//���ñ߿���Ӱ
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

	//����ë����Ч��
	QGraphicsBlurEffect* blur_window = new QGraphicsBlurEffect;
	blur_window->setBlurRadius(3);
	ui.frame_blur->setGraphicsEffect(blur_window);

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

	//��ʼ�û���ϸ
	ui.lbl_welcome->setText(qs("��ӭ��") + qs8(user->name) + qs("    �û����ͣ��ͻ�"));
	ui.le_name->setText(qs8(user->name));
	if (user->is_male)
		ui.rb_male->setChecked(true);
	else ui.rb_female->setChecked(true);

	//��ʼ���û��б�
	QStringList sl;
	sl << qs("С��") << qs("С��");
	QStringListModel* slm = new QStringListModel(sl);
	ui.lv_members->setModel(slm);
	ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//��С����ť
void Index_user::btn_minimize_click()
{
	this->showMinimized();
}

//�رհ�ť
void Index_user::btn_close_click()
{
	this->close();
	
	//��ʾ�����ڲ���ʱ�ö�
	parent->show();
	parent->raise();
}

//ת�˰�ť
void Index_user::btn_trade_click()
{
	trade_form->show();
	trade_form->raise();
}

//��ק����
void Index_user::mousePressEvent(QMouseEvent* event)
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


void Index_user::mouseMoveEvent(QMouseEvent* event)
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

void Index_user::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}