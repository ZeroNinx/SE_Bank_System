#pragma once

#include "stdafx.h"
#include "ui_Index.h"
#include "User.h"

class Index : public QWidget
{
	Q_OBJECT
public:
	Index(QWidget* p, std::string u);
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

	std::vector<User> users;

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť

	void lv_members_click(QModelIndex);//�����б�


private:
	Ui::Index ui;
};
