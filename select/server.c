#include "wrap.h"
#include <sys/select.h>
#include <ctype.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
	int domain = AF_INET, type = SOCK_STREAM;
	int lfd, cfd, ret, ready, opt = 1;
	struct sockaddr_in ser_addr, cli_addr;
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr.sin_family = domain;
	ser_addr.sin_port = htons(8080);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	lfd = Socket(domain, type, 0);
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	Bind(lfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
	Listen(lfd, 128);

	int max = lfd;
	char buf[128];
	fd_set readfds;
	FD_SET(lfd, &readfds); // 添加lfd进入监听队列
	int fds[1024];
	int idx = 0;
	for (; idx < 1024; idx++) {
		fds[idx] = -1;
	}

	fds[lfd] = 1;

	while (1) {
		fd_set rfds = readfds;
		ready = select(max + 1, &rfds, NULL, NULL, NULL); // 注册集合和返回集合不同
		if (-1 == ready) {
			err("select error\n");
		}
	
		if (FD_ISSET(lfd, &rfds)) { // 判断是否有挂载
			socklen_t clilen = sizeof(cli_addr);
			bzero(&cli_addr, clilen);
			cfd = Accept(lfd, (struct sockaddr*)&cli_addr, &clilen);

			FD_SET(cfd, &readfds);
			fds[cfd] = 1;
			if (cfd > max) max = cfd; // 需要随时更新最大值 这是监听范围

			if (0 == (--ready)) continue; // 只有注册新连接 那么就退出了
		}

		int i = 0;
		for (; i <= max; i++) {
			if (fds[i] == -1) { // 解决重复访问
				continue;
			}

			if (FD_ISSET(i, &rfds)) {
				if ((ret = Read(i, &buf, sizeof(buf))) == 0) { // 结束标志 退出 取消注册
					Close(i);
					FD_CLR(i, &readfds);
					fds[i] = -1;
				} else {
					Write(STDOUT_FILENO, &buf, ret);
					int j = 0;
					for (; j < ret; j++) {
						buf[i] = toupper(buf[i]);
					}
					Write(cfd, &buf, ret);
				}

				if (0 == (--ready)) break;
			}

		}
	}

	return 0;
}
