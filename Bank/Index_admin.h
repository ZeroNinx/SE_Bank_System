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
	//拖拽窗口
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void init_adlin_list();

private:

	//窗口拖拽变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	//属性
	QWidget* parent;//父窗口
	std::string account;//用户名

	//管理员存放数组
	std::vector<Admin> admins;
	int current_index;

	//权限表映射
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
	void btn_minimize_click();//最小化按钮
	void btn_close_click();//关闭按钮
	void lv_members_click(QModelIndex i);//选定管理员
	void btn_add_admin_click();//增加管理员按钮
	void btn_edit_admin_click();
	void btn_remove_admin_click();//删除管理员按钮
	void create_admin_success();//管理员创建成功

private:
	Ui::Index_admin ui;
};
