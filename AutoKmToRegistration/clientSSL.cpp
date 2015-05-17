#include "stdafx.h"
#include "clientSSL.h"

using boost::asio::ip::tcp;

clientSSL::clientSSL(boost::asio::io_service& io_service, boost::asio::ssl::context& context,
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator, std::string host)
	: socket_(io_service, context),
	host(host)
{	
	socket_.set_verify_mode(boost::asio::ssl::verify_peer);
	socket_.set_verify_callback(
		boost::bind(&clientSSL::verify_certificate, this, _1, _2));

	boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
		boost::bind(&clientSSL::handle_connect, this,
		boost::asio::placeholders::error));
}

bool clientSSL::verify_certificate(bool preverified,
	boost::asio::ssl::verify_context& ctx)
{
	// The verify callback can be used to check whether the certificate that is
	// being presented is valid for the peer. For example, RFC 2818 describes
	// the steps involved in doing this for HTTPS. Consult the OpenSSL
	// documentation for more details. Note that the callback is called once
	// for each certificate in the certificate chain, starting from the root
	// certificate authority.

	// In this example we will simply print the certificate's subject name.
	char subject_name[256]; 
	
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

	
	std::cout << "Verifying " << subject_name << "\n";
	// todo: check the certificate, because this application is used interaly we assume the certificate is save
	//return preverified;
	return true;
}

void clientSSL::handle_connect(const boost::system::error_code& error)
{
	if (!error)
	{
		socket_.async_handshake(boost::asio::ssl::stream_base::client,
			boost::bind(&clientSSL::handle_handshake, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Connect failed: " << error.message() << "\n";
	}
}

void clientSSL::handle_handshake(const boost::system::error_code& error)
{
	if (!error)
	{
		std::string  contentData = "SUBMIT=Ga+naar&year=2015&month=5&project=000001%2B2015";
		request = "POST /profituren/index.php HTTP/1.1\r\n";
		request += "Host: intranet.topic.nl\r\n";
		request += "Authorization: Basic Z2VyY28udmVyc2xvb3RAdG9waWMubmw6bTN0aDBkMyEhJHRvcGlj\r\n"; // base64 (username:password)
		request += "Accept: */*\r\n";
		request += "Accept-Language: en-us,en;q=0.5\r\n";
		request += "Referer: https://intranet.topic.nl/profituren/index.php \r\n";
		request += "Content-Type: application/x-www-form-urlencoded \r\n";
		request += "Content-Length: " + std::to_string(contentData.size()) + "\r\n";
		request += "Connection: close\r\n\r\n";
		request += contentData + "\r\n";
		std::map<std::string, std::string> addtitionHeaders;
		addtitionHeaders["Authorization"] = "Basic Z2VyY28udmVyc2xvb3RAdG9waWMubmw6bTN0aDBkMyEhJHRvcGlj";
		//request = clientSSL::HTTP_header_generator(http_methode(GET), "/profituren/index.php", addtitionHeaders);
		std::map<std::string, std::string> data = { 
			{ "year", "2015" }, 
			//{ "projectID", "000001" }, 
			//{"phaseID", "2015"},
			//{"u_zkm_u_d6", "310"},
			{"month", "5"},
			{"project", "000001%2B2015"}
		};
		/* get Urenstaat 
		boost::assign::insert(data)
			("year", "2015")
			("month", "5")
			("project", "000001%2B2015");*/
		request = clientSSL::HTTP_post_generator("/profituren/index.php", submit{ "SUBMIT", "Ga+naar" }, data, addtitionHeaders);
		
		//request = clientSSL::HTTP_post_generator("/profituren/index.php", submit{ "SUBMIT", "Totalen berekenen + opslaan" }, data, addtitionHeaders);

		std::cout << request;
		/*
		std::cin.getline(request_, max_length);
		size_t request_length = strlen(request_);
		*/
		boost::asio::async_write(socket_,boost::asio::buffer(request),
			boost::bind(&clientSSL::handle_write_request, this,
				boost::asio::placeholders::error
			)
		);
	}
	else
	{
		std::cout << "Handshake failed: " << error.message() << "\n";
	}
}

void clientSSL::handle_write_request(const boost::system::error_code& err)
{
	if (!err)
	{
		// Read the response status line.
		boost::asio::async_read_until(socket_, response_, "\r\n",
			boost::bind(&clientSSL::handle_read_status_line, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void clientSSL::handle_read_status_line(const boost::system::error_code& err)
{
	if (!err)
	{
		// Check that response is OK.
		std::istream response_stream(&response_);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code ";
			std::cout << status_code << "\n";
			return;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
			boost::bind(&clientSSL::handle_read_headers, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

void clientSSL::handle_read_headers(const boost::system::error_code& err)
{
	if (!err)
	{
		// Process the response headers.
		std::istream response_stream(&response_);
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		if (response_.size() > 0)
			std::cout << &response_;

		// Start reading remaining data until EOF.
		boost::asio::async_read(socket_, response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&clientSSL::handle_read_content, this,
			boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

void clientSSL::handle_read_content(const boost::system::error_code& err)
{
	if (!err)
	{
		// Write all of the data that has been read so far.
		

		// Continue reading remaining data until EOF.
		boost::asio::async_read(socket_, response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&clientSSL::handle_read_content, this,
			boost::asio::placeholders::error));
	}
	else if (err != boost::asio::error::eof)
	{
		std::ostringstream ss;
		ss << &response_;
		std::string site = ss.str();

		std::ofstream outfile("site.html", std::ofstream::binary);
		if (!outfile.is_open()) {
			std::cerr << "Couldn't open 'output.txt'" << std::endl;
			return;
		}

		outfile << site << std::endl;
		outfile.close();

		intranetTopicHoursTool page(site);

		//std::cout << s;
	}
}

std::string clientSSL::HTTP_post_generator(std::string page, submit submit, std::map<std::string, std::string> data, std::map<std::string, std::string> extraHeaders){
	std::string postData = submit.submitName + "=" + submit.submitValue;
	for each (auto dataItem in data)
	{
		postData += "&" + dataItem.first + "=" + dataItem.second;
	}
	postData += " \r\n";

	extraHeaders["Content-Length"] = std::to_string(postData.size());
	extraHeaders["Content-Type"] = "application/x-www-form-urlencoded";

	return HTTP_header_generator(http_methode(POST), page, extraHeaders) + postData;
}

std::string clientSSL::HTTP_header_generator(http_methode methode, std::string page, std::map<std::string, std::string> extraHeaders){
	
	std::string methode_s, HTTPrequest;
	switch (methode)
	{
	case GET:
		methode_s = "GET";
		break;
	case POST:
		methode_s = "POST";
		break;
	default:
		throw;
		break;
	}
	std::map<std::string, std::string> headers = { 
		{ "Host", host },
		{ "Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" },
		{ "Accept-Encoding", "identity" },
		{ "Accept-Language", "en-us,en;q=0.5" },
		{ "Connection", "close" },
		{ "Origin", "https://" + host },
		{ "Referer", "https://" + host + page } 
	};
	
	for (auto header : extraHeaders){
		headers[header.first] = header.second;
	}
	HTTPrequest = methode_s + " " + page + " HTTP/1.1 \r\n";
	for each (auto header in headers)
	{
		HTTPrequest += header.first + ": " + header.second + " \r\n";
	}
	return HTTPrequest + "\r\n";
}