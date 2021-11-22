#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>

#include <mysql/jdbc.h>
#include <jdbc/cppconn/driver.h>

#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

class DB
{
private:
	sql::Driver* driver = NULL;
	sql::Connection* con = NULL;
	sql::Statement* stmt = NULL;
	sql::PreparedStatement* pstmt = NULL;
	sql::ResultSet* result = NULL;

public:


public:
	DB(std::string addr, std::string id, std::string pw, std::string schema);
	virtual ~DB();

	void processTableQuery(std::string query);
	void processRecordQueryWithString(std::string query, std::string str1, std::string str2);


};

