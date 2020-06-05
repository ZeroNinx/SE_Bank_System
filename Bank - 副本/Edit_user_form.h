#pragma once

#include "stdafx.h"
#include "ui_Edit_user_form.h"
#include "User.h"

class Edit_user_form : public QWidget
{
	Q_OBJECT

public:
	Edit_user_form(QWidget *p,User* u,int o,boost::beast::http::verb m);

	QWidget* parent;//������

	int opt;
	std::string host;
	std::string port;
	std::string cookie;
	boost::beast::http::verb method;//���ӷ���

	User* user;

protected:
	//��ק���ں���
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	//��ק���ڱ���
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
