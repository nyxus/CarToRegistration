#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/assign/list_inserter.hpp>
#include "intranetTopicHoursTool.h"

typedef enum http_methode{GET, POST};
enum { max_length = 1024 };
struct submit{
	std::string submitName, submitValue;

};
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class clientSSL
{
public:
	const std::string host;
	clientSSL(boost::asio::io_service& io_service, 
		boost::asio::ssl::context& context,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator, 
		std::string host);
	~clientSSL(){};
	
private:
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	char request_[max_length];
	boost::asio::streambuf response_;
	std::string request;

	bool verify_certificate(bool preverified,
		boost::asio::ssl::verify_context& ctx);
	void handle_connect(const boost::system::error_code& error);
	void handle_handshake(const boost::system::error_code& error);
	void handle_write_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_content(const boost::system::error_code& err);
	std::string HTTP_post_generator(std::string page, submit submit, std::map<std::string, std::string> data, std::map<std::string, std::string> extraHeaders);
	std::string HTTP_header_generator(http_methode methode, std::string page, std::map<std::string, std::string> extraHeaders = std::map<std::string, std::string>());
};

