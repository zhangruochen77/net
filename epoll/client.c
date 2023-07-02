#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>

void err(const char* msg) {
	perror(msg);
	perror(strerror(errno));
	_exit(0);
}

int main() {
	int domain = AF_INET, type = SOCK_STREAM, protocal = 0;
	int cfd = socket(domain, type, protocal);
	if (-1 == cfd) {
		err("create socket error\n");
	}

	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
//	addr_in.sin_port = 8080;
//	addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &addr_in.sin_addr);
	addr_in.sin_port = htons(8080);
	int ret = connect(cfd, (struct sockaddr *)&addr_in, sizeof(addr_in));
	if (-1 == ret) {
		err("connet error\n");
	}

	int i = 10;
	char buf[50];
	while (--i) {
		ret = write(cfd, "hello world\n", sizeof("hello world\n"));
		if (-1 == ret) {
			err("write error\n");
		}

		ret = read(cfd, &buf, sizeof(buf));
		if (-1 == ret) {
			err("read error\n");
		}

		printf("%s\n", buf);
		sleep(1);
	}

	close(cfd);
	return 0;
}
