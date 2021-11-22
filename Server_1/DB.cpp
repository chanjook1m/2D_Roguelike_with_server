#include "DB.h"

DB::DB(std::string addr, std::string id, std::string pw, std::string schema)
{
	driver = get_driver_instance();
	con = driver->connect(addr, id, pw);
	/* Connect to the MySQL database */
	con->setSchema(schema);

	stmt = con->createStatement();
}

DB::~DB()
{
	delete pstmt;
	delete stmt;
	delete con;
}

void DB::processTableQuery(std::string query)
{
	stmt->execute(query);
}

void DB::processRecordQueryWithString(std::string query, std::string str1, std::string str2)
{
	pstmt = con->prepareStatement(query);
	pstmt->setString(1, str1);
	pstmt->setString(2, str2);
	pstmt->execute();
}