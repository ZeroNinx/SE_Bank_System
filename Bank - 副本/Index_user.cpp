#include "Index_user.h"
#include "Bank.h"
#include "Edit_user_form.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::beast::http;

//���캯��
Index_user::Index_user(QWidget* p, User* u) :parent(p), user(u)
{
	ui.setupUi(this);

	//�趨����ͼ��
	setWindowIcon(QIcon("Resources/icon.png"));

	//�趨����͸��
	setAttribute(Qt::WA_TranslucentBackground, true);

	//���ر�����
	setWindowFlags(Qt::FramelessWindowHint);

	//ͼ�����
	ui.chart_view->setRenderHint(QPainter::Antialiasing);

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
	ui.chart_view->setGraphicsEffect(shadow[4]);

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
	ui.de_birthday->setDate(QDate::fromString(qs8(user->birthday), qs8("yyyy-MM-dd")));

	//ȡ�ò���
	Bank* pr = (Bank*)parent;
	host = pr->host;
	port = pr->port;
	cookie = pr->cookie;

	//��ʼ���û��б�
	init_account_list();

}

//��ʼ���˻��б�
void Index_user::init_account_list()
{
	accounts.clear();
	try
	{
		HttpConn* init = new HttpConn(host, port);
		init->build(verb::get,"/account/"+user->username,11);
		init->request.set(field::cookie, cookie);
		init->connect();
		
		//����json
		ptree resp;
		stringstream resp_ss;
		resp_ss << init->response.body();
		read_json(resp_ss, resp);

		int code = resp.get<int>("code");
		if (code != 3001)
		{
			string msg = resp.get<string>("msg");
			see(qs8(msg));
			return;
		}
		ptree data = resp.get_child("data");

		//����˻�
		QStringList sl;
		BOOST_FOREACH(ptree::value_type & v,data)
		{
			Account acc(v.second,user->username);
			sl << qs8(acc.id);
			accounts.push_back(acc);
		}
		QStringListModel* slm = new QStringListModel(sl);
		ui.lv_members->setModel(slm);
		ui.lv_members->setEditTriggers(QAbstractItemView::NoEditTriggers);

	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}

//�б�ѡ��
void Index_user::lv_members_click(QModelIndex mi)
{
	int index = mi.row();
	current_account_index = index;

	Account acc = accounts[index];
	char money[50];
	sprintf(money, "%.2f", acc.money);

	//����
	ui.le_name->setText(qs(acc.id));
	//����

	ui.lbl_money_value->setText(qs8(money));
	if (acc.freeze)
	{
		ui.lbl_state_value->setText(qs("�Ѷ���"));
		ui.lbl_state_value->setStyleSheet(qs("color:red;font:15px;"));
		ui.btn_trade->setEnabled(false);
	}
	else
	{
		ui.lbl_state_value->setText(qs("����"));
		ui.lbl_state_value->setStyleSheet(qs("color:green;font:15px;"));
		ui.btn_trade->setEnabled(true);
	}

	init_graphics();
}

//��ʼ������ͼ
void Index_user::init_graphics()
{
	try
	{
		//�����׽���
		HttpConn* history = new HttpConn(host, port);
		history->build(verb::get, "/moneyChanges/" + accounts[current_account_index].id, 11);
		history->request.set(field::cookie, cookie);
		history->connect();
		//see(qs(history->response.body()));

		//����json
		ptree resp;
		stringstream resp_ss;
		resp_ss << history->response.body();
		read_json(resp_ss, resp);
		int code = resp.get<int>("code");
		if (code == 5001)
		{
			double money = 0;
			double max_money = 0;
			int cnt = 0;

			//����ͼ��
			QChart* chart = new QChart();

			//�����㼯
			QLineSeries* ls = new QLineSeries();
			ls->setPen(QPen(Qt::blue, 1));
			ls->append(0, 0);

			vector<string> times;

			//��ӵ�
			ptree data = resp.get_child("data");
			BOOST_FOREACH(ptree::value_type & v, data)
			{
				double money_change = v.second.get<double>("money");
				if (accounts[current_account_index].id != v.second.get<string>("accountId"))
					money -= money_change;
				else money += money_change;

				times.push_back(v.second.get<string>("time"));

				max_money = max(money, max_money);
				ls->append(QPointF(++cnt, money));
			}

			//���õ㼯
			chart->addSeries(ls);

			//����������
			QValueAxis* axisX = new QValueAxis;
			axisX->setRange(0, cnt);
			axisX->setLabelsVisible(false);
			axisX->setGridLineVisible(true);
			axisX->setTickCount(cnt + 1);     //���ö��ٸ�

			//����������
			QValueAxis* axisY = new QValueAxis;
			axisY->setRange(0, max_money + 10);
			axisY->setLabelFormat("%.2f"); //���ÿ̶ȵĸ�ʽ
			axisY->setGridLineVisible(true);
			axisY->setTickCount(5);     //���ö��ٸ�

			//QChart������,�����ḽ�ӵ��㼯��
			chart->setAxisY(axisY, ls);
			chart->setAxisX(axisX, ls);

			//������ʽ
			chart->setAnimationOptions(QChart::SeriesAnimations);
			chart->setTheme(QChart::ChartThemeQt);
			chart->setMargins(QMargins(0, 10, 0, 0));

			//����ͼ��
			chart->legend()->hide();

			//��ʾ	
			ui.chart_view->setChart(chart);
		}
		else
		{
			string msg = resp.get<string>("msg");
			see(qs8(msg));
		}
	}
	catch (const std::exception&)
	{

	}
}

//�༭��ť
void Index_user::btn_edit_click()
{
	Edit_user_form* ed = new Edit_user_form(this,user, 0,verb::put);

	auto reload = [=]()
	{
		ui.le_name->setText(qs8(user->name));
		if (user->is_male)
			ui.rb_male->setChecked(true);
		else ui.rb_female->setChecked(true);
		ui.de_birthday->setDate(QDate::fromString(qs(user->birthday), qs("yyyy-MM-dd")));
	};

	connect(ed, &Edit_user_form::edit_complete, reload);
	ed->show();
}

//ת�˰�ť
void Index_user::btn_trade_click()
{
	if (current_account_index >= 0 && current_account_index < accounts.size())
	{
		Trade* td = new Trade(this, &accounts[current_account_index]);
		connect(td, &Trade::trade_success, this, &Index_user::init_account_list);
		td->show();
	}
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