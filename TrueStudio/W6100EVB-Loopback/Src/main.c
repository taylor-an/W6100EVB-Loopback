#include <stdio.h>
#include "wizchip_conf.h"
#include "loopback.h"
#include "board_init.h"

wiz_NetInfo gWIZNETINFO = { .mac = {0x00,0x08,0xdc,0xFF,0xFF,0xFF},
							.ip = {192,168,0,80},
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 0, 1},
							.dns = {168, 126, 63, 1},
							//.dhcp = NETINFO_STATIC,
							.lla={0xfe,0x80,0x00,0x00,
									0x00,0x00, 0x00,0x00,
									0x02,0x08, 0xdc,0xff,
									0xfe,0x57, 0x57,0x25},   ///< Source Link Local Address
							.gua={0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00},   ///< Source Global Unicast Address
							.sn6={0xff,0xff,0xff,0xff,
									0xff,0xff,0xff,0xff,
									0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00 },   ///< IPv6 Prefix
							.gw6={0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00}   ///< Gateway IPv6 Address
};

uint8_t WIZ_Dest_IP_virtual[4] = {192, 168, 0, 230};                  //DST_IP Address
uint8_t WIZ_Dest_IP_Google[4] = {216, 58, 200, 174};              //DST_IP Address

uint8_t mcastipv4_0[4] ={239,1,2,3};
uint8_t mcastipv4_1[4] ={239,1,2,4};
uint8_t mcastipv4_2[4] ={239,1,2,5};
uint8_t mcastipv4_3[4] ={239,1,2,6};

uint16_t WIZ_Dest_PORT = 15000;                                 //DST_IP port

#define ETH_MAX_BUF_SIZE	1024*16

uint8_t  remote_ip[4] = {192,168,177,200};                      //
uint16_t remote_port = 8080;

unsigned char ethBuf0[ETH_MAX_BUF_SIZE];
#if 0
unsigned char ethBuf1[ETH_MAX_BUF_SIZE];
unsigned char ethBuf2[ETH_MAX_BUF_SIZE];
unsigned char ethBuf3[ETH_MAX_BUF_SIZE];
unsigned char ethBuf4[ETH_MAX_BUF_SIZE];
unsigned char ethBuf5[ETH_MAX_BUF_SIZE];
unsigned char ethBuf6[ETH_MAX_BUF_SIZE];
unsigned char ethBuf7[ETH_MAX_BUF_SIZE];
#endif

uint8_t bLoopback = 1;
uint8_t bRandomPacket = 0;
uint8_t bAnyPacket = 0;
uint16_t pack_size = 0;

void print_network_information(void);

int main(void)
{
	volatile int i;
	volatile int j,k;
 	uint16_t ver=0;
 	uint16_t curr_time = 0;
	uint8_t syslock = SYS_NET_LOCK;
	uint8_t svr_ipv4[4] = {192, 168, 177, 235};
	uint8_t svr_ipv6[16] = {0xfe, 0x80, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00,
							0xc1, 0x0b, 0x0a, 0xdf,
							0xea, 0xf4, 0xf4, 0x2d};

	BoardInitialze();
	
	// 20220307 taylor
	printf("Compiled@%s/%s\r\n", __DATE__, __TIME__);

	ctlwizchip(CW_SYS_UNLOCK,& syslock);
	ctlnetwork(CN_SET_NETINFO,&gWIZNETINFO);

	printf("Register value after W6100 initialize!\r\n");

	print_network_information();

#ifdef DMA
	printf("DMA SPI\r\n");
#else
	printf("SPI\r\n");
#endif

	while(1)
    {

#if 0
		// tcp server
		loopback_tcps(0,ethBuf0,50000,AS_IPV4);
#else
		// udp server
	    loopback_udps(0,ethBuf0,50000,AS_IPV4);
#endif
#if 0
		loopback_tcps(1,ethBuf1,50003,AS_IPV4);
		loopback_tcps(2,ethBuf2,50004,AS_IPV6);
		loopback_tcps(3,ethBuf3,50005,AS_IPDUAL);
#endif
    }
}

void delay(unsigned int count)
{
	int temp;
	temp = count + TIM2_gettimer();
	while(temp > TIM2_gettimer()){}
}

void print_network_information(void)
{
	wizchip_getnetinfo(&gWIZNETINFO);

	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("SM Mask    : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);

	print_ipv6_addr("GW6 ", gWIZNETINFO.gw6);
	print_ipv6_addr("LLA ", gWIZNETINFO.lla);
	print_ipv6_addr("GUA ", gWIZNETINFO.gua);
	print_ipv6_addr("SUB6", gWIZNETINFO.sn6);

	printf("\r\nNETCFGLOCK : %x\r\n", getNETLCKR());
}

void print_ipv6_addr(uint8_t* name, uint8_t* ip6addr)
{
	printf("%s : ", name);
	printf("%04X:%04X", ((uint16_t)ip6addr[0] << 8) | ((uint16_t)ip6addr[1]), ((uint16_t)ip6addr[2] << 8) | ((uint16_t)ip6addr[3]));
	printf(":%04X:%04X", ((uint16_t)ip6addr[4] << 8) | ((uint16_t)ip6addr[5]), ((uint16_t)ip6addr[6] << 8) | ((uint16_t)ip6addr[7]));
	printf(":%04X:%04X", ((uint16_t)ip6addr[8] << 8) | ((uint16_t)ip6addr[9]), ((uint16_t)ip6addr[10] << 8) | ((uint16_t)ip6addr[11]));
	printf(":%04X:%04X\r\n", ((uint16_t)ip6addr[12] << 8) | ((uint16_t)ip6addr[13]), ((uint16_t)ip6addr[14] << 8) | ((uint16_t)ip6addr[15]));
}
