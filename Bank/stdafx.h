#pragma once

//Winapi
#include <sdkddkver.h>

//stdc++
#include <queue>
#include <sstream>
#include <vector>

//Boost
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Qt
#include <QBrush>
#include <QCloseEvent>
#include <QEvent>
#include <QFile>
#include <QFontDatabase>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <QObject>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

//�Զ���http������
#include "HttpConn.h"

#define see(x) QMessageBox::information(NULL, qs("��ʾ"), x)
#define ffor(i,a,b) for(int i=a;i<=b;i++)
#define rfor(i,a,b) for(int i=a;i>=b;i--)

//����תQString
QString qs(std::string text);
QString qs(char* text);
QString qs8(std::string text);
