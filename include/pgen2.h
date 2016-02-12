

#include <pgen2/core/address.h>
#include <pgen2/core/header.h>
#include <pgen2/io/util.h>
#include <pgen2/io/arch.h>
#include <pgen2/types/data_container.h>




namespace pgen {
    using io::util::hex;
    


	// This component's designing will be do after designed core and io. 
    namespace module {}

    namespace core {
        namespace header {}
        // class packet;
        // class ethernet  : packet; #<{(| has eth header              |)}>#
        // class arp       : packet; #<{(| has eth and arp header      |)}>#
        // class ip        : packet; #<{(| has eth and ip header       |)}>#
        // class icmp      : packet; #<{(| has eth, ip and icmp header |)}>#
        // class tcp       : packet; #<{(| has eth, ip and tcp header  |)}>#
        // class udp       : packet; #<{(| has eth, ip and udp header  |)}>#
        //
        // class ipaddress;
        // class macaddress;
        // class port;
    }

    namespace io {

        namespace util {
            // void hex(const void* buffer, size_t bufferlen);
        }
        namespace netutil {}
        namespace arch {}
    }
	
	namespace types {
        // class data_container;
    } 
}



namespace core {

	          
}
