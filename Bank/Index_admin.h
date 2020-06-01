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

	std::string host;
	std::string port;
	std::string cookie;

protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void init_adlin_list();

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
	int current_index;

	//Ȩ�ޱ�ӳ��
	QCheckBox* perms[8];
	std::string perm_list[8] =
	{
		"cl_ge",
		"cl_up",
		"cl_ad",
		"cl_del",
		"ac_ad",
		"ac_ge",
		"ac_fre",
		"ac_del"
	};

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void lv_members_click(QModelIndex i);//ѡ������Ա
	void btn_add_admin_click();//���ӹ���Ա��ť
	void btn_edit_admin_click();
	void btn_remove_admin_click();//ɾ������Ա��ť
	void create_admin_success();//����Ա�����ɹ�

private:
	Ui::Index_admin ui;
};
