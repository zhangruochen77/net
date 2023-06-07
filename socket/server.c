#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BLOCK_LOG 128
#define BUF_SIZE 128
#define SERVER_ADDR "127.0.0.1"
int ret;

void err(const char* msg) {
	perror(msg);
	perror(strerror(errno));
	_exit(1);
}



int main() {
	int domain = AF_INET, type = SOCK_STREAM, protocal = 0; 
	int sfd = socket(domain, type, protocal); // 1.地址类型 ipv4 2.协议 tcp 3. 选择协议方式 0 跟随默认
	if (-1 == sfd) {
		err("socket error\n");
	}

	struct sockaddr_in addr_in;
	addr_in.sin_family = domain;				// 设置地址类型
	addr_in.sin_port = htons(PROT);				// 绑定端口 本地字节序转网络字节序
	addr_in.sin_addr = inet_addr(SERVER_ADDR);	// 绑定地址
	ret = bind(sfd, (struct sockaddr *)&addr_in, sizeof(addr_in)); // 1 socker fd, 2 host + ip, addr_in 强制转化使用， 3 地址大小
	if (-1 == ret) {
		err("bind server addr error\n");
	}

	ret = listen(sfd, BLOCK_LOG); // 设置socket最大连接上限
	if (-1 == ret) {
		err("set listen error\n");
	}

	struct sockadd_in n_addr_in;
	int addr_len = sizeof(n_addr_in);
	int nfd = accept(sfd, (struct sockaddr *)&n_addr_in, &n_addr_in); // 用于接收连接 会返回连接成功的sock_fd, 以及clent的信息
	if (-1 == nfd) {
		err("accept new socket error\n");
	}
	
	char buf[BUF_SIZE]
	while (1) {
		ret = read(nfd, &buf, BUF_SIZE);
		int i = 0;
		for (; i < ret; i++) {
			buf[i] = toupper(buf[i]);
		}

	}

	close(nfd); // 关闭 socket 连接
	return 0;
}
