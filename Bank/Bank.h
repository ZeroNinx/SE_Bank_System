#pragma once

#include "stdafx.h"
#include "ui_Bank.h"
#include "ScrollBtn.h"
#include "GradientsBtn.h"
#include "Index.h"
#include "Index_user.h"
#include "Index_admin.h"
#include "Index_admin_user.h"

class Bank : public QFrame
{
	Q_OBJECT

public:
	Bank(QFrame *parent = Q_NULLPTR);

	//���Ӳ���
	const std::string host = "39.105.182.121";
	const std::string port = "8001";
	std::string cookie;

	bool dev = false;//�Ƿ񿪷���
	bool is_admin = true;//Ĭ�ϵ�¼״̬

protected:
	//��ק���ں���
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	bool eventFilter(QObject* watched, QEvent* event);

private:

	std::string target = "/login/user";

	//�����ݶ�
	std::queue<int> key_queue;
	std::queue<int> password_queue;

	//��ק���ڱ���
	bool m_bDrag;
	QPoint mouseStartPoint;
	QPoint windowTopLeftPoint;

	//�Զ�����
	ScrollBtn* btn_select;//����ѡ��ť
	GradientsBtn* btn_login;//��¼��ť
	
public slots:
	void btn_minimize_click();//��С����ť
	void btn_close_click();//�رհ�ť
	void btn_login_click();//��¼��ť
	void btn_select_move_over();
	
private:
	Ui::main_window ui;
};
