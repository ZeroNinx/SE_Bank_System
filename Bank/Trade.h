#pragma once

#include "stdafx.h"
#include "ui_Trade.h"
#include "Account.h"

class Trade : public QWidget
{
	Q_OBJECT

public:
	Trade(QWidget* p,Account* a);

	std::string host;
	std::string port;
	std::string cookie;

protected:
	//拖拽窗口
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	
	//窗口拖拽变量
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;
	Account* account;

signals:
	void trade_success();

public slots:
	void btn_close_click();
	void btn_submit_click();

private:
	Ui::Trade ui;
};
