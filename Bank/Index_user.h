#pragma once

#include "stdafx.h"
#include "User.h"
#include "Trade.h"
#include "Account.h"

using namespace QtCharts;
#include "ui_Index_user.h"

class Index_user : public QWidget
{
	Q_OBJECT
public:
	Index_user(QWidget* p, User* u);

	std::string host;
	std::string port;
	std::string cookie;

protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	
private:
	//����
	QWidget* parent;//������
	User* user;

	//������ק����
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	std::vector<Account> accounts;
	int current_account_index=9999;

public slots:
	void init_account_list();//��ʼ���˻��б�
	void init_graphics();//��ʼ������ͼ

	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void btn_edit_click();//�༭��ť
	void btn_trade_click();//ת�˰�ť
	void lv_members_click(QModelIndex);//�б�ѡ��



private:
	Ui::Index_user ui;
};
