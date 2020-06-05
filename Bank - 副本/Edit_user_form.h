#pragma once

#include "stdafx.h"
#include "ui_Edit_user_form.h"
#include "User.h"

class Edit_user_form : public QWidget
{
	Q_OBJECT

public:
	Edit_user_form(QWidget *p,User* u,int o,boost::beast::http::verb m);

	QWidget* parent;//父窗口

	int opt;
	std::string host;
	std::string port;
	std::string cookie;
	boost::beast::http::verb method;//连接方法

	User* user;

protected:
	//拖拽窗口函数
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	//拖拽窗口变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

signals:
	void edit_complete();

public slots:
	void btn_submit_click();
	void btn_close_click();
private:
	Ui::Edit_user_form ui;
};
