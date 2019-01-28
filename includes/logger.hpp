// ************************************************************************** //
//                                                                            //
//                                                                            //
//   logger.hpp                                                               //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/01/28 17:30:55 by Mateo                                    //
//   Updated: 2019/01/28 18:37:34 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdio.h>
#include <stdlib.h>

#define ERROR_HEADER "Error: "
#define ENDL "\n"
#define C_CLEAR "\e[0m"
#define C_ERROR "\e[38;5;196m"
#define C_SUCCESS "\e[38;5;82m"
#define C_LOG "\e[38;5;111m"
#define U_LOG "\u0c66"
#define U_ERROR "\u0c6b"
#define U_SUCCESS "\u16b5"
#define F_ERROR(str) (C_ERROR U_ERROR " " ERROR_HEADER str C_CLEAR ENDL)
#define F_SUCCESS(str) (C_SUCCESS U_SUCCESS " " str C_CLEAR ENDL)
#define F_LOG(str) (C_LOG U_LOG " " str C_CLEAR ENDL)

#define EX_FATAL_ERROR 0x1

#endif
