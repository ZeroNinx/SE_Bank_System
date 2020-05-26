#pragma once

#include "stdafx.h"
#include "ui_Create_admin_form.h"

class Create_admin_form : public QWidget
{
	Q_OBJECT

public:
	Create_admin_form(QWidget *p = Q_NULLPTR);

protected:
	//拖拽窗口
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	QWidget* parent;

	//窗口拖拽变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

public slots:
	void btn_close_click();
	void btn_submit_click();

private:
	Ui::Create_admin_form ui;
};
