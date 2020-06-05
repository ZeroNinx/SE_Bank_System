#include "Index_admin.h"
#include "Bank.h"
#include "Edit_admin_form.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

Index_admin::Index_admin(QWidget* p, string a) :parent(p), account(a)
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
	ui.btn_add_admin->setGraphicsEffect(shadow[2]);
	ui.btn_edit_admin->setGraphicsEffect(shadow[3]);
	ui.btn_remove_admin->setGraphicsEffect(shadow[4]);


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
		ui.lbl_state->setFont(font);
		ui.lbl_account->setFont(font);
		ui.lbl_username->setFont(font);
		ui.cb_1->setFont(font);
		ui.cb_2->setFont(font);
		ui.cb_3->setFont(font);
		ui.cb_4->setFont(font);
		ui.cb_5->setFont(font);
		ui.cb_6->setFont(font);
		ui.cb_7->setFont(font);
		ui.cb_8->setFont(font);
		ui.btn_add_admin->setFont(font);
		ui.btn_edit_admin->setFont(font);
		ui.btn_remove_admin->setFont(font);
		ui.lv_members->setFont(font);
	}

	//��ȡ�ϼ�����
	Bank* pr = (Bank*)p;
	cookie = pr->cookie;
	host = pr->host;
	port = pr->port;

	//�趨�û���ֻ��
	ui.le_account->setReadOnly(true);

	//��ʼ��Ȩ���б�
	perms[0] = ui.cb_1;
	perms[1] = ui.cb_2;
	perms[2] = ui.cb_3;
	perms[3] = ui.cb_4;
	perms[4] = ui.cb_5;
	perms[5] = ui.cb_6;
	perms[6] = ui.cb_7;
	perms[7] = ui.cb_8;


	//��ʼ������Ա�б�
	init_adlin_list();
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

//��ʼ������Ա�б�
void Index_admin::init_adlin_list()
{
	admins.clear();
	//��ȡ����
	try
	{
		Bank* p = (Bank*)parent;
		HttpConn* init = new HttpConn(p->host, p->port);
		init->build(verb::get, "/users", 11);
		init->request.set(field::cookie, p->cookie);
		init->connect();
		//see(qs(init->response.body()));

		//����JSON
		ptree root;
		stringstream ss;
		ss << init->response.body();
		read_json(ss, root);

		//see(qs(ss.str()));

		int code = root.get<int>("code");
		if (code != 4001)
		{
			string msg = root.get<string>("msg");
			see(qs8(msg));
			return;
		}

		//��ʼ���û��б�
		QStringList sl;

		//��������Ա
		ptree data = root.get_child("data");
		BOOST_FOREACH(ptree::value_type & i, data)
		{
			//��ʼ������Ա�б�
			Admin admin = Admin(i.second);
			admins.push_back(admin);
			sl << qs8(admin.username);
		}

		//Ӧ���û��б�
		QStringListModel* slm = new QStringListModel(sl);
		ui.lv_members->setModel(slm);
		ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);

		lv_members_click(ui.lv_members->model()->index(0,0));
	}
	catch (const std::exception& e)
	{
		see(e.what());
	}
}

//��Ա�б�ѡ��
void Index_admin::lv_members_click(QModelIndex mi)
{
	//��ȡ�±�
	int index = mi.row();
	current_index = index;
	//see(qs(to_string(index)));
	
	//��ȡ�û���
	ui.le_account->setText(qs8(admins[index].account));
	ui.le_username->setText(qs8(admins[index].username));

	//��ȡȨ��
	ffor(i, 0, 7)
		perms[i]->setChecked(admins[index].perms[i]);
}

//���ӹ���Ա��ť
void Index_admin::btn_add_admin_click()
{
	Edit_admin_form* form = new Edit_admin_form(this);
	connect(form, &Edit_admin_form::create_complete, this, &Index_admin::create_admin_success);
	form->show();
}

//�༭����Ա��ť
void Index_admin::btn_edit_admin_click()
{
	try
	{
		//��������
		Bank* p = (Bank*)parent;
		HttpConn* create = new HttpConn(p->host, p->port);

		//����JSON
		ptree root;
		stringstream ss;

		root.put("account", ui.le_account->text().toStdString());

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

		//see(qs(ss.str()));

		//�趨����
		create->build(verb::put, "/user", 11);
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
		if (code == 4101)
		{
			see(qs("�޸ĳɹ���"));
			init_adlin_list();
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

//ɾ������Ա��ť
void Index_admin::btn_remove_admin_click()
{
	try
	{
		Bank* p = (Bank*)parent;

		HttpConn* del = new HttpConn(p->host, p->port);
		del->build(verb::delete_, "/user/" + ui.le_account->text().toStdString(), 11);
		del->request.set(field::cookie, cookie);
		del->connect();

		ptree resp;
		stringstream resp_ss;
		resp_ss << del->response.body();
		read_json(resp_ss, resp);
		see(qs8(resp_ss.str()));

		int code = resp.get<int>("code");
		if (code == 4301)
		{
			see(qs("ɾ���ɹ���"));
			init_adlin_list();
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

//����Ա�����ɹ�
void Index_admin::create_admin_success()
{
	init_adlin_list();
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