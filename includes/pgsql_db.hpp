// ************************************************************************** //
//                                                                            //
//                                                                            //
//   pgsql_db.hpp                                                             //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/01/29 12:11:25 by Mateo                                    //
//   Updated: 2019/01/29 14:42:49 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#ifndef PG_HPP
#define PG_HPP

#include </usr/include/postgresql/libpq-fe.h>
#include <string>

#define DB_NAME "pam_test"
#define DB_USER "capiq_manager"
#define DB_PASS "capiqunchartech"
#define DB_HOST "192.168.27.122"
#define DB_TIMOUT "10" //Max

class c_data_base
{
private:
	PGconn *connection;
	std::string db_name;
	std::string db_user;
	std::string db_pass;
	std::string db_host;

public:
	c_data_base();
	c_data_base(const std::string db_name,
							const std::string db_user,
							const std::string db_pass,
							const std::string db_host);
	void connect();
	PGresult *exe_query(const std::string query, const ExecStatusType expected_res);
	void disconnect();
};

#endif
