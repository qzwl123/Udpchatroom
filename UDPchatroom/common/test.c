/*************************************************************************
	> File Name: test.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年08月07日 星期日 20时11分30秒
 ************************************************************************/

#include <curses.h>
 
int main()
{
        initscr();//ncurse界面的初始化函数
        printw("this is a curses window\n");//在ncurse模式下的printf
        getch();//等待用户的输入，如果没有这句话，程序就退出了，看不到运行的结果，也就是无法看到上面那句话
        endwin();//程序退出，恢复shell终端的显示，如果没有这句话，shell终端字乱码，坏掉
 
        return 0;
}
