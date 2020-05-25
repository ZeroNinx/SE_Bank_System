#pragma once

#include "stdafx.h"
#include "ui_Index_user.h"
#include "User.h"
#include "Trade.h"

class Index_user : public QWidget
{
	Q_OBJECT
public:
	Index_user(QWidget* p, User* u);

protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	//����
	QWidget* parent;//������
	User* user;

	Trade* trade_form = new Trade();

	//������ק����
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void btn_trade_click();//ת�˰�ť

private:
	Ui::Index_user ui;
};
