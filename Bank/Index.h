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
	//拖拽窗口
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void init_user_list();
private:
	//属性
	QWidget* parent;//父窗口
	std::string username;//用户名

	//窗口拖拽变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	//保存用户的数组
	std::vector<User> users;
	int current_index;
public slots:
	void btn_minimize_click();//最小化按钮
	void btn_close_click();//关闭按钮
	void lv_members_click(QModelIndex);//单击列表

	void btn_add_user_click();//添加客户
	void btn_remove_user_click();//删除客户
	void btn_edit_user_click();//编辑客户
	void btn_add_account_click();//添加账户
	void btn_freeze_account_click();//冻结账户
	void btn_unfreeze_account_click();//解冻账户
	void btn_remove_account_click();//删除账户

private:
	Ui::Index ui;
};
