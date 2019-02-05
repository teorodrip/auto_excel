// ************************************************************************** //
//                                                                            //
//                                                                            //
//   pgsql_db.cpp                                                             //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/01/29 13:28:03 by Mateo                                    //
//   Updated: 2019/01/29 14:43:24 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#include "../includes/pgsql_db.hpp"
#include "../includes/logger.hpp"

c_data_base::c_data_base()
{
	connection = NULL;
	db_name = DB_NAME;
	db_user = DB_USER;
	db_pass = DB_PASS;
	db_host = DB_HOST;
}

c_data_base::c_data_base(const std::string db_name,
												 const std::string db_user,
												 const std::string db_pass,
												 const std::string db_host)
{
	connection = NULL;
	this->db_name = db_name;
	this->db_user = db_user;
	this->db_pass = db_pass;
	this->db_host = db_host;
}

void c_data_base::connect()
{
	printf(F_LOG("Connecting to: %s (%s)"), db_name.c_str(), db_host.c_str());
	connection = PQsetdbLogin(db_host.c_str(),
											NULL, NULL, NULL,
											db_name.c_str(),
											db_user.c_str(),
											db_pass.c_str());
	switch(PQstatus(connection))
		{
		case CONNECTION_OK:
			printf(F_LOG("Connected to db"));
			break;
		case CONNECTION_BAD:
			fprintf(stderr, F_ERROR("Can not connect to db"));
			throw (EX_FATAL_ERROR);
			break;
		case CONNECTION_AWAITING_RESPONSE:
			printf(F_LOG("Connection status: CONNECTION_AWAITING_RESPONSE"));
			break;
		case CONNECTION_AUTH_OK:
			printf(F_LOG("Connection status: CONNECTION_AUTH_OK"));
			break;
		case CONNECTION_CHECK_WRITABLE:
			printf(F_LOG("Connection status: CONNECTION_CHECK_WRITABLE"));
			break;
		case CONNECTION_CONSUME:
			printf(F_LOG("Connection status: CONNECTION_CONSUME"));
			break;
		case CONNECTION_MADE:
			printf(F_LOG("Connection status: CONNECTION_MADE"));
			break;
		case CONNECTION_NEEDED:
			printf(F_LOG("Connection status: CONNECTION_NEEDED"));
			break;
		case CONNECTION_SETENV:
			printf(F_LOG("Connection status: CONNECTION_SETENV"));
			break;
		case CONNECTION_SSL_STARTUP:
			printf(F_LOG("Connection status: CONNECTION_SSL_STARTUP"));
			break;
		case CONNECTION_STARTED:
			printf(F_LOG("Connection status: CONNECTION_STARTED"));
			break;
		}
}


PGresult *c_data_base::exe_query(const std::string query, const ExecStatusType expected_res)
{
	PGresult *res;

	res = PQexec(connection, query.c_str());
	if (PQresultStatus(res) != expected_res)
		{
			fprintf(stderr, F_ERROR("Failed to execute the following query:\n%s"), query.c_str());
			PQclear(res);
			throw (EX_FATAL_ERROR);
		}
	return (res);
}

void c_data_base::disconnect()
{
	PQfinish(connection);
}
