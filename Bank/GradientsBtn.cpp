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

	//͸���ȱ仯��
	opacity_timer->setInterval(50);//50����ִ��һ��
	auto opacity_tick = [&]()
	{
		//�ж�͸���ȱ仯����
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
	//�ı�͸���ȱ仯����
	if (event->type() == QEvent::HoverEnter)
		opacity_up = false;
	if (event->type() == QEvent::HoverLeave)
		opacity_up = true;
	return QPushButton::eventFilter(watched, event);
}