#pragma once

#include "stdafx.h"
#include "ui_Index_admin.h"
#include "Admin.h"
#include "Trade.h"

class Index_admin : public QWidget
{
	Q_OBJECT
public:
	Index_admin(QWidget* p, std::string u);

protected:
	//拖拽窗口
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	//属性
	QWidget* parent;//父窗口
	std::string username;//用户名

	//窗口拖拽变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	std::vector<Admin> admins;

public slots:
	void btn_minimize_click();//最小化按钮
	void btn_close_click();//关闭按钮

private:
	Ui::Index_admin ui;
};
