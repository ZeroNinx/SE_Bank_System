#include "stdafx.h"

//¿ìËÙ×ªQString
QString qs(std::string text)
{
	return QString::fromLocal8Bit(text.c_str());
}
QString qs(char* text)
{
	return QString::fromLocal8Bit(text);
}

QString qs8(std::string text)
{
	return QString::fromUtf8(text.c_str());
}
