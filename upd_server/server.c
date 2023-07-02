#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#define true 1
#define false 0

int main() {
	int domain = AF_INET , type = SOCK_DGRAM;
	char buf[128];
	sockaddr_in = ser_addr;
	bzero(&ser_addr, sizeof(ser_addr)); // 对象信息清空
	ser_addr.sin_family = domain;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定唯一可用地址
	ser_addr.sin_port = htons(8080); // 绑定端口

	int sfd = socket(domain, type, 0); // 获取socket
	bind(sfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)); // 绑定服务器

	while (true) {
		struct sockaddr_in = cli_addr;
		socklen_t clilen = sizeof(cli_addr);
		ssize_t read_size = recvfrom(sfd, buf, sizeof(buf), flags, (struct sockaddr*)&cli_addr, clilen);
		if (read_size == 0) {
			perror("recvfrom error");
			exit(1);
		}

		int i = 0;
		for (; i < read_size; i++) {
			toupper(buf[i]);
		}

		sendto()
	}
}
