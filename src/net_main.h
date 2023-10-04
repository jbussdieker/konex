void net_Init();
void net_Frame();
void net_Shutdown();
bool net_Connect(char *ipaddress);
void net_WaitForPlayer();
void net_StartServer();
void net_SendChat(void *buffer, int size);
