/*************************************************************************
	> File Name: udp_server.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月12日 星期二 19时48分57秒
 ************************************************************************/
#include "head.h"

extern int subfd;

// 创建套接字返回套接字
int socket_create_udp(int port) {
    int listener;
    if((listener = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return -1;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	
	unsigned long opt = 1;
	// 端口复用
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	int ret = bind(listener, (struct sockaddr *)&server, sizeof(server));
	if(ret < 0) return -1; 

	return listener;
}

/*
	listener 会收到一个UDP数据包 (对端)
		打开之后包含：
			- 名字
			- 登录信息
		比对一下结构搜否和相信的一样
			- 报文 结构 一不一样
			- 大小长度 
			- 是否包含了我们所需要的信息
*/
	
int udp_accept(int fd, struct User *user) {
    int new_fd, ret;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    // 报文
    struct Msg request, response;
	bzero(&request, sizeof(request));
	bzero(&response, sizeof(response)); 

	// 接受数据
	ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);

	// 接受的数据的大小 和 recvfrom返回字节的个数 不同说明数据不全
	if(ret != sizeof(request)) {
		response.type = CHAT_FIN;
		strcpy(response.msg, "(请求数据有误)Login Request Error!");
		sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
		return -1;
	}

	/*
		// 判断是否在线 重复登录
		if(check_online(&request)) {
			response.type = CHAT_FIN;
			strcpy(response.msg, "(重复登录)Already login!");
			sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
			return -1;
		}
	*/

	if(request.type & CHAT_FIN) {
		return -1;
	}else if((request.type & CHAT_SYN) && (request.type & CHAT_ACK)) {
		DBG(GREEN "<Acceptor>" NONE " : SYNACK recived!\n");	
	}else if(request.type != CHAT_SYN) {
		response.type = CHAT_FIN;
		strcpy(response.msg, "(发过来的不是SYN)ERROR Request!");
		sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
		printf("request.msg = %s, request.from = %s type = %d\n", request.msg, request.from, request.type);
		return -1;
	}
	DBG(GREEN "<Acceptor>" NONE " : [%s] %s..\n", request.from, request.msg);

	strcpy(user->name, request.from);
	user->flag = FL_ONLINE;
	user->cnt = 5;
	new_fd = udp_connect(&client);
	if(new_fd < 0) {
		perror("udp_connect");
		return -1;
	}
	user->fd = new_fd;
	char buff[512] = {0};
	sprintf(buff, "%s (登录成功)Login success!\n", user->name);
	strcpy(response.msg, buff);
	response.type = CHAT_ACK | CHAT_SYN;
	// 发送确认
	send(new_fd, (void *)&response, sizeof(response), 0);
	
	return new_fd;
}

