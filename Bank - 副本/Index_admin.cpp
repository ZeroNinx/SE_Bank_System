#include "Index_admin.h"
#include "Bank.h"
#include "Edit_user_form.h"
#include "Index_account.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//���캯��
Index_admin::Index_admin(QWidget* p, string u) :parent(p), username(u)
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
	ffor(i, 0, 7)
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
	ui.btn_account_list->setGraphicsEffect(shadow[5]);
	ui.btn_freeze_user->setGraphicsEffect(shadow[6]);
	ui.btn_unfreeze_user->setGraphicsEffect(shadow[7]);

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
		ui.lbl_sex->setFont(font);
		ui.rb_male->setFont(font);
		ui.rb_female->setFont(font);
		ui.lbl_birthday->setFont(font);

		ui.btn_add_user->setFont(font);
		ui.btn_edit_user->setFont(font);
		ui.btn_remove_user->setFont(font);
		ui.btn_account_list->setFont(font);
		ui.btn_freeze_user->setFont(font);
		ui.btn_unfreeze_user->setFont(font);
	}


	//��ʼ��״̬��
	ui.lbl_welcome->setText(qs("��ӭ��") + qs8(username) + qs("    �û����ͣ�����Ա"));

	//ȡ�ò���
	Bank* pr = (Bank*)parent;
	host = pr->host;
	port = pr->port;
	cookie = pr->cookie;

	//��ʼ���û��б�
	init_user_list();
}

//��ʼ���û��б�
void Index_admin::init_user_list()
{
	users.clear();
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

//�б�ѡ��
void Index_admin::lv_members_click(QModelIndex mi)
{
	int index = mi.row();
	current_user_index = index;

	//��ʾ�û�
	User u = users[current_user_index];
	ui.le_name->setText(qs8(u.name));
	if (u.is_male)
		ui.rb_male->setChecked(true);
	else ui.rb_female->setChecked(true);
	ui.de_birthday->setDate(QDate::fromString(qs8(u.birthday), qs8("yyyy-MM-dd")));
}

//��ӿͻ�
void Index_admin::btn_add_user_click()
{
	Edit_user_form* add = new Edit_user_form(this, NULL, 2, verb::post);
	connect(add, &Edit_user_form::edit_complete, this, &Index_admin::init_user_list);
	add->show();
}

//ɾ���ͻ�
void Index_admin::btn_remove_user_click()
{
	if (current_user_index >= 0 && current_user_index < users.size())
	{
		try
		{
			HttpConn* remove = new HttpConn(host, port);

			//�趨����
			remove->build(verb::delete_, "/client/" + users[current_user_index].username, 11);
			remove->request.set(field::cookie, cookie);

			//����
			remove->connect();

			//����JSON
			stringstream resp_ss;
			resp_ss << remove->response.body();
			//see(qs8(resp_ss.str()));

			ptree resp;
			read_json(resp_ss, resp);
			int code = resp.get<int>("code");
			if (code == 2301)
			{
				see(qs("ɾ���ɹ���"));
				init_user_list();
				close();
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
}

//�༭�ͻ�
void Index_admin::btn_edit_user_click()
{
	if (current_user_index >= 0 && current_user_index < users.size())
	{
		Edit_user_form* edit = new Edit_user_form(this, &users[current_user_index], 1, verb::put);
		connect(edit, &Edit_user_form::edit_complete, this, &Index_admin::init_user_list);
		edit->show();
	}
}

//��ʾ�˻��б�
void Index_admin::btn_account_list_click()
{
	if (current_user_index >= 0 && current_user_index < users.size())
	{
		Index_account* ia = new Index_account(this, users[current_user_index], 1);
		ia->show();
	}
}

//����ͻ���δ��ɣ�
void Index_admin::btn_freeze_user_click()
{

}

//�ⶳ�ͻ���δ��ɣ�
void Index_admin::btn_unfreeze_user_click()
{

}

//��С����ť
void Index_admin::btn_minimize_click()
{
	this->showMinimized();
}

//�رհ�ť
void Index_admin::btn_close_click()
{
	this->close();
	
	//��ʾ�����ڲ���ʱ�ö�
	parent->show();
	parent->raise();
}

//��ק����
void Index_admin::mousePressEvent(QMouseEvent* event)
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

void Index_admin::mouseMoveEvent(QMouseEvent* event)
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

void Index_admin::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}