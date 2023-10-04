/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker   *
 *   jbussdieker@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "defs.h"
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <sys/poll.h>
#include <errno.h>

#define NET_PORT 5050
#define MAX_CLIENTS 10
#define NET_TIMEOUT 11

#define NET_STATUS_IDLE 0
#define NET_STATUS_SERVER 1
#define NET_STATUS_CLIENT 2

//int netStatus = NET_IDLE;

struct net_chat_t
{
	int size;
	char buffer[128];
	bool pending;
};

struct net_status_t
{
	bool server;
	bool connecting;
	bool connected;
	float contectttl;
	int updatepos;
} netstatus;

struct net_connection_t
{
	int socket;
	char address[32];
	bool used;
	int updatepos;
	dword playerid;
	float updatettl;
};

net_connection_t connections[MAX_CLIENTS];
net_chat_t chatbuffer;
int mainsocket = -1;

dword getFreeConnection()
{
	dword i;
	for (i = 1; i < MAX_CLIENTS; i++)
		if (connections[i].used == false)
			return i;

	return 0;
}

pollfd pollSocket(int socket)
{
	pollfd pfd;

	pfd.events = POLLIN | POLLOUT | POLLPRI;
	pfd.revents = 0;
	pfd.fd = socket;
	
	if (poll(&pfd, 1, 1) == -1)
	{
		console_Print("Poll Error");
	}

	return pfd;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
int readSocket(int socket, void* buffer, int buffersize)
{
	int recvsize;

	errno = 0;
	recvsize = recv(socket, &buffer, buffersize, MSG_PEEK);

	if (recvsize == -1)
	{
		if (errno != EAGAIN)
			console_Print("NET: readSocket():recv failed.");

		recvsize = 0;
	}

	return recvsize;
}

int createSocket()
{
	int newsocket;
	newsocket = socket(AF_INET, SOCK_STREAM, 0);

	if (newsocket == -1)
	{
		console_Print("NET: createSocket():socket failed.");
		return -1;
	}
	else
	{
		if (fcntl(newsocket, F_SETFL, O_NONBLOCK) == -1)
		{
			console_Print("NET: createSocket():fcntl failed.");
			return -1;
		}

		console_Print("NET: createSocket(): Succeeded.");
		return newsocket;
	}
}

int connectSocket(int socket, char *server, dword port)
{
	char ipaddress[128];
	struct hostent *h;
	sockaddr_in dest_addr;

	h = gethostbyname(server);
	if (h == NULL)
	{
		console_Print("NET: connectSocket():gethostbyname failed.");
		return -1;
	}

	strcpy(ipaddress, inet_ntoa(*((in_addr *)h->h_addr)));
	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(ipaddress);

	bzero(&dest_addr.sin_zero, 8);
	
	errno = 0;
	if (connect(socket, (sockaddr *)&dest_addr, sizeof(sockaddr)) == -1)
	{
		if (errno != EINPROGRESS)
		{
			console_Print("NET: connectSocket():connect failed.");
			return -1;
		}
	}

	console_Notify("NET: connectSocket(): Connecting to %s...", ipaddress);
	return 1;
}

int bindSocket(int socket, dword port)
{
	sockaddr_in bind_addr;
	
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(port);
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(bind_addr.sin_zero), 8);

	errno = 0;

	if (bind(socket, (struct sockaddr *)&bind_addr, sizeof(struct sockaddr)) == -1)
	{
		switch (errno)
		{
			case EBADF:
				console_Print("NET: bindSocket():bind bad file.");
				break;
			case EINVAL:
				console_Print("NET: bindSocket():bind bad invalid.");
				break;
			case EACCES:
				console_Print("NET: bindSocket():bind bad access.");
				break;
			default:
				console_Print("NET: bindSocket():bind failed.");
				break;
		}

		return -1;
	}
	
	console_Print("NET: bindSocket(): Succeeded.");
	return 1;
}

int listenSocket(int socket)
{
	if (listen(socket, MAX_CLIENTS) == -1)
	{
		console_Print("NET: listenSocket():listen failed.");
		return -1;
	}

	console_Print("NET: listenSocket(): Succeeded.");
	return 1;
}

int acceptSocket(int socket)
{
	int newsocket;

	sockaddr_in their_addr;
	socklen_t sin_size = sizeof(sockaddr_in);

	errno = 0;
	newsocket = accept(socket, (struct sockaddr *)&their_addr, &sin_size);
	if (newsocket == -1)
	{
		if (errno == EWOULDBLOCK)
			return 0;

		console_Print("%m");
		return 0;
	}

	return newsocket;
}

