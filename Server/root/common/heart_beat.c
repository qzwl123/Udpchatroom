/*************************************************************************
	> File Name: heart_beat.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年08月01日 星期一 10时11分42秒
 ************************************************************************/
#include "head.h"

extern struct User *users;

// 心跳 	
void * heart_beat(void *arg) {
	struct Msg heart;
	heart.type = CHAT_HEART;
	while (1) {
		for(int i = 0; i < MAXUSER; i++) {
			if(users[i].flag) {
				if(users[i].cnt <= 0) {
					DBG("😈" BLUE "<%s>" NONE "is removed by heart beat...!\n", users[i].name);
					users[i].flag = 0;
					del_from_reactor(users[i].fd);
					DBG("🛰" RED "%s" NONE " : 超时 is removed from sub reactor...!\n", users[i].name);
					close(users[i].fd);
				}
				send(users[i].fd, (void *)&heart, sizeof(heart), 0);
				users[i].cnt--; 
			}
		}
		sleep(30);
	}
	

	return NULL;
}
