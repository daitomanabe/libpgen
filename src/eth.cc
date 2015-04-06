#include "packet.h"
#include "pgen-variable.h"

#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <net/ethernet.h> 

pgen_eth::pgen_eth(){
	pgen_packet::clear();
	clear();	
}

void pgen_eth::clear(){
	eth_srcEth = 0;
	eth_dstEth = 0;
}

void pgen_eth::wrap(const char* ifname){
	packetType = PGEN_PACKETTYPE_ETH;
	memset(data, 0, sizeof data);

	eth.ether_type = htons(0);
	for(int i=0; i< 6; i++){
		eth.ether_shost[i] = eth_srcEth._addr[i];	
		eth.ether_dhost[i] = eth_dstEth._addr[i];	
	}

	u_char* p = data;
	memcpy(data, &eth, sizeof eth);
	p += sizeof(eth);
	len = p - data;


	if((sock=socket(AF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) < 0){
		perror("eth::wrap bind()");
		exit(PGEN_ERROR);
	}

	memset(&addr, 0, sizeof addr);
	addr.sa_family = AF_PACKET;
	snprintf(addr.sa_data, sizeof(addr.sa_data), "%s", ifname);
	if(bind(sock, &addr, sizeof(addr)) < 0){
		perror("eth::wrap bind()");
		exit(PGEN_ERROR);
	}
}



void pgen_eth::info(){
	std::map<int , const char*> _ethtype;
	_ethtype[0x0800] = "IPv4";
	_ethtype[0x0806] = "ARP";
	_ethtype[0x8035] = "RARP";
	_ethtype[0x8191] = "NetBios";
	_ethtype[0x86dd] = "IPv6";

	printf(" * Ethernet  %s -> %s \n", eth_srcEth.bender(), eth_dstEth.bender());
	printf("    - Destination     :  %s (%s)   \n", eth_dstEth.c_str(), eth_dstEth.bender());
	printf("    - Source          :  %s (%s)   \n" , eth_srcEth.c_str(), eth_srcEth.bender());
	printf("    - Type            :  %s  (0x%04x)   \n", 
			_ethtype[htons(eth.ether_type)] ,htons(eth.ether_type));
}