int closeSocket(int socket)
{
	if (close(socket) == -1)
	{
			console_Print("NET: closeSocket():close failed.");
			return -1;
	}

	console_Print("NET: closeSocket(): Succeeded.");
	return 1;
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

bool sendChat(int socket)
{
	//return true;
	if (chatbuffer.pending == true)
	{
		unsigned char size = chatbuffer.size;
		char buffer[128];
		sprintf(buffer, "%c%s", size, chatbuffer.buffer);
		
		if (send(socket, buffer, size + 1, MSG_DONTWAIT) != -1)
		{
			chatbuffer.pending = true;
			return true;
		}
	}
	else
	{
		unsigned char a = 0;
		if (send(socket, &a, 1, MSG_DONTWAIT) != -1)
			return true;
	}

	return false;
}

bool readChat(int socket)
{
	//return true;
	int recvsize;
	char buffer[5000];
	memset(&buffer, 0, sizeof(buffer));

	recvsize = readSocket(socket, &buffer, sizeof(buffer));

	if (recvsize > 0)
	{
		int size = buffer[0];
		if (recvsize > size + 1)
		{
			recv(socket, &buffer, size + 1, 0);

			//connections[i].address

			//if (size > 0)
				//console_Print("NET: (%s): %s", "slash", buffer+1);

			return true;
		}
	}

	return false;
}

void clientDisconnect()
{
	netstatus.connected = false;
	netstatus.connecting = false;
	netstatus.contectttl = NET_TIMEOUT;
	netstatus.server = false;
	netstatus.updatepos = -1;

	if (mainsocket > -1)
		closeSocket(mainsocket);
}

void clientConnectFrame()
{
	pollfd pfd;
	pfd = pollSocket(mainsocket);
	
	if ((pfd.revents & POLLNVAL) == POLLNVAL)
	{
		console_Print("NET: POLL Not Valid");
		netstatus.connecting = false;
		return;
	}
	if ((pfd.revents & POLLHUP) == POLLHUP)
	{
		console_Print("NET: POLL Hang Up");
		netstatus.connecting = false;
		return;
	}
	if ((pfd.revents & POLLERR) == POLLERR)
	{
		console_Print("NET: POLL Error");
		netstatus.connecting = false;
		return;
	}

	if ((pfd.revents & POLLOUT) == POLLOUT)
	{
		console_Notify("Connected...");
		netstatus.connecting = false;
		netstatus.connected = true;
		netstatus.updatepos = -1;
		game_CreateEmpty();
	}
	else
	{
		netstatus.contectttl -= tm_FrameTime;
		if (netstatus.contectttl < 0.0f)
		{
			console_Notify("Timed out connecting to server.");
			clientDisconnect();
		}
	}
}

void clientFrame()
{
	// Check socket state
	pollfd pfd;
	pfd = pollSocket(mainsocket);

	if ((pfd.revents & POLLERR) == POLLERR)
	{
		console_Print("NET: Connection closed.");
		clientDisconnect();
	}

	if ((pfd.revents & POLLOUT) == 0 && (pfd.revents & POLLIN) == 0)
		return;


	int recvsize;
	char buffer[5000];
	memset(&buffer, 0, sizeof(buffer));
	recvsize = readSocket(mainsocket, &buffer, sizeof(buffer));

	//console_Print("NET: %d Recieved %d bytes", netstatus.updatepos, recvsize);

	if (netstatus.updatepos == -1)
	{
		if (recvsize < sizeof(gameMainPlayer))
			return;

		recv(mainsocket, &gameMainPlayer, sizeof(gameMainPlayer), 0);
		viewPlayer = gameMainPlayer;
		netstatus.updatepos++;
		console_Print("NET: Entered game as %d", gameMainPlayer);
	}
	else if (netstatus.updatepos == 0)
	{
//		if (recvsize < sizeof(entities))
//			return;

//		recv(mainsocket, &entities, sizeof(entities), 0);
		netstatus.updatepos++;
	}
	else if (netstatus.updatepos == 1)
	{
//		if (send(mainsocket, &entities[gameMainPlayer], sizeof(entities_entity_t), MSG_DONTWAIT) != -1)
			netstatus.updatepos++;
	}
	/*else if (netstatus.updatepos == 4)
	{
		if (sendChat(mainsocket) == true)
			netstatus.updatepos++;
	}
	else if (netstatus.updatepos == 5)
	{
		if (readChat(mainsocket) == true)
			netstatus.updatepos = 0;
	}*/
}

void serverFrame()
{
	// Accept new connections
	int newconnection = acceptSocket(mainsocket);
		
	if (newconnection > 0)
	{
		dword tmpid = getFreeConnection();

		connections[tmpid].socket = newconnection;
		connections[tmpid].used = true;
		connections[tmpid].updatepos = -1;

		sockaddr_in their_addr;
		socklen_t sin_size = sizeof(sockaddr_in);
		getsockname(newconnection, (sockaddr *)&their_addr, &sin_size);

		strcpy(connections[tmpid].address, inet_ntoa(their_addr.sin_addr));

		//connections[tmpid].playerid = player_CreateMultiplayer();
//		connections[tmpid].playerid = gameMainPlayer;

		console_Print("NET: (%s) Connection recieved.", connections[tmpid].address);
	}

	// Read socket data
	dword i;
	for (i = 1; i < MAX_CLIENTS; i++)
	{
		if (connections[i].used == true)
		{
			// Check socket state
			pollfd pfd;
			pfd = pollSocket(connections[i].socket);
	
			if ((pfd.revents & POLLERR) == POLLERR)
			{
				console_Print("NET: (%s) Connection closed.", connections[i].address);
				connections[i].used = false;
				connections[i].updatepos = -1;
//				entities[connections[i].playerid].used = false;
				continue;
			}

			if ((pfd.revents & POLLOUT) == 0 && (pfd.revents & POLLIN) == 0)
				continue;

			if (connections[i].updatepos == -1)
			{
				if (send(connections[i].socket, &connections[i].playerid, sizeof(connections[i].playerid), MSG_DONTWAIT) != -1)
					connections[i].updatepos++;
			}
			else if (connections[i].updatepos == 0)
			{
//				if (send(connections[i].socket, &entities, sizeof(entities), MSG_DONTWAIT) != -1)
					connections[i].updatepos++;
			}
			else if (connections[i].updatepos == 1)
			{
				int recvsize;
				char buffer[5000];
				memset(&buffer, 0, sizeof(buffer));

//				recvsize = readSocket(connections[i].socket, &buffer, sizeof(buffer));
				//console_Print("NET: %d Recieved %d bytes", connections[i].updatepos, recvsize);

//				if (recvsize >= sizeof(entities_entity_t))
				{
//					recv(connections[i].socket, &entities[connections[i].playerid], sizeof(entities_entity_t), 0);
					connections[i].updatepos++;
				}
			}
			/*else if (connections[i].updatepos == 4)
			{
				if (readChat(connections[i].socket) == true)
					connections[i].updatepos++;
			}
			else if (connections[i].updatepos == 5)
			{
				if (sendChat(connections[i].socket) == true)
					connections[i].updatepos = 0;
			}*/
		}
	}
}

void net_SendChat(void *buffer, int size)
{
	if (chatbuffer.pending == true)
		console_Print("NET: Pending chat overwritten");

	memcpy(chatbuffer.buffer, buffer, size);
	chatbuffer.size = strlen(chatbuffer.buffer);
	chatbuffer.pending = true;
}

void net_StartServer()
{
	clientDisconnect();

	mainsocket = createSocket();
	bindSocket(mainsocket, NET_PORT);
	listenSocket(mainsocket);

	netstatus.server = true;
	console_Print("NET: Server started, listening for connections");
}

#define MAXBUFLEN 255

void net_Init()
{
	netstatus.server = false;
	netstatus.connected = false;
	netstatus.connecting = false;

	console_Print("NET: Network system operational...");
}

bool net_Connect(char *server)
{
	clientDisconnect();

	mainsocket = createSocket();
	connectSocket(mainsocket, server, NET_PORT);

	netstatus.connecting = true;
	netstatus.updatepos = -1;
	netstatus.contectttl = NET_TIMEOUT;
}

void net_Frame()
{
	if (netstatus.server == true)
	{
		serverFrame();
	}
	else if (netstatus.connecting == true)
	{
		clientConnectFrame();
	}
	else if (netstatus.connected == true)
	{
		clientFrame();
	}
}

void net_Shutdown()
{
	closeSocket(mainsocket);

	dword i;

	for (i = 1; i < MAX_CLIENTS; i++)
		if (connections[i].used == true)
		{
			closeSocket(connections[i].socket);
			connections[i].used = false;
		}
}
