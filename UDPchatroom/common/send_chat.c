/*************************************************************************
	> File Name: send_chat.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年08月01日 星期一 15时28分05秒
 ************************************************************************/

#include "head.h"
#include <stdio.h>

extern char name[20];
extern struct User *users;
extern struct ChatMsg chatmsg;

extern WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;
extern int msg_num;

void *send_chat(void *arg) {
	struct Msg msgxx;
	// DBG("msgxx.type = CHAT_MSG : %d; \n", msgxx.type);
	while(1) {
		memset(&msgxx, 0, sizeof(msgxx));
		strcpy(msgxx.from, name);
		msgxx.type = CHAT_MSG; 
		// printf("Please Input:\n");
		scanf("%s", msgxx.msg);
		// wgetstr(sub_input_win, msgxx.msg);
		for(int i = 0; i < MAXUSER; i++) {
			if(users[i].flag) {
				strcpy(msgxx.from, name);
				msgxx.type = CHAT_MSG; 
				send(users[i].fd, &msgxx, sizeof(msgxx), 0);
			}
		}
	}
	return NULL;
}
