/*************************************************************************
	> File Name: chat_ui.h
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年08月03日 星期三 15时04分50秒
 ************************************************************************/

#ifndef _CHAT_UI_H
#define _CHAT_UI_H

void gotoxy(int x, int y);
void gotoxy_putc(int x, int y, char c);
void gotoxy_puts(int x, int y, char *s);
// 去一个窗口做一个事情
void w_gotoxy_putc(WINDOW *win, int x, int y, char c);
void w_gotoxy_puts(WINDOW *win, int x, int y, char* s);
void show_help();
// 初始化ui
void init_ui();
// 消息循环播放
void print_message(WINDOW *, struct Msg *, int );

WINDOW* create_newwin(int, int, int, int);


#endif
