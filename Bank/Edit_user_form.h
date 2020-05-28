#pragma once

#include "stdafx.h"
#include "ui_Edit_user_form.h"

class Edit_user_form : public QWidget
{
	Q_OBJECT

public:
	Edit_user_form(QWidget *p,int o);

	QWidget* parent;//¸¸´°¿Ú

	int opt;
	std::string host;
	std::string port;
	std::string cookie;

signals:
	void create_success();

private:
	Ui::Edit_user_form ui;
};
