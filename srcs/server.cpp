// ************************************************************************** //
//                                                                            //
//                                                                            //
//   server.cpp                                                               //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/01/28 17:19:35 by Mateo                                    //
//   Updated: 2019/01/28 18:57:59 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#include "../includes/server.hpp"
#include "../includes/logger.hpp"

c_server::c_server()
{
	server_data.sin_family = AF_INET;
	server_data.sin_addr.s_addr = htons(ADDR);
	server_data.sin_port = htons(PORT);
	server_data_len = sizeof(server_data);
}

c_server::c_server(const uint16_t addr, const uint16_t port)
{
	server_data.sin_family = AF_INET;
	server_data.sin_addr.s_addr = htons(addr);
	server_data.sin_port = htons(port);
	server_data_len = sizeof(server_data);
}

c_server::c_server(const struct sockaddr_in server_data)
{
	this->server_data = server_data;
	server_data_len = sizeof(server_data);
}

int c_server::start()
{
	int enable = 1;

	//create the socket
	if((fd_server = socket(server_data.sin_family, SOCK_STREAM, 0)) < 0)
		{
			fprintf(stderr, F_ERROR("Creating the socket"));
			throw (EX_FATAL_ERROR);
		}
	//to close the socket when program is finished
	if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		{
			fprintf(stderr, F_ERROR("Setting socket option"));
			throw (EX_FATAL_ERROR);
		}
	//bind the socket
	if ((bind(fd_server, (struct sockaddr *)&server_data, server_data_len)) < 0)
		{
			fprintf(stderr, F_ERROR("Binding the socket"));
			throw (EX_FATAL_ERROR);
		}
	//start listen connections
	if((listen(fd_server, MAX_CONNECTIONS)) < 0)
		{
			fprintf(stderr, F_ERROR("Making the socket listen"));
			throw (EX_FATAL_ERROR);
		}
	printf(F_LOG("Server is up for connections"));
	return(fd_server);
}

int c_server::set_connection_blocking(const int fd, const bool blocking)
{
	int flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
		{
			fprintf(stderr, F_ERROR("Getting fd flags"));
			throw (EX_FATAL_ERROR);
		}
	if (blocking)
		flags &= ~(O_NONBLOCK);
	else
		flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		{
			fprintf(stderr, F_ERROR("Setting the connection non blocking"));
			throw (EX_FATAL_ERROR);
		}
	return (fd);
}

void c_server::accept_clients()
{
	int fd;
	client_t *new_cli;

		while ((fd = accept(fd_server, (struct sockaddr *)&server_data,
												(socklen_t *)&server_data_len)) >= 0)
		{
			new_cli = new client_t;
			set_connection_blocking(fd, false);
			assign_client(fd, new_cli, cli_head);//continue here
			*cli_head = new_cli;
			printf("A client has made a connection\n");
		}
	if (errno != EAGAIN && errno != EWOULDBLOCK)
		{
			printf("Error: in accept connection\n");
			exit(EXIT_FAILURE);
		}

}
