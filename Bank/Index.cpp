#include "Index.h"
#include "Bank.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Index::Index(QWidget* p, string u) :parent(p), username(u)
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
		ui.lbl_money->setFont(font);
		ui.lbl_state->setFont(font);

		ui.btn_add_user->setFont(font);
		ui.btn_edit_user->setFont(font);
		ui.btn_remove_user->setFont(font);
		ui.btn_freeze_user->setFont(font);
		ui.btn_unfreeze_user->setFont(font);
	}


	//��ʼ��״̬��
	ui.lbl_welcome->setText(qs("��ӭ��") + qs8(username) + qs("    �û����ͣ�����Ա"));

	//��ʼ���û��б�
	init_user_list();
}

//��ʼ���û��б�
void Index::init_user_list()
{
	try
	{
		//��������
		Bank* p = (Bank*)parent;
		HttpConn* init = new HttpConn(p->host, p->port);
		init->build(verb::get,"/clients",11);
		init->request.set(field::cookie, p->cookie);
		init->connect();

		//����JSON
		ptree root;
		stringstream ss;
		ss << init->response.body();
		read_json(ss, root);

		int code = root.get<int>("code");
		if (code == 2001)
		{
			ptree data = root.get_child("data");

			//����û��б�
			QStringList sl;
			BOOST_FOREACH(ptree::value_type & v, data)
			{
				User u(v.second);
				users.push_back(u);
				sl << qs8(u.name);
			}

			QStringListModel* slm = new QStringListModel(sl);
			ui.lv_members->setModel(slm);
			ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
		}
		else
		{
			string msg = root.get<string>("msg");
			see(qs8(msg));
		}

	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}

//��С����ť
void Index::btn_minimize_click()
{
	this->showMinimized();
}

//�رհ�ť
void Index::btn_close_click()
{
	this->close();
	
	//��ʾ�����ڲ���ʱ�ö�
	parent->show();
	parent->raise();
}

//�б�ѡ��
void Index::lv_members_click(QModelIndex mi)
{
	int index = mi.row();
	User u = users[index];
	ui.le_name->setText(qs8(u.name));
	if (u.is_male)
		ui.rb_male->setChecked(true);
	else ui.rb_female->setChecked(true);
}

//��ק����
void Index::mousePressEvent(QMouseEvent* event)
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

void Index::mouseMoveEvent(QMouseEvent* event)
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

void Index::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}