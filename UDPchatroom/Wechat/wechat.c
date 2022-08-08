/*************************************************************************
	> File Name: wechat.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月28日 星期四 17时20分48秒
 ************************************************************************/

#include "head.h"

#define MAX 10
#define MAXUSER 1000
const char *config = "./chat.conf";
int port = 0;
struct User *users;
char name[20] = {0}, msg[521] = {0};
int subfd;

WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;
int msg_num;
struct ChatMsg chatmsg;

void loqout(int signum) {
	struct Msg l_msg;
	l_msg.type = CHAT_FIN;
	strcpy(l_msg.from, name);
	strcpy(l_msg.msg, "Bye!");
	for(int i = 0; i < MAXUSER; i++) {
		if(users[i].flag & FL_ONLINE) {
			send(users[i].fd, &l_msg, sizeof(l_msg), 0);			
		}
	}
	DBG("\n 🙅‍ logout from wechat system...\n");
	exit(0);
}

// 先从服务端角度开始写
int main(int argc, char **argv) {
    int opt, listener, epollfd, new_fd;
	pthread_t c_discover, heart_beat_tid, reactor_tid, send_tid;
    while((opt = getopt(argc, argv, "p:n:m")) != -1) {
		switch (opt) {
			case 'p':
				port = atoi(optarg);
				break;
			case 'm':
				strcpy(msg, optarg);
				break;
			case 'n':
				strcpy(name, optarg);
				break;
            default:
                fprintf(stderr, "Usage : %s -n name -p prot -m msg\n", argv[0]);
            	exit(-1);
		}
	}
	// ctr + c 退出时候
	signal(SIGINT, loqout);
    if(!strlen(name)) strncpy(name, get_value(config, "name"), 20);
    if(!strlen(msg)) strncpy(msg, get_value(config, "msg"), sizeof(msg));
    if(!port) port = atoi(get_value(config, "port"));
   

	DBG(BLUE "<Starting...> " NONE "name = %s, prot = %d, msg = %s\n", name, port, msg);

    if((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }

	DBG(BLUE "<Starting...>" NONE "listener = %d ok\n", listener);
	/*
		epoll_create
		创建一个新的epoll实例。在内核中创建了一个数据，
		这个数据中有两个比较重要的数据，一个是需要检测
		的文件描述符的信息（红黑树），还有一个是就绪列表
		存放检测到数据发送改变的文件描述符信息（双向链表）。
	*/
	// 主反应堆-epollfd文件描述符，操作epoll实例
	if((epollfd = epoll_create(1)) < 0) {
		perror("epoll_create");
		exit(1);
	}
	// 从应堆-操作epoll实例文件描述符
	if((subfd = epoll_create(1)) < 0) {
		perror("epoll_create");
		exit(1);
	}
	
	struct epoll_event ev, events[MAX];

	ev.events = EPOLLIN;
	ev.data.fd = listener;
	// 主机套接字注册
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
		perror("epoll_ctl");
		exit(1);
	}

	users = (struct User *)calloc(MAXUSER, sizeof(struct User));
	
	setlocale(LC_ALL, "zh_CN.utf8");

	initscr();
	noecho();
	cbreak();
    keypad(sub_input_win, TRUE);
 	init_ui();

	/*
		1.线程 1 号 给所有 iplist 列表发送 SYN 请求
		2.线程 2 号 发送心跳线程
		3.线程 3 号 处理心跳线程(从反应堆)
		4.线程 3 号 发送数据
		
	*/

	pthread_create(&c_discover, NULL, client_discover, NULL);
	pthread_create(&heart_beat_tid, NULL, heart_beat, NULL);
	pthread_create(&reactor_tid, NULL, reactor, NULL);
	pthread_create(&send_tid, NULL, send_chat, NULL);

	// 主反应堆的一些安全验证、验证成成功添加到从反应堆
	while (1) {
		int nfds = epoll_wait(epollfd, events, MAX, -1);
		DBG("⚙" RED "<Mian Reactor> " NONE ": (有事件来)Got Events..\n");
		if(nfds < 0) {
			perror("epoll_wait");
			exit(1);
		}

		for(int i = 0; i < nfds; i++) {
			struct User newuser;
			new_fd = udp_accept(listener, &newuser);
			printf(" new_fd = %d\n",new_fd);
			if(new_fd < 0) {
				fprintf(stderr, "error in udp_accept!\n");
				continue;
			}

			// 添加到从反应堆、
			add_to_reactor(subfd, &newuser);
			// DBG(": Adding client to sub reactor..\n");
			DBG("⚙" RED "<Mian Reactor>" NONE " : (添加到从反应堆)Adding client to sub reactor..\n");
		}
	}
	

    return 0;
}
