#pragma once

#include "stdafx.h"
#include "ui_Edit_admin_form.h"

class Edit_admin_form : public QWidget
{
	Q_OBJECT

public:
	Edit_admin_form(QWidget *p = Q_NULLPTR);

protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	QWidget* parent;

	//������ק����
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	QCheckBox* perms[9];
	std::string perm_list[9] =
	{
		"cl_ge",
		"cl_up",
		"cl_ad",
		"cl_del",
		"ac_ad",
		"ac_ge",
		"ac_fre",
		"ac_unf",
		"ac_del"
	};

public slots:
	void btn_close_click();
	void btn_submit_click();

signals:
	void create_complete();//��������ź�

private:
	Ui::Edit_admin_form ui;
};
