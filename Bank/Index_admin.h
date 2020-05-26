#pragma once

#include "stdafx.h"
#include "ui_Index_admin.h"
#include "Admin.h"
#include "Trade.h"

class Index_admin : public QWidget
{
	Q_OBJECT
public:
	Index_admin(QWidget* p, std::string a);
	std::string cookie;

protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:

	//������ק����
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	//����
	QWidget* parent;//������
	std::string account;//�û���

	//����Ա�������
	std::vector<Admin> admins;

	//Ȩ�ޱ�ӳ��
	QCheckBox* perms[8];

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void lv_members_click(QModelIndex i);//ѡ������Ա
	void btn_add_admin_click();//���ӹ���Ա��ť

private:
	Ui::Index_admin ui;
};
