#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>

#define BCN_TRIG_PORT 0xBEAC
#define BCN_TRIG_DEFAULT_MAGIC 0xCABADABA

struct trigger_packet
{
	uint32_t magic;
};

static bool is_trigger_packet(struct trigger_packet packet)
{
	if (ntohl(packet.magic) == BCN_TRIG_DEFAULT_MAGIC)
		return true;

	return false;
}

static int wait_for_trigger_packet(void)
{
	int ret, sock;
	struct trigger_packet packet = {0};
	struct sockaddr_in info;
	socklen_t info_len = sizeof(info);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == -1) {
		perror("Beacon socket creation failed");
		return 1;
	}

	info.sin_family = AF_INET;
	info.sin_addr.s_addr = htonl(INADDR_ANY);
	info.sin_port = htons(BCN_TRIG_PORT);

	ret = bind(sock, (struct sockaddr*)&info, sizeof(struct sockaddr_in));
	if (ret < 0) {
		perror("Beacon socket bind failed");
		close(sock);
		return ret;
	}

	while (1) {
		ret = recvfrom(sock, &packet, sizeof(packet), 0,
			      (struct sockaddr*)&info, &info_len);
		if (ret != sizeof(packet)) {
			fprintf(stderr, "Rcv failed: (ret: %d | errno: %d)\n",
					ret, errno);
			close(sock);
			return 1;
		}

		if (is_trigger_packet(packet)) {
			ret = 0;
			break;
		}
	}

	close(sock);
	return ret;

}

int main(int argc, char** argv)
{
	return wait_for_trigger_packet() ? 1 : 0;
}
