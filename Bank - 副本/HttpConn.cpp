#include "HttpConn.h"
using namespace std;
using namespace boost::beast::http;

//���캯��
HttpConn::HttpConn(std::string h, std::string p) :host(h), port(p)
{
	try
	{
		//��������
		auto const results = resolver.resolve(host, port);
		// �����ҵ�������
		boost::asio::connect(socket, results.begin(), results.end());
	}
	catch (exception e)
	{
		see(qs(e.what()));
	}
}

//��������
void HttpConn::build(boost::beast::http::verb method, std::string target, int version)
{
	try
	{
		request = boost::beast::http::request<string_body>{ method,target,version };
		request.set(field::host, host);
	}
	catch (exception e)
	{
		see(qs(e.what()));
	}
}

//����
void HttpConn::connect()
{
	try
	{
		write(socket, request);
		read(socket, buf, response);
	}
	catch (const std::exception& e)
	{
		see(qs(e.what()));
	}
}