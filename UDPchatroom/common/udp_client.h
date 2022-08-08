/*************************************************************************
	> File Name: udp_client.h
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月29日 星期五 15时03分48秒
 ************************************************************************/

#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

int socket_udp();
int udp_connect(struct sockaddr_in *client); 

#endif
