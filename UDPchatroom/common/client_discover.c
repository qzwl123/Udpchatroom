/*************************************************************************
	> File Name: client_discover.c
	> Author: 秃头王
	> Mail: 1658339000@qq.com
	> Created Time: 2022年07月31日 星期日 09时41分02秒
 ************************************************************************/

#include "head.h"

extern char *config;
extern char name[20];
extern char msg[512];

void *client_discover(void *arg) {
    char file[521] = {0};
	FILE *fp;
	struct Msg request;
	struct sockaddr_in client;
	int sockfd;
	sockfd = socket_udp();
	if(sockfd < 0) {
		perror("socket_udp");
		exit(1);
	}
	
	strcpy(request.from, name);
	strcpy(request.msg, msg);
	request.type = CHAT_SYN;

    strcpy(file, get_value(config, "iplist"));
	
	if ((fp = fopen(file, "r")) == NULL) {
		perror("fopen");
		exit(1);
	}

	DBG("< Ip Discover > start...\n");

	client.sin_family = AF_INET;
	client.sin_port = htons(8888);

	while(!feof(fp)) {
		char ip[20] = {0};
		fscanf(fp, "%s", ip);
		if(!strlen(ip)) continue;
		client.sin_addr.s_addr = inet_addr(ip);
		sendto(sockfd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, sizeof(client));
		// DBG(GREEN "<request Disscover>" NONE "I'm %s, msg is %s..\n",request.from, request.msg);
		// DBG(GREEN "<name, msgDisscover>" NONE "I'm %s, msg is %s..\n",name, msg);
		// DBG("<Ip Discover> %s\n", ip);
	}
	DBG("< Ip Discover > end.\n");

	return NULL;
}
