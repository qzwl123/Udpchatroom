/*************************************************************************
	> File Name: common.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月28日 星期四 21时34分26秒
 ************************************************************************/

#include "head.h"

char conf_value_ans[512] = {0};

// 在配置文件中查找
char *get_value(const char *path, const char *key) {

    if(path == NULL || key == NULL) {
        /*
            fprintf(stderr, "Errot in argument\n");
            errno = EINVAL;
            return NULL;
        */
        fprintf(stderr, "Errot in argument\n");
        exit(1);
    }

    FILE *fp = NULL;
    char *line = NULL, *sub = NULL;
    // ssize_t ==  long int * or int 
    size_t  nread, len;

    if((fp = fopen(path, "r")) == NULL) {
        fprintf(stderr, "file open error!\n");
        exit(1);
        // return NULL;
    }
    
    while((nread = getline(&line,&len, fp)) != -1) {
        if((sub = strstr(line, key)) == NULL) continue;
        if(line[strlen(key)] == '=' && sub == line) {
            strcpy(conf_value_ans, line + strlen(key) + 1);
            // 去掉回车
            conf_value_ans[strlen(conf_value_ans) - 1] = '\0';
        }
    }

    free(line);
    fclose(fp);
    return conf_value_ans;
}
