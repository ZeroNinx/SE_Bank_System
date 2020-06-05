#include "Bank.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//��ʼ�������캯��
Bank::Bank(QFrame* parent) : QFrame(parent)
{
	ui.setupUi(this);
	
	//�趨����ͼ��
	setWindowIcon(QIcon("Resources/icon.png"));

	//�趨����͸��
	setAttribute(Qt::WA_TranslucentBackground, true);

	//���ر�����
	setWindowFlags(Qt::FramelessWindowHint);

	//����ѡ��ť
	btn_select = new ScrollBtn(ui.frame_window);
	btn_select->setGeometry(205, 322, 56, 27);
	btn_select->raise();
	connect(btn_select, &ScrollBtn::move_over, this, &Bank::btn_select_move_over);

	//�����¼��ť
	btn_login = new GradientsBtn(ui.frame_window,"��¼");
	btn_login->setGeometry(30, 360, 406, 41);
	btn_login->raise();
	connect(btn_login, &GradientsBtn::clicked, this, &Bank::btn_login_click);

	//����Ա�������

	//ͨ������
	password_queue.push(Qt::Key_Up);
	password_queue.push(Qt::Key_Up);
	password_queue.push(Qt::Key_Down);
	password_queue.push(Qt::Key_Down);
	password_queue.push(Qt::Key_Left);
	password_queue.push(Qt::Key_Right);
	password_queue.push(Qt::Key_Left);
	password_queue.push(Qt::Key_Right);
	password_queue.push(Qt::Key_B);
	password_queue.push(Qt::Key_A);

	ui.lbl_img_index->installEventFilter(this);

	//���ñ߿���Ӱ
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(6);
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
		ui.lbl_username->setFont(font);
		ui.lbl_password->setFont(font);
	}

	QFile fontfile2(qs("Resources/Բ��/medium.TTF"));
	if (fontfile2.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile2.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		ui.lbl_user->setFont(font);
		ui.lbl_admin->setFont(font);
	}

	QFile fontfile3(qs("Resources/Բ��/heavy.TTF"));
	if (fontfile3.open(QFile::ReadOnly))
	{
		//���룬��������
		int id = QFontDatabase::addApplicationFontFromData(fontfile3.readAll());
		QFont font(QFontDatabase::applicationFontFamilies(id).at(0), 15, 10);
		btn_login->setFont(font);
	}

	//��������
	ui.le_username->clear();
	ui.le_password->clear();
}


//��С����ť
void Bank::btn_minimize_click()
{
	this->showMinimized();
}

//�رհ�ť
void Bank::btn_close_click()
{
	this->close();
}

//��¼״̬ѡ����
void Bank::btn_select_move_over()
{
	is_admin = !is_admin;
}

//��¼��ť
void Bank::btn_login_click()
{
	if (dev)
		target = "/login/dev";
	else
	{
		//������
		if (ui.le_username->text().isEmpty() || ui.le_password->text().isEmpty())
		{
			see(qs("������������"));
			return;
		}
		if (is_admin)
			target = "/login/user";
		else target = "/login/client";
	}

	try
	{
		//��������
		HttpConn login(host, port);
		login.build(verb::post, target, 11);

		//��¼����
		if (!dev)
		{
			ptree login_json;
			stringstream login_ss;
			login_json.put("account", ui.le_username->text().toStdString());
			login_json.put("password", ui.le_password->text().toStdString());
			write_json(login_ss, login_json);

			login.request.set(field::content_length, login_ss.str().length());
			login.request.body() = login_ss.str();
		}

		//Request
		//�趨����
		login.request.set(field::content_type, "application/json");

		//ͨ���׽��ַ���request
		login.connect();

		//Response
		response<string_body> resp = login.response;

		//for (auto i = resp.begin(); i != resp.end(); i++)
		//	see(qs(i->value().to_string()));

		//������ص�����
		//see(resp.body().c_str());

		//��������ֵ
		stringstream resp_ss;
		ptree resp_json;
		resp_ss << resp.body();
		read_json(resp_ss, resp_json);

		//Ѱ��״̬��
		int code;
		if (resp_json.find("code") == resp_json.not_found())
		{
			see(qs("���Ӵ���"));
			return;
		}
		code = resp_json.get<int>("code");

		//����״̬��
		if (code != 1001)
		{
			string msg = resp_json.get<string>("msg");
			see(qs8(msg));
			return;
		}
		else
		{
			//ȡ��cookie
			cookie = resp[field::set_cookie].to_string().substr(0, 43);
			//see(qs(cookie));

			if (dev)//������
			{
				if (is_admin)
				{
					Index_admin* index = new Index_admin(this, "");
					index->show();
					this->hide();
				}
				else
				{
					Index_admin_user* index = new Index_admin_user(this, "");
					index->show();
					this->hide();
				}
			}
			else
			{
				ptree data = resp_json.get_child("data");

				//����Ա
				if (is_admin)
				{
					//��ȡ����
					string username = data.get<string>("username");
					

					//��½�ɹ���������һ������
					Index* index = new Index(this, username);
					index->show();
					this->hide();
				}
				else//�ͻ�
				{
					//��ʼ���ͻ�
					User* user = new User(data);

					//��½�ɹ���������һ������
					Index_user* index = new Index_user(this, user);
					index->show();
					this->hide();
				}
			}
		}
	}
	catch(exception e)
	{
		see(e.what());
	}
}

//�¼�������
bool Bank::eventFilter(QObject* watched, QEvent* event)
{
	//����Ա�ܵ�
	if (watched == ui.lbl_img_index)
	{	
		//���񰴼�����
		if (event->type() == QEvent::KeyRelease)
		{
			QKeyEvent* key_event = static_cast<QKeyEvent*>(event);

			//��ʽ����������
			while(key_queue.size()>=10)
				key_queue.pop();
			key_queue.push(key_event->key());
			if (key_queue.size() == 10)
			{
				bool correct = true;
				
				//�Ƚ�����
				ffor(cnt, 1, 10)
				{
					int current_key = key_queue.front();
					int password_key = password_queue.front();
					key_queue.pop();
					password_queue.pop();
					if (current_key != password_key)
						correct = false;
					key_queue.push(current_key);
					password_queue.push(password_key);
				}
				if (correct)//��ȷ������������ģʽ
				{
					see(qs("��ϲ����������ģʽ��"));
					ui.lbl_user->setText(qs("�ͻ�����"));
					ui.lbl_admin->setText(qs("����Ա����"));
					ui.le_username->setText(qs("������"));
					ui.le_password->setEchoMode(QLineEdit::Normal);
					ui.le_password->setText(qs("���¤�һ"));
					dev = true;
				}
			}

			//see(qs(to_string(key_event->key())));
		}
	}


	return QWidget::eventFilter(watched, event);
}

//��ק����(��д����¼�)
void Bank::mousePressEvent(QMouseEvent* event)
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

void Bank::mouseMoveEvent(QMouseEvent* event)
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

void Bank::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}