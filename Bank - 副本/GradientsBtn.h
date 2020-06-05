#pragma once
#include "stdafx.h"

class GradientsBtn : public QPushButton
{
	Q_OBJECT
public:
	GradientsBtn(QWidget* parent,std::string text);

private:
	bool GradientsBtn::eventFilter(QObject* watched, QEvent* event);

	//自定义透明度动画函数
	QGraphicsOpacityEffect* opacity_effect = new QGraphicsOpacityEffect();
	QTimer* opacity_timer = new QTimer();
	double opacity = 1.0;
	bool opacity_up = true;
};

