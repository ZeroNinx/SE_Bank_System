#pragma once

//Winapi
#include <sdkddkver.h>

//stdc++
#include <algorithm>
#include <queue>
#include <sstream>
#include <vector>

//Boost
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/foreach.hpp>
#include <boost/locale.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Qt
#include <QBrush>
#include <QCheckBox>
#include <QCloseEvent>
#include <QEvent>
#include <QFile>
#include <QFontDatabase>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QMessageBox>
#include <QListView>
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
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

//自定义http连接类
#include "HttpConn.h"

#define see(x) QMessageBox::information(NULL, qs("提示"), x)
#define ffor(i,a,b) for(int i=a;i<=b;i++)
#define rfor(i,a,b) for(int i=a;i>=b;i--)

//快速转QString
QString qs(std::string text);
QString qs(char* text);
QString qs8(std::string text);
std::string to_utf8(std::string);
