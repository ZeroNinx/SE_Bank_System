#pragma once
#include "stdafx.h"

class HttpConn
{
public:
	HttpConn(std::string host, std::string port);

	boost::beast::http::request< boost::beast::http::string_body> request;
	boost::beast::flat_buffer buf;
	boost::beast::http::response<boost::beast::http::string_body> response;

	//函数
	void build(boost::beast::http::verb method, std::string target, int version);
	void connect();

private:
	//套接字
	boost::asio::io_service io_se;
	boost::asio::ip::tcp::resolver resolver{ io_se };
	boost::asio::ip::tcp::socket socket{ io_se };

	//连接
	std::string host = "";
	std::string port = "";
	std::string target = "";
};

