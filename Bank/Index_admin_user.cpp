#include "Index_admin_user.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Index_admin_user::Index_admin_user(QWidget* p, string u) :parent(p), username(u)
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
	ui.btn_add_user->setGraphicsEffect(shadow[2]);
	ui.btn_remove_user->setGraphicsEffect(shadow[3]);
	ui.btn_edit_user->setGraphicsEffect(shadow[4]);
	ui.btn_freeze_user->setGraphicsEffect(shadow[5]);
	ui.btn_unfreeze_user->setGraphicsEffect(shadow[6]);

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

	//��ʼ��״̬��
	ui.lbl_state->setText(qs("��ӭ��") + qs8(username) + qs("    �û����ͣ�����Ա"));

	//��ʼ���û��б�
	QStringList sl;
	sl << qs("С��") << qs("С��");
	QStringListModel* slm = new QStringListModel(sl);
	ui.lv_members->setModel(slm);
	ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//��С����ť
void Index_admin_user::btn_minimize_click()
{
	this->showMinimized();
}

//�رհ�ť
void Index_admin_user::btn_close_click()
{
	this->close();
	
	//��ʾ�����ڲ���ʱ�ö�
	parent->show();
	parent->raise();
}


//��ק����
void Index_admin_user::mousePressEvent(QMouseEvent* event)
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

void Index_admin_user::mouseMoveEvent(QMouseEvent* event)
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

void Index_admin_user::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}