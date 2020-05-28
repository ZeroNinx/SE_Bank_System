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

	std::vector<User> users;

public slots:
	void btn_minimize_click();//最小化按钮
	void btn_close_click();//关闭按钮

	void lv_members_click(QModelIndex);//单击列表


private:
	Ui::Index ui;
};
