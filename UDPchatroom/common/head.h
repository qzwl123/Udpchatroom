/*************************************************************************
	> File Name: head.h
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月28日 星期四 15时35分12秒
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>

#include <ncurses.h>
#include <locale.h> 
#include "chat_ui.h"

#include "color.h"
#include "udp_client.h"
#include "udp_server.h"
#include "common.h"
#include "wechat.h"
#include "reactor.h"
#include "client_discover.h"
#include "heart_beat.h"
#include "send_chat.h"


#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...) 
#endif

#endif
