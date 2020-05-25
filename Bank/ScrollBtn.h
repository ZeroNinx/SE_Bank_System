#pragma once
#include "stdafx.h"

class ScrollBtn :public QPushButton
{
	Q_OBJECT

signals:
	//动画完成返回当前值
	void move_over();

public:
	ScrollBtn(QFrame* parent = 0);
	bool is_admin = true;
	void set_dev_style();

protected:
	QWidget* circle;
	QTimer* timer;
	int px = 31;
	bool ScrollBtn::eventFilter(QObject* watched, QEvent* event);
};

