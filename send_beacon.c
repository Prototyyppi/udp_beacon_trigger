#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <arpa/inet.h>

// Router
#define BCN_TRIG_INET "192.168.1.1"
#define BCN_TRIG_INET_MASK 0xFFFFFF00

#define BCN_TRIG_PORT 0xBEAC
#define BCN_TRIG_DEFAULT_MAGIC 0xCABADABA

struct trigger_packet
{
	uint32_t magic;
};

static int send_trigger_packet(void)
{
	int ret, sock, ena = 1;
	struct sockaddr_in info;
	struct trigger_packet packet =
	{
		.magic = htonl(BCN_TRIG_DEFAULT_MAGIC)
	};

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == -1) {
		perror("Beacon socket creation failed");
		return 1;
	}

	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &ena, sizeof(ena));
	if (ret == -1) {
		perror("Beacon sending permission set failed");
		close(sock);
		return 1;
	}

	info.sin_family = AF_INET;
	/*
	 * Broadcast address can be derived from ip as follows:
	 * eg. inet 192.168.1.103 netmask 0xffffff00 broadcast 192.168.1.255.
	 * So like: INET_ADDRESS | ~INET_MASK;
	 *
	 * htonl(INADDR_BROADCAST) = 255.255.255.255 can also be used,
	 * and may be fine depending on situation. But because sendto only sends
	 * one packet the receiving interface can be ambiguous. (For example if
	 * computer is connected to wired and wireless, which interface gets the
	 * packet?).
	 */
	info.sin_addr.s_addr = inet_addr(BCN_TRIG_INET) |
			       ~htonl(BCN_TRIG_INET_MASK);
	info.sin_port = htons(BCN_TRIG_PORT);

	ret = sendto(sock, &packet, sizeof(packet), 0,
		    (struct sockaddr *)&info, sizeof(info));
	if (ret < 0) {
		perror("Beacon sendto failed");
		close(sock);
		return 1;
	}

	close(sock);
	return 0;
}

int main(int argc, char** argv)
{
	return send_trigger_packet();
}
