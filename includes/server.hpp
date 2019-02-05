// ************************************************************************** //
//                                                                            //
//                                                                            //
//   server.hpp                                                               //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/01/28 17:06:59 by Mateo                                    //
//   Updated: 2019/02/05 16:26:55 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <exception>
#include <errno.h>
#include <unistd.h>

#define PORT 8080
#define ADDR INADDR_ANY
#define MAX_CONNECTIONS 3
#define BUFF_SIZE 1024 //buffer must be 4 or more
#define META_INFO_LEN 3 // 1 byte for code 2 bytes for length
#define MAX_TICKER_LEN 256

typedef struct 	client_s
{
	int client_fd;
	struct client_s *next;
}								client_t;

class c_server
{
private:
	int fd_server;
	struct sockaddr_in server_data;
	socklen_t server_data_len;
	client_t *cli_head;

	void assign_client(client_t *new_cli);
	void disconnect_client(client_t *prev, client_t *client);

public:
	c_server();
	c_server(const uint16_t addr, const uint16_t port);
	c_server(const struct sockaddr_in server_data);
	int start();
	void shut_down();
	int set_connection_blocking(const int fd, const bool blocking);
	void accept_clients();
	void read_clients();
	virtual void decode_data(const char *buff, const ssize_t readed) = 0;
};
#endif
