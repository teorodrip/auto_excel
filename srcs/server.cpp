// ************************************************************************** //
//                                                                            //
//                                                                            //
//   server.cpp                                                               //
//                                                                            //
//   By: Mateo <teorodrip@protonmail.com>                                     //
//                                                                            //
//   Created: 2019/01/28 17:19:35 by Mateo                                    //
//   Updated: 2019/02/05 10:49:47 by Mateo                                    //
//                                                                            //
// ************************************************************************** //

#include "../includes/server.hpp"
#include "../includes/logger.hpp"

// init server default
c_server::c_server()
{
	server_data.sin_family = AF_INET;
	server_data.sin_addr.s_addr = htons(ADDR);
	server_data.sin_port = htons(PORT);
	server_data_len = sizeof(server_data);
	cli_head = NULL;
}

//init server with address and port
c_server::c_server(const uint16_t addr, const uint16_t port)
{
	server_data.sin_family = AF_INET;
	server_data.sin_addr.s_addr = htons(addr);
	server_data.sin_port = htons(port);
	server_data_len = sizeof(server_data);
	cli_head = NULL;
}

// init server manually
c_server::c_server(const struct sockaddr_in server_data)
{
	this->server_data = server_data;
	server_data_len = sizeof(server_data);
	cli_head = NULL;
}

// start the server
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

//end the server properly
void c_server::shut_down()
{
	client_t *tmp;
	while (cli_head)
		{
			tmp = cli_head;
			cli_head = cli_head->next;
			close(tmp->client_fd);
			delete tmp;
		}
	close(fd_server);
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

void c_server::assign_client(client_t *new_cli)
{
	if (!cli_head)
		cli_head = new_cli;
	else
		{
			new_cli->next = cli_head;
			cli_head = new_cli;
		}
}

void c_server::disconnect_client(client_t *prev, client_t *cli)
{
	if (cli == cli_head)
		cli_head = cli->next;
	else if (cli->next == NULL)
		prev->next = NULL;
	else
		prev->next = cli->next;
	close(cli->client_fd);
	delete cli;
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
			new_cli->client_fd = fd;
			new_cli->next = NULL;
			assign_client(new_cli);
			printf(F_LOG("New client connected (fd = %d)"), fd);
		}
	if (errno != EAGAIN && errno != EWOULDBLOCK)
		{
			fprintf(stderr, F_WARNING("Accepting a connection"));
			throw (EX_WARNING);
		}
}

void c_server::read_clients()
{
	client_t *cli;
	client_t *prev;
	char buff[BUFF_SIZE];
	ssize_t readed;

	prev = cli_head;
	cli = cli_head;

	while (cli)
		{
			while ((readed = read(cli->client_fd, buff, BUFF_SIZE)) > 0)
				{
					decode_data(buff, readed);
				}
			if (readed == 0)
				{
					printf(F_LOG("Client disconnected (fd = %d)"), cli->client_fd);
					disconnect_client(prev, cli);
					if (cli_head)
						cli = prev->next;
					else
						cli = NULL;
					continue;
				}
			prev = cli;
			cli = cli->next;
		}
}
