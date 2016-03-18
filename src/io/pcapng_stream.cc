

#include <pgen2/io/stream.h>
#include <pgen2/io/pcapng_stream.h>
#include <pgen2/exception.h>
#include <pgen2/util.h>
#include <assert.h>



namespace pgen {


struct pcapng_block_head {
    uint32_t type;
    uint32_t total_length;
};

struct pcapng_block_tail {
    uint32_t total_length;
};



void pcapng_block::read(const void* buffer, size_t bufferlen) {
    if (bufferlen < sizeof(pcapng_block_head) + impl_length() + sizeof(pcapng_block_tail)) 
        throw pgen::exception("pgen::pcapng_SHB::read: buffer length is too small");

    const uint8_t* buffer_pointer = reinterpret_cast<const uint8_t*>(buffer);

    const struct pcapng_block_head* pcapng_head = 
        reinterpret_cast<const pcapng_block_head*>(buffer_pointer);
    type = pcapng_head->type;
    total_length = pcapng_head->total_length;
    buffer_pointer += sizeof(pcapng_block_head);

    read_impl(buffer_pointer);
    buffer_pointer += impl_length();

    option.resize(total_length - sizeof(pcapng_block_head) - impl_length() - sizeof(pcapng_block_tail));
    memcpy(option.data(), buffer_pointer, option.size());
    buffer_pointer += option.size();

    const struct pcapng_block_tail* pcapng_tail = 
        reinterpret_cast<const pcapng_block_tail*>(buffer_pointer);
    uint32_t total_length2   = pcapng_tail->total_length;
    if (total_length2 != total_length) 
        throw pgen::exception("pgen::pcapng_block::read: totlen1 != totlen2");
    buffer_pointer += sizeof(struct pcapng_block_tail);

    if (buffer_pointer - reinterpret_cast<const uint8_t*>(buffer) != total_length) 
        throw pgen::exception("pgen::pcapng_block::read: length error");
}


void pcapng_block::write(void* buffer, size_t bufferlen) const {
    if (bufferlen < sizeof(pcapng_block_head) + impl_length() + sizeof(pcapng_block_tail)) 
        throw pgen::exception("pgen::pcapng_SHB::write: buffer length is too small");
    if (bufferlen < total_length) 
        throw pgen::exception("pgen::pcapng_SHB::write: buffer length is too small");

    uint8_t* buffer_pointer = reinterpret_cast<uint8_t*>(buffer);

    struct pcapng_block_head* pcapng_head = 
        reinterpret_cast<pcapng_block_head*>(buffer_pointer);
    pcapng_head->type = type;
    pcapng_head->total_length = total_length;
    buffer_pointer += sizeof(pcapng_block_head);

    write_impl(buffer_pointer);
    buffer_pointer += impl_length();

    memcpy(buffer_pointer, option.data(), option.size());
    buffer_pointer += option.size();

    struct pcapng_block_tail* pcapng_tail = reinterpret_cast<pcapng_block_tail*>(buffer_pointer);
    pcapng_tail->total_length   = total_length;
    buffer_pointer += sizeof(struct pcapng_block_tail);

    if (buffer_pointer - reinterpret_cast<uint8_t*>(buffer) != total_length) {
        // ERASE
        // printf("bp - b: %zd \n", buffer_pointer - reinterpret_cast<uint8_t*>(buffer));
        // printf("total len : %zd \n", total_length);
        throw pgen::exception("pgen::pcapng_block::write: length error");
    }
}





struct shb_struct {
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    uint32_t section_length[2];
};

pcapng_SHB::pcapng_SHB() {
    type = pgen::pcapng_type::SHB;
    total_length = 
        sizeof(struct pcapng_block_head) +
        sizeof(struct shb_struct) + 
        option.size() + 
        sizeof(struct pcapng_block_tail);
    magic = 0x1a2b3c4d;
    version_major = 0x0001;
    version_minor = 0x0000;
    section_length[0] = 0xffffffff;
    section_length[1] = 0xffffffff;
}

void pcapng_SHB::summary(bool moreinfo) {
    printf("Section Header Block \n");
    if (moreinfo) {
        printf("magic : %04x \n", magic);
        printf("version_minor: %04x \n", version_minor);
        printf("version_major: %04x \n", version_major);
        printf("section_length: 0x%04x%04x \n", 
                section_length[0],section_length[1]);
        printf("option: \n");
        pgen::hex(option.data(), option.size());
    }
}

size_t pcapng_SHB::impl_length() const {
    return sizeof(struct shb_struct);
}

void pcapng_SHB::read_impl(const void* buffer) {
    const struct shb_struct* shb = 
        reinterpret_cast<const shb_struct*>(buffer);

    magic             = shb->magic         ;
    version_major     = shb->version_major ;
    version_minor     = shb->version_minor ;
    section_length[0] = shb->section_length[0];
    section_length[1] = shb->section_length[1];

    if (type != pgen::pcapng_type::SHB)
        throw pgen::exception("pgen::pcapng_SHB::read: this is not SHB");

}

void pcapng_SHB::write_impl(void* buffer) const {

    struct shb_struct* shb = reinterpret_cast<shb_struct*>(buffer);
    shb->magic             = magic;
    shb->version_major     = version_major;
    shb->version_minor     = version_minor;
    shb->section_length[0] = section_length[0];
    shb->section_length[1] = section_length[1];

}









//
// struct idb_struct {
//     uint32_t type;
//     uint32_t total_length;
//     uint16_t link_type;
//     uint16_t reserved;
//     uint32_t snap_length;
// };
//
// pcapng_IDB::pcapng_IDB() {
//     type = pgen::pcapng_type::IDB;
//     total_length = option.size() + 
//         sizeof(idb_struct) + sizeof(pcapng_block_tail);
//     link_type = 1 ; // TODO this is hard coding 1:ethernet, 9:ppp
//     reserved = 0; // TODO I'v not gathered information about reserved 
//     snap_length = 0; // TODO I'v not ry) but being 0 is OK, not error
// }
//
// void pcapng_IDB::summary(bool moreinfo) {
//     printf("Interface Description Block \n");
//     if (moreinfo) {
//         printf("link type: %02x \n", link_type);
//         printf("reserved : %02x \n", reserved);
//         printf("snap length: %04x \n", snap_length);
//         printf("option: \n");
//         pgen::hex(option.data(), option.size());
//     }
// }
//
// void pcapng_IDB::read(const void* buffer, size_t bufferlen) {
//     if (bufferlen < sizeof(struct idb_struct)+sizeof(struct pcapng_block_tail)) 
//         throw pgen::exception("pgen::pcapng_IDB::read: buffer length is too small");
//
//     const uint8_t* buffer_pointer = reinterpret_cast<const uint8_t*>(buffer);
//
//     const struct idb_struct* idb =
//         reinterpret_cast<const idb_struct*>(buffer_pointer);
//     type         = idb->type;
//     total_length = idb->total_length;
//     link_type    = idb->link_type;
//     reserved     = idb->reserved;
//     snap_length  = idb->snap_length;
//     buffer_pointer += sizeof(struct idb_struct);
//     if (type != pgen::pcapng_type::IDB)
//         throw pgen::exception("pgen::pcapng_IDB::read: this is not IDB");
//
//     read_opt_from(buffer_pointer, 
//             total_length - sizeof(shb_struct) - sizeof(pcapng_block_tail));
//     buffer_pointer += option.size();
//
//     read_blocktail(buffer_pointer);
//     buffer_pointer += sizeof(struct pcapng_block_tail);
//
//     if (buffer_pointer - reinterpret_cast<const uint8_t*>(buffer) != total_length) 
//         throw pgen::exception("pgen::pcapng_IDB::read: length error");
// }
//
// void pcapng_IDB::write(void* buffer, size_t bufferlen) const {
//     if (bufferlen < sizeof(struct idb_struct)+sizeof(struct pcapng_block_tail)) 
//         throw pgen::exception("pgen::pcapng_IDB::write: buffer length is too small");
//
//     uint8_t* buffer_pointer = reinterpret_cast<uint8_t*>(buffer);
//
//     struct idb_struct* idb = reinterpret_cast<idb_struct*>(buffer_pointer);
//     idb->type         = type        ;
//     idb->total_length = total_length;
//     idb->link_type    = link_type   ;
//     idb->reserved     = reserved    ;
//     idb->snap_length  = snap_length ;
//     buffer_pointer += sizeof(struct idb_struct);
//
//     memcpy(buffer_pointer, option.data(), option.size());
//     buffer_pointer += option.size();
//
//     write_blocktail(buffer_pointer);
//     buffer_pointer += sizeof(struct pcapng_block_tail);
//
//     if (buffer_pointer - reinterpret_cast<const uint8_t*>(buffer) != total_length) 
//         throw pgen::exception("pgen::pcapng_IDB::write: length error");
// }
//
//
//





} /* namespace pgen */
