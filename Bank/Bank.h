#pragma once

#include "stdafx.h"
#include "ui_Bank.h"
#include "ScrollBtn.h"
#include "GradientsBtn.h"
#include "Index.h"
#include "Index_user.h"
#include "Index_admin.h"
#include "Index_admin_user.h"

class Bank : public QFrame
{
	Q_OBJECT

public:
	Bank(QFrame *parent = Q_NULLPTR);

	//连接参数
	const std::string host = "39.105.182.121";
	const std::string port = "8001";
	std::string cookie;

	bool dev = false;//是否开发者
	bool is_admin = true;//默认登录状态

protected:
	//拖拽窗口函数
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	bool eventFilter(QObject* watched, QEvent* event);

private:

	std::string target = "/login/user";

	//按键梯队
	std::queue<int> key_queue;
	std::queue<int> password_queue;

	//拖拽窗口变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	//自定义类
	ScrollBtn* btn_select;//滑动选择按钮
	GradientsBtn* btn_login;//登录按钮
	
public slots:
	void btn_minimize_click();//最小化按钮
	void btn_close_click();//关闭按钮
	void btn_login_click();//登录按钮
	void btn_select_move_over();
	
private:
	Ui::main_window ui;
};
