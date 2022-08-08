/*************************************************************************
	> File Name: udp_client.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月28日 星期四 17时04分21秒
 ************************************************************************/

#include "head.h"
extern int port;

int socket_udp() {
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0) return -1;	
	return sockfd;

}

//	创建一个通信的套接字为了 连接对端
int udp_connect(struct sockaddr_in *client) {
    int sockfd;
    sockfd = socket_create_udp(port);
    if(sockfd < 0) return -1;
    
	client->sin_port = htons(8888);
    if(connect(sockfd, (struct sockaddr *)client, sizeof(struct sockaddr)) < 0) return -1;

    return sockfd;
}
