

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#ifndef NETUTIL_H
#define NETUTIL_H



#include <sys/types.h>
#include <pgen/pgen-types.h>
#include <pgen/io/pgen-netutil.h>
#include <pgen/io/pgen-util.h>
#include <pgen/io/pgen-error.h>



/* for pgen_open_offline()'s  *
 * 2nd argument, opening mode */
#define PCAP_READ 0
#define PCAP_WRITE 1
#define PCAPNG_READ 2
#define PCAPNG_WRITE 3


pgen_t* pgen_open(const char* dev, void*);
pgen_t* pgen_open_offline(const char*, int mode);
void pgen_close(pgen_t* p);

void pgen_perror(const char* str);		/* print err-msg with string */

int pgen_sendpacket_L2(const char*, const void*, int);
int pgen_sendpacket_L3(const char*, const void*, int, struct sockaddr*);
int pgen_sendpacket_handle(pgen_t* p, const void* packet, int len);




#endif /* NETUTIL_H */
