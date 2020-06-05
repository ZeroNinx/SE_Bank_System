#include "GradientsBtn.h"

using namespace std;
using namespace QtCharts;

GradientsBtn::GradientsBtn(QWidget* parent,string text) :QPushButton(parent)
{
	installEventFilter(this);
	resize(406, 41);
	setStyleSheet("QPushButton\n{\n	font:17px;\n	color:white;\n	background-color:rgba(0, 170, 255,0.8);\n	border-radius:18px;\n		outline:0px;\n}");
	setCursor(QCursor(Qt::PointingHandCursor));
	setText(qs(text));

	//透明度变化组
	opacity_timer->setInterval(50);//50毫秒执行一次
	auto opacity_tick = [&]()
	{
		//判断透明度变化过程
		if (opacity_up && opacity < 0.99)
		{
			opacity_effect->setOpacity(opacity);
			setGraphicsEffect(opacity_effect);
			opacity += 0.05;
		}
		else if (!opacity_up && opacity > 0.84)
		{
			opacity -= 0.05;
			opacity_effect->setOpacity(opacity);
			setGraphicsEffect(opacity_effect);
		}
	};
	connect(opacity_timer, &QTimer::timeout, opacity_tick);
	opacity_timer->start();
}

bool GradientsBtn::eventFilter(QObject* watched, QEvent* event)
{
	//改变透明度变化方向
	if (event->type() == QEvent::HoverEnter)
		opacity_up = false;
	if (event->type() == QEvent::HoverLeave)
		opacity_up = true;
	return QPushButton::eventFilter(watched, event);
}