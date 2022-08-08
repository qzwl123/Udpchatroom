/*************************************************************************
	> File Name: wechat.h
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月29日 星期五 13时57分31秒
 ************************************************************************/

#ifndef _WECHAT_H
#define _WECHAT_H

#define MAXUSER 1000

#define CHAT_SYN 0x001
#define CHAT_ACK 0x002
#define CHAT_FIN 0x004
#define CHAT_HEART 0x008
#define CHAT_MSG 0x010

struct Msg {
    int type;
    // 从那来
    char from[20];
    // 给谁
    char to[20];
    char msg[1024];
};


#define FL_SYNSENT 0x001
#define FL_ONLINE 0x002

struct User {
    char name[20];
    int fd;
    // 用户状态、0-1在线or不在线
    int flag;
    int cnt;
};


#define CHAT_SYS 0x001
#define CHAT_PRI 0x002

#define MSG_WIDTH 100
#define MSG_HEIGHT 25
#define HELP_WIDTH 15
#define INPUT_HEIGHT 5

struct ChatMsg {
	char name[20];
	char msg[20];
	int type;
};


#endif
