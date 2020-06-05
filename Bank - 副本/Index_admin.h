#pragma once

#include "stdafx.h"
#include "ui_Index_admin.h"
#include "Account.h"
#include "User.h"

class Index_admin : public QWidget
{
	Q_OBJECT
public:
	Index_admin(QWidget* p, std::string u);
	std::string host;
	std::string port;
	std::string cookie;

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
	std::vector<User> users;
	int current_user_index = 9999;

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void lv_members_click(QModelIndex);//�����б�

	void btn_add_user_click();//��ӿͻ�
	void btn_remove_user_click();//ɾ���ͻ�
	void btn_edit_user_click();//�༭�ͻ�
	void btn_account_list_click();//��ʾ�˻��б�
	void btn_freeze_user_click();//����ͻ�
	void btn_unfreeze_user_click();//�ⶳ�ͻ�

private:
	Ui::Index_admin ui;
};
