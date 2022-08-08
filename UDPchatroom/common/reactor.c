/*************************************************************************
	> File Name: reactor.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月29日 星期五 13时53分27秒
 ************************************************************************/

#include "head.h"

extern struct User *users; 
extern int subfd; 

extern WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;
extern int msg_num;

//	subfd(从) epoll_event 添加用户事件
void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
	struct epoll_event ev;
	ev.events = events;
	ev.data.ptr = (void *)user;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
	return ;
}

int add_to_reactor(int fd, struct User *user) {
	int sub = user->fd;
	users[sub] = *user;

	add_event_ptr(fd, users[sub].fd, EPOLLIN | EPOLLET, &users[sub]);

	return 1;
}

void del_from_reactor(int fd) {
	epoll_ctl(subfd, EPOLL_CTL_DEL, fd, NULL);
	return ;
}

void *reactor(void *arg) {
	struct epoll_event events[20];
	struct Msg msg;
	while (1) {
		int nfds = epoll_wait(subfd, events, 20, -1);
		if(nfds < 0) {
			perror("subfd epoll_wait!");
			exit(1);
		} 
		for(int i = 0; i < nfds; i++) {
			int fd = ((struct User *)(events[i].data.ptr))->fd;
			recv(fd, &msg, sizeof(msg), 0);
			users[fd].cnt = 5;

			if(msg.type & CHAT_HEART) {
				DBG(BLUE "<%s> " NONE " 💓 -> " BLUE "<Me>\n" NONE, users[fd].name);
				msg.type = CHAT_ACK;
				send(fd, &msg, sizeof(msg), 0);
			} else if(msg.type & CHAT_ACK) {
				DBG(BLUE"<Me>" NONE "💕 -> <%s>\n", users[fd].name);
			} else if(msg.type & CHAT_FIN) {
				DBG("😡" BLUE "<%s> 🤞 \n", msg.from);
				// epoll_ctl delete from sub reactor
				del_from_reactor(fd);
				close(fd);
				users[fd].flag = 0;
				DBG("😡" BLUE "<%s> " NONE "is leaving!\n", users[fd].name);
				DBG("🛰" RED "<%s> is removed from sub reactor...\n" NONE, users[fd].name);
			} else if(msg.type & CHAT_MSG) {
				// 收到一条信息
				DBG("😘" BLUE "<%s> " NONE " : %s\n", msg.from, msg.msg);
				print_message(sub_msg_win, &msg, 0);
			} else {
				DBG("❀\n");
			}
		}
	}
	return NULL;
}