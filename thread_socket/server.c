#include "wrap.h"
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <ctype.h>

struct Info {
	int fd;						// 操作的文件描述符号
	struct sockaddr_in cli_addr;// 客服端信息
	socklen_t len;				// 地址长度
};

void *func(void *arg) {
	struct Info *info = (struct Info*)arg;
	int ret;
	char buf[128];

	while ((ret = Read(info->fd, &buf, sizeof(buf))) != 0) {
		int i;
		Write(STDOUT_FILENO, &buf, ret);
		for (i = 0; i < ret; i++) {
			buf[i] = toupper(buf[i]);
		}
		Write(STDOUT_FILENO, &buf, ret);
		Write(info->fd, &buf, ret);
	}

	close(info->fd); // 结束 关闭
	pthread_exit(NULL);
}

int main() {
	int domain = AF_INET, type = SOCK_STREAM;
	int lfd, cfd;
	int opt = 1;

	struct sockaddr_in ser_addr, cli_addr;
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(8080);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 系统分配可用地址

	lfd = Socket(domain, type, 0);
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // 开启端口复用
	Bind(lfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
	Listen(lfd, 128);

	while (1) {
		socklen_t clilen = sizeof(cli_addr);
		bzero(&cli_addr, clilen);
		cfd = Accept(lfd, (struct sockaddr *)&cli_addr, &clilen);

		pthread_t thd;
		struct Info info;
		info.fd = cfd;
		info.cli_addr = cli_addr;
		info.len = clilen;
		pthread_create(&thd, NULL, func, (void *)&info);
	}

	return 0;
}
