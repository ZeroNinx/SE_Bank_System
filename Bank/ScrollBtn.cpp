#include "ScrollBtn.h"
#include "Bank.h"

//构造函数
ScrollBtn::ScrollBtn(QFrame* parent) :QPushButton(parent)
{
	//初始化
	resize(56, 27);
	setStyleSheet("QPushButton\n{\n	\n	background-color: rgb(55, 255, 55);\n	border:0px solid gray;\n	border-radius:13px;\n	outline:0px;\n}");
	setCursor(QCursor(Qt::PointingHandCursor));

	//给圆球安装事件过滤器
	circle = new QWidget(this);
	circle->installEventFilter(this);

	//设置动画定时器
	timer = new QTimer(this);
	timer->setInterval(75/15);
	auto tick = [&]()
	{
		if (is_admin)
			px += 2;
		else px -= 2;
		circle->update();
		if (px <= 3)
			setStyleSheet("QPushButton\n{\n	\n	background-color: rgb(255, 55, 55);\n	border:0px solid gray;\n	border-radius:13px;\n	outline:0px;\n}");
		if (px >= 31)
			setStyleSheet("QPushButton\n{\n	\n	background-color: rgb(55, 255, 55);\n	border:0px solid gray;\n	border-radius:13px;\n	outline:0px;\n}");
		if (px <= 3 || px >= 31)
			timer->stop();
	};
	connect(timer, &QTimer::timeout, tick);
}

//事件过滤器
bool ScrollBtn::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == circle)
	{
		//点击事件，改变状态
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (!timer->isActive())
			{
				is_admin = !is_admin;
				//发送信号
				emit move_over();
				timer->start();
			}
		}

		//绘图事件
		if (event->type() == QEvent::Paint)
		{
			QPainter painter(circle);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::NoPen);
			QBrush brush(Qt::SolidPattern);
			brush.setColor(Qt::white);
			//画圆
			painter.setBrush(brush);
			painter.drawEllipse(px, 3, 21, 21);
		}
	}
	return QPushButton::eventFilter(watched, event);
}

//设置管理员样式
void ScrollBtn::set_dev_style()
{
	circle->hide();
	setStyleSheet("QPushButton\n{\n	\n	background-color: rgb(85, 170, 255);\n	border:0px solid gray;\n	border-radius:13px;\n	outline:0px;\n}");
}
