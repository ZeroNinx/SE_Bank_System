#include "Index_account.h"
#include "Index.h"
#include "Index_admin.h"
#include "Edit_user_form.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//���캯��s
Index_account::Index_account(QWidget* p,User u,int opt) :parent(p), user(u)
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
	ffor(i, 0, 5)
	{
		shadow.push_back(new QGraphicsDropShadowEffect);
		shadow[i]->setBlurRadius(7);
		shadow[i]->setColor(Qt::black);
		shadow[i]->setOffset(0, 0);
	}
	ui.lv_members->setGraphicsEffect(shadow[0]);
	ui.le_search->setGraphicsEffect(shadow[1]);
	ui.btn_add_account->setGraphicsEffect(shadow[2]);
	ui.btn_freeze_account->setGraphicsEffect(shadow[3]);
	ui.btn_unfreeze_account->setGraphicsEffect(shadow[4]);
	ui.btn_remove_account->setGraphicsEffect(shadow[5]);

	QGraphicsDropShadowEffect* shadow_div = new QGraphicsDropShadowEffect;
	shadow_div->setBlurRadius(3);
	shadow_div->setColor(Qt::black);
	shadow_div->setOffset(0, 0);

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
		ui.lbl_money->setFont(font);
		ui.lbl_state->setFont(font);

		ui.btn_freeze_account->setFont(font);
		ui.btn_unfreeze_account->setFont(font);
		ui.btn_add_account->setFont(font);
		ui.btn_remove_account->setFont(font);
	}


	//ȡ�ò���
	if (opt == 1)
	{
		Index* pr = (Index*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}
	else if (opt == 2)
	{
		Index_admin* pr = (Index_admin*)parent;
		host = pr->host;
		port = pr->port;
		cookie = pr->cookie;
	}

	//��ʼ���û��б�
	init_user_list();
}

//��ʼ���û��б�
void Index_account::init_user_list()
{
	accounts.clear();
	try
	{
		//��ȡ�˻��б�
		HttpConn* get_account = new HttpConn(host, port);
		get_account->build(verb::get, "/account/" + user.username, 11);
		get_account->request.set(field::cookie, cookie);
		get_account->connect();
		//see(qs8(get_account->response.body()));

		//����JSON
		ptree resp_json;
		stringstream resp_ss;
		resp_ss << get_account->response.body();
		read_json(resp_ss, resp_json);
		int code_a = resp_json.get<int>("code");
		if (code_a == 3001)
		{
			ptree data_a = resp_json.get_child("data");
			QStringList sl;
			BOOST_FOREACH(ptree::value_type & v, data_a)
			{
				Account a(v.second, user.username);
				accounts.push_back(a);
				sl << qs8(a.id);
			}
			QStringListModel* slm = new QStringListModel(sl);
			ui.lv_members->setModel(slm);
			ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);
		}
		else
		{
			string msg = resp_json.get<string>("msg");
			see(qs8(msg));
		}

	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}

//�б�ѡ��
void Index_account::lv_members_click(QModelIndex mi)
{
	int index = mi.row();
	current_account_index = index;

	//��ʾ�˻�
	Account acc = accounts[index];
	char money[50];
	sprintf(money, "%.2f", acc.money);
	ui.lbl_money_value->setText(qs8(money));
	if (acc.freeze)
	{
		ui.lbl_state_value->setText(qs("�Ѷ���"));
		ui.lbl_state_value->setStyleSheet(qs("color:red;font:15px;"));
	}
	else
	{
		ui.lbl_state_value->setText(qs("����"));
		ui.lbl_state_value->setStyleSheet(qs("color:green;font:15px;"));
	}
}

//����˻�
void Index_account::btn_add_account_click()
{
	try
	{
		HttpConn* add = new HttpConn(host, port);

		ptree req;
		stringstream ss;
		req.put("clientAccount", user.username);
		write_json(ss, req);

		//�趨����
		add->build(verb::post, "/account", 11);
		add->request.set(field::cookie, cookie);
		add->request.set(field::content_type, "application/json");
		add->request.set(field::content_length, ss.str().length());
		add->request.body() = ss.str();

		//����
		add->connect();

		//����JSON
		stringstream resp_ss;
		resp_ss << add->response.body();
		//see(qs8(resp_ss.str()));

		ptree resp;
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 3201)
		{
			see(qs("�����ɹ���"));
			init_user_list();
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

//�����˻�
void Index_account::btn_freeze_account_click()
{
	if (current_account_index >= 0 && current_account_index < accounts.size())
	{
		try
		{
			HttpConn* remove = new HttpConn(host, port);
			remove->build(verb::delete_, "/account/"+accounts[current_account_index].id, 11);

			//�趨����
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
			if (code == 3301)
			{
				see(qs("ɾ���ɹ���"));
				init_user_list();
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

//�ⶳ�˻�
void Index_account::btn_unfreeze_account_click()
{

}

//ɾ���˻�
void Index_account::btn_remove_account_click()
{
	if (current_account_index >= 0 && current_account_index < accounts.size())
	{
		try
		{
			HttpConn* remove = new HttpConn(host, port);
			remove->build(verb::delete_, "/account/"+accounts[current_account_index].id, 11);

			//�趨����
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
			if (code == 3301)
			{
				see(qs("ɾ���ɹ���"));
				init_user_list();
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


//��С����ť
void Index_account::btn_minimize_click()
{
	this->showMinimized();
}

//�رհ�ť
void Index_account::btn_close_click()
{
	this->close();

	//��ʾ�����ڲ���ʱ�ö�
	parent->show();
	parent->raise();
}

//��ק����
void Index_account::mousePressEvent(QMouseEvent* event)
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

void Index_account::mouseMoveEvent(QMouseEvent* event)
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

void Index_account::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}