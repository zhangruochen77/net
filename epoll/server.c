#include "wrap.h"
#include <sys/epoll.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SIZE 1024
#define PORT 8080
#define BUFSIZE 128

int main() {
	int ret;
	int domain = AF_INET, type = SOCK_STREAM;
	int lfd, cfd;
	struct sockaddr_in seraddr, cliaddr;
	char buf[BUFSIZE];
	seraddr.sin_family = domain;
	seraddr.sin_addr.s_addr = INADDR_ANY;
	seraddr.sin_port = htons(PORT);
	int epfd = epoll_create(SIZE);		// 创建一个 epoll 文件句柄
	lfd = Socket(domain, type, 0);	// 创建服务端 socket 连接
	Bind(lfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
	Listen(lfd, SIZE);

	struct epoll_event ev, evout[SIZE];
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev); // 添加lfd到epoll句柄当中
	printf("server start port %d\n", PORT);
	while (1) {
		ret = epoll_wait(epfd, evout, SIZE, -1);
		int i;
		for (i = 0; i < ret; i++) {
			if (EPOLLIN != evout[i].events) {
				continue;
			}

			if (lfd == evout[i].data.fd) { // 客户端连接
				socklen_t clilen = sizeof(cliaddr);
				cfd = Accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
				struct epoll_event cev;
				cev.data.fd = cfd;
				cev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cev);
				printf("get conn to cfd = %d\n", cfd);
			} else { // 客户端写数据
				ret = Read(evout[i].data.fd, &buf, BUFSIZE);
				if (0 == ret) {
					printf("conn close %d\n", evout[i].data.fd);
					epoll_ctl(epfd, EPOLL_CTL_DEL, evout[i].data.fd, NULL);
					Close(evout[i].data.fd);
				} else {
					printf("conn get info %d\n", evout[i].data.fd);
					Write(STDOUT_FILENO, &buf, ret);
					int i = 0;
					for (; i < ret; i++) {
						buf[i] = toupper(buf[i]);
					}
					Writen(evout[i].data.fd, &buf, ret);
				}
			}
		}
	}
}
