#pragma once

#include "stdafx.h"
#include "ui_Index_account.h"
#include "Account.h"
#include "User.h"

class Index_account : public QWidget
{
	Q_OBJECT
public:
	Index_account(QWidget* p, User u,int opt);
	std::string host;
	std::string port;
	std::string cookie;

	int opt;
protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void init_user_list();
private:
	//����
	QWidget* parent;//������
	std::string username;//�û���

	//������ק����
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	//�����û�������
	User user;
	std::vector<Account> accounts;
	int current_account_index=9999;

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void lv_members_click(QModelIndex account);//�����б�

	void btn_add_account_click();//����˻�
	void btn_freeze_account_click();//�����˻�
	void btn_unfreeze_account_click();//�ⶳ�˻�
	void btn_remove_account_click();//ɾ���˻�

private:
	Ui::Index_account ui;
};
