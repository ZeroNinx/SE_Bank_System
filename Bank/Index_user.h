#pragma once

#include "stdafx.h"
#include "ui_Index_user.h"
#include "User.h"
#include "Trade.h"
#include "Account.h"

class Index_user : public QWidget
{
	Q_OBJECT
public:
	Index_user(QWidget* p, User* u);

	std::string host;
	std::string port;
	std::string cookie;

protected:
	//拖拽窗口
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void init_account_list();

private:
	//属性
	QWidget* parent;//父窗口
	User* user;

	Trade* trade_form = new Trade();

	//窗口拖拽变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	std::vector<Account> accounts;

public slots:
	void btn_minimize_click();//最小化按钮
	void btn_close_click();//关闭按钮
	void btn_edit_click();//编辑按钮
	void btn_trade_click();//转账按钮
	void lv_members_click(QModelIndex);//列表选中

private:
	Ui::Index_user ui;
};
