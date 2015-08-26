/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "address.h"

#include "netutil.h"

#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>		// for struct tcp		



pgen_tcp::pgen_tcp(){
	clear();
}



pgen_tcp::pgen_tcp(const u_char* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_tcp::clear(){
	pgen_ip::clear();
	this->TCP.src = 20;
	this->TCP.dst = 80;
	this->TCP.flags.fin = 0;
	this->TCP.flags.syn = 0;
	this->TCP.flags.rst = 0;
	this->TCP.flags.psh = 0;
	this->TCP.flags.ack = 0;
	this->TCP.flags.urg = 0;
	this->TCP.window = 8192;
	this->TCP.seq = 0;
	this->TCP.ack = 0;
}





void pgen_tcp::compile(){
	this->IP.tot_len = IP_HDR_LEN + TCP_HDR_LEN;
	this->IP.protocol = MT_IPPROTO_TCP;
	pgen_ip::compile();

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char buf[1000];
	u_char *bp;
	memset(buf, 0, sizeof buf);

	memset(&this->tcp, 0, TCP_HDR_LEN);
	this->tcp.source = htons(this->TCP.src);
	this->tcp.dest   = htons(this->TCP.dst);
	this->tcp.seq    = htonl(this->TCP.seq);
	this->tcp.ack_seq = htonl(this->TCP.ack);
	this->tcp.doff = 20 >> 2;  // header length
	this->tcp.window = htons(this->TCP.window);
	this->tcp.check  = 0;
	if(this->TCP.flags.fin == 1)	this->tcp.fin = 1;
	if(this->TCP.flags.syn == 1)	this->tcp.syn = 1;
	if(this->TCP.flags.rst == 1)	this->tcp.rst = 1;
	if(this->TCP.flags.psh == 1)	this->tcp.psh = 1;
	if(this->TCP.flags.ack == 1)	this->tcp.ack = 1;
	if(this->TCP.flags.urg == 1)	this->tcp.urg = 1;
	
	bp = buf;
	memcpy(bp, &this->ip, IP_HDR_LEN);
	bp += IP_HDR_LEN;
	memcpy(bp, &this->tcp, TCP_HDR_LEN);
	bp += TCP_HDR_LEN;
	this->tcp.check = checksumTcp(buf, bp-buf);

	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	memcpy(p, &this->tcp, TCP_HDR_LEN);
	p += TCP_HDR_LEN;
	len = p - this->data;
	
	compile_addData();
}





void pgen_tcp::cast(const u_char* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_tcp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_ip::cast(data, len);

	const u_char* p = data;
	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;

	struct MYTCP* buf = (struct MYTCP*)p;
	p += TCP_HDR_LEN;

	this->TCP.src = ntohs(buf->source);
	this->TCP.dst = ntohs(buf->dest);
	this->TCP.seq = ntohl(buf->seq);
	this->TCP.ack = ntohl(buf->ack_seq);
	this->TCP.window = ntohs(buf->window);
	this->TCP.flags.fin = buf->fin;
    this->TCP.flags.syn = buf->syn;
    this->TCP.flags.rst = buf->rst;
    this->TCP.flags.psh = buf->psh;
    this->TCP.flags.ack = buf->ack;
	this->TCP.flags.urg = buf->urg;
	
	this->len = p - data;
	addData(p, len-(p-data));
}



void pgen_tcp::summary(){
	compile();
	printf("TCP{ ");
	int len = tcp.doff;
	std::string flag;
	if(tcp.fin == 1)	flag+= "FIN";
	if(tcp.syn == 1)	flag+= "SYN";
	if(tcp.rst == 1)	flag+= "RST";
	if(tcp.psh == 1)	flag+= "PSH";
	if(tcp.ack == 1)	flag+= "ACK";
	if(tcp.urg == 1)	flag+= "URG";
	
	printf("%d > %d [%s] seq=%d win=%d len=%d }\n",
			TCP.src, TCP.dst, flag.c_str(), TCP.seq, TCP.window, len);
}





void pgen_tcp::info(){
	compile();
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %d (%s) \n",
			TCP.src, pgen_port2service(TCP.src, 1));
	printf("    - Dest Port       :  %d (%s) \n", 
			TCP.dst, pgen_port2service(TCP.dst, 1));
	printf("    - Frags           :  ");
	if(tcp.fin == 1)	printf("F");
	if(tcp.syn == 1)	printf("S");
	if(tcp.rst == 1)	printf("R");
	if(tcp.psh == 1)	printf("P");
	if(tcp.ack == 1)	printf("A");
	if(tcp.urg == 1)	printf("U");
	printf("\n");
	printf("    - Window size     :  %d \n", TCP.window);
	printf("    - Checksum        :  0x%04x \n", ntohs(tcp.check));
	printf("    - sequence        :  %u \n", TCP.seq);
	printf("    - acknowledge     :  %u \n", TCP.ack);

}




