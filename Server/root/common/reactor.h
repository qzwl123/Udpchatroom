/*************************************************************************
	> File Name: reactor.h
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月29日 星期五 14时03分44秒
 ************************************************************************/

#ifndef _REACTOR_H
#define _REACTOR_H

int add_to_reactor(int subfd, struct User *user);
void *reactor(void *arg);
void  del_from_reactor(int fd);


#endif
