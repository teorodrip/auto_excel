// ************************************************************************** //
//                                                                            //
//                                                                            //
//   auto_excel_server.hpp                                                    //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/02/05 15:44:53 by Mateo                                    //
//   Updated: 2019/02/05 16:28:50 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#ifndef AE_SERVER_HPP
#define AE_SERVER_HPP

#include "server.hpp"

class c_auto_excel_server : public c_server
{
public:
	void decode_data(const char *buff, const ssize_t readed);
};

#endif
