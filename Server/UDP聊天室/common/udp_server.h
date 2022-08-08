/*************************************************************************
	> File Name: udp_server.h
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月28日 星期四 10时20分25秒
 ************************************************************************/

#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H
 
int socket_create_udp(int potr);
int udp_accept(int fd, struct User *user);

#endif
