#include "ScrollBtn.h"
#include "Bank.h"

//���캯��
ScrollBtn::ScrollBtn(QFrame* parent) :QPushButton(parent)
{
	//��ʼ��
	resize(56, 27);
	setStyleSheet("QPushButton\n{\n	\n	background-color: rgb(55, 255, 55);\n	border:0px solid gray;\n	border-radius:13px;\n	outline:0px;\n}");
	setCursor(QCursor(Qt::PointingHandCursor));

	//��Բ��װ�¼�������
	circle = new QWidget(this);
	circle->installEventFilter(this);

	//���ö�����ʱ��
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

//�¼�������
bool ScrollBtn::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == circle)
	{
		//����¼����ı�״̬
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (!timer->isActive())
			{
				is_admin = !is_admin;
				//�����ź�
				emit move_over();
				timer->start();
			}
		}

		//��ͼ�¼�
		if (event->type() == QEvent::Paint)
		{
			QPainter painter(circle);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::NoPen);
			QBrush brush(Qt::SolidPattern);
			brush.setColor(Qt::white);
			//��Բ
			painter.setBrush(brush);
			painter.drawEllipse(px, 3, 21, 21);
		}
	}
	return QPushButton::eventFilter(watched, event);
}

//���ù���Ա��ʽ
void ScrollBtn::set_dev_style()
{
	circle->hide();
	setStyleSheet("QPushButton\n{\n	\n	background-color: rgb(85, 170, 255);\n	border:0px solid gray;\n	border-radius:13px;\n	outline:0px;\n}");
}
