// AutoKmToRegistration.cpp : Defines the entry point for the console application.
//
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "stdafx.h"
#include <Windows.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Car.h"
#include "ClientSSL.h"
#include "Client.h"

using namespace std;
using namespace boost::posix_time;


int _tmain(int argc, _TCHAR* argv[])
{	
	/*
	string sfindFile = "LOGBOOK__*.txt";
	vector<wstring> logbookFiles;
	
	// find file with the  Windows libery
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	// get all drives
	// Get the bit mask of drive letters
	vector<wstring> existingDirs;
	DWORD drives = ::GetLogicalDrives();
	// Go through all possible letters from a to z
	for (int i = 0; i < 26; i++)
	{
		// Check if the respective bit is set
		if (drives & (1 << i))
		{
			existingDirs.push_back(wstring(1, static_cast<char>(_T('A') + i)));
		}
	}

	for (wstring dir : existingDirs){
		
		wstring findFile = dir + L":/" + wstring(sfindFile.begin(), sfindFile.end());
 
		hFind = FindFirstFile(findFile.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			wcout << "FindFirstFile failed: " <<  GetLastError() << " in " << dir << endl;
		}
		else
		{
			logbookFiles.push_back(dir + L":/" + wstring(FindFileData.cFileName));
			wcout << "The first file(" << FindFileData.cFileName << ") is found in " << dir << endl;
		}

		while (FindNextFile(hFind, &FindFileData) != 0){
			logbookFiles.push_back(dir + L":/" + wstring(FindFileData.cFileName));
			wcout << "The next file(" << FindFileData.cFileName << ") is found in " << dir << endl;
		}
		FindClose(hFind);
	}

	if (!logbookFiles.size()){
		cout << "No logsbooks found in any root" << endl << "Forgotten to insert the USB or to copy the logbook file to any root?" << endl;
		return 0;
	}

	Car opelAstra("Opel", "Astra", 0);
	for (auto fileName : logbookFiles){
		opelAstra.addLogbook(fileName);
	}
	*/
	
	/* HTTP request (basic witout SSL) 
	try
	{
		boost::asio::io_service io_service;
		client c(io_service, "intranet.topic.nl", "/profituren");
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
	*/
	/* HTTP(S) request with SSL  */
	/**/
	try
	{
			

		std::cout << "go get page!" << std::endl;
		boost::asio::io_service io_service;
		std::string host = "intranet.topic.nl";
		//std::string host = "www.example.com";
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(host, "443");
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
		context.set_default_verify_paths();
		clientSSL c(io_service, context, iterator, host);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n"; 
	}
	
	return 0; 
}