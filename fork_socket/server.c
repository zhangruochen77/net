#include "wrap.h"
#include <netinet/in.h>
#include <signal.h>

void func(int signum) {
	while ((waitpid(0, NULL, WNOHANG)) > 0);
}

int main() {
	struct sigaction act;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, NULL);

	int domain = AF_INET, type = SOCK_STREAM, ret;
	int lfd, cfd, opt = 1;
	struct sockaddr_in ser_addr, cli_addr;
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(8080);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定系统可用地址
	char buf[128];

	lfd = Socket(domain, type, 0);
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // 设置端口复用
	Bind(lfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
	Listen(lfd, 128);

	while (1) {
		bzero(&cli_addr, sizeof(cli_addr));
		socklen_t clilen = sizeof(cli_addr);
		cfd = Accept(lfd, (struct sockaddr *)&cli_addr, &clilen);

		pid_t pid = fork();
		if (-1 == pid) {
			err("fork error\n");
		} else if (0 == pid) {
			Close(lfd);
			while ((ret = Read(cfd, &buf, sizeof(buf))) != 0) {
				Write(STDOUT_FILENO, &buf, ret);
				int i = 0;
				for (; i < ret; i++) {
					buf[i] = toupper(buf[i]);
				}
				Write(STDOUT_FILENO, &buf, ret);
				Write(cfd, &buf, ret);
			}

			Close(cfd);
			break;
		} else {
			Close(cfd);
		}
	}

	return 0;
}
