#include "stdafx.h"

using namespace std;

//快速转QString
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

//转成utf-8
string to_utf8(std::string s)
{
	return boost::locale::conv::between(s, "UTF-8", "GB2312");
}