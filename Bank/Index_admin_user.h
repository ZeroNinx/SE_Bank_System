#pragma once

#include "stdafx.h"
#include "ui_Index_admin_user.h"

class Index_admin_user : public QWidget
{
	Q_OBJECT
public:
	Index_admin_user(QWidget* p, std::string u);

protected:
	//��ק����
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	//����
	QWidget* parent;//������
	std::string username;//�û���


	//������ק����
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť

private:
	Ui::Index_admin_user ui;
};
