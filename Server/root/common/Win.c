#include "chat_ui.h"
WINDOW *msg_win, *sub_msg_win, *input_win, *sub_input_win, *help_win, *sub_help_win;
int msg_num;

int main() {
    //  initscr();
    // keypad(sub_input_win, TRUE);
    // echo();
    // cbreak();
    init_ui();
    int a;
    scanf("%d", &a);
	return 0;
}
