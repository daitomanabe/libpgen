
/*
 * $B%Q%1%C%H%/%i%9$N;HMQNc(B
 * $B%X%C%@MWAG$N%"%/%;%9$OA4$FFbIt$G(Bsetter, getter$B$r;HMQ$7$F$$$k$b$N$H$9$k!#(B
 * $B4v$D$+Nc$r<($9$,!"$3$l$O$?$@!"%$%s%?!<%U%'!<%9$N:9$J$N$G(B
 * $BBP$7$F:9$O$J$$$,!"8D?ME*$K$O>/$7$@$1$3$@$o$j$,$"$k!#!#!#!#(B($B>P(B)
 */
#include <pgen2.h>



/* $B0lHVL5Fq(B($B$H$j$"$($:!"$3$l$G<BAu$7$F9T$3$&$H;W$&(B) */
int example2() {
	pgen::core::arp pack;

	pack.ARP.set_operation(pgen::arp::reply);
	pack.ARP.set_hwsrc("22:22:22:22:22:22");
	pack.ARP.set_psrc("192.168.0.2");
	pack.ARP.set_hwdst("11:11:11:11:11:11");
	pack.ARP.set_pdst("192.168.0.1");
	pack.ETH.set_src("22:22:22:22:22:22");
	pack.ETH.set_dst("11:11:11:11:11:11");
	
	pack.summary();
	pack.hex();
}

/* $BA4$F$NMWAG$r%/%i%92=(B($BD6%3!<%IA}$($=$&$@$1$I!"9%$-(B) */
int example2() {
	pgen::core::arp pack;

	pack.ARP.operation.set(pgen::arp::reply);
	pack.ARP.hwsrc.set("22:22:22:22:22:22");
	pack.ARP.psrc.set("192.168.0.2");
	pack.ARP.hwdst.set("11:11:11:11:11:11");
	pack.ARP.pdst.set("192.168.0.1");
	pack.ETH.src.set("22:22:22:22:22:22");
	pack.ETH.dst.set("11:11:11:11:11:11");
	
	pack.summary();
	pack.hex();
}


/* $BA4$F$$$^$^$G$I$&$j$K%"%/%;%92DG=(B($B$I$&$d$C$FI=8=$9$l$P$$$$$N$+ITL@(B) */
int example1() {

	pgen::arp pack;

	pack.ARP.operation = pgen::arp::reply;
	pack.ARP.hwsrc = "22:22:22:22:22:22";
	pack.ARP.psrc  = "192.168.0.2";
	pack.ARP.hwdst = "11:11:11:11:11:11";
	pack.ARP.pdst  = "192.168.0.1";
	pack.ETH.src   = "22:22:22:22:22:22";
	pack.ETH.dst   = "11:11:11:11:11:11";
	
	pack.summary();
	pack.hex();
}
