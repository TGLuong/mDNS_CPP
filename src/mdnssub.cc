#include "mdnssub.h"
#include <dns_sd.h>


typedef void (*callback_def)();

struct callback_store {
    callback_def call;
};

// private

void 
mdns::MDnsSub::DomainCallback_
(
    DNSServiceRef       sd_ref, 
    DNSServiceFlags     flags, 
    uint32_t            interface_index, 
    DNSServiceErrorType error_code, 
    const char          *reply_domain, 
    void                *context
) 
{
    printf("%s\n", reply_domain);
    struct callback_store *store = (struct callback_store *) context;
    store->call();
}

void 
mdns::MDnsSub::ServiceCallback_
(
    DNSServiceRef       sd_ref, 
    DNSServiceFlags     flags, 
    uint32_t            interface_index, 
    DNSServiceErrorType error_code, 
    const char          *service_name, 
    const char          *regist_type, 
    const char          *reply_domain, 
    char                *context
) 
{

}

void 
mdns::MDnsSub::RecordCallback_
(
    DNSServiceRef       sd_ref,
    DNSServiceFlags     flags,
    uint32_t            interface_index,
    DNSServiceErrorType error_code,
    const char          *fullname,
    uint16_t            regist_record_type,
    uint16_t            regist_record_class,
    uint16_t            record_data_len,
    const void          *rdata,
    uint32_t            ttl,
    void                *context
)
{

}

// public

mdns::MDnsSub::MDnsSub
(
    std::string name, 
    std::string regist_type, 
    std::string domain, 
    uint32_t interface_index
) 
{
    this->name_ = name;
    this->regist_type_ = regist_type;
    this->domain_ = domain;
    this-> interface_index_ = interface_index;
}

mdns::MDnsSub::~MDnsSub() { }

int
mdns::MDnsSub::ScanDomain(void callback()) {
    int status;
    struct  callback_store store;
    store.call = callback;
    status = DNSServiceEnumerateDomains(
        &sd_ref_domain_, 
        kDNSServiceFlagsBrowseDomains, 
        0, 
        DomainCallback_, 
        &store
    );
    while (1) DNSServiceProcessResult(sd_ref_domain_); 
    return 0;
}

int
mdns::MDnsSub::ScanService(void callback()) {
    return 0;
}

int
mdns::MDnsSub::ScanRecord(void callback()) {
    return 0;
}

// setter
void
mdns::MDnsSub::set_name(std::string name) {
    this->name_ = name;
}

void
mdns::MDnsSub::set_regist_type(std::string regist_type) {
    this->regist_type_ = regist_type;
}

void
mdns::MDnsSub::set_domain(std::string domain) {
    this->domain_ = domain;
}


void
mdns::MDnsSub::set_interface_index(uint32_t interface_index) {
    this->interface_index_ = interface_index;
}

// getter
std::string
mdns::MDnsSub::get_name() {
    return this->name_;
}

std::string
mdns::MDnsSub::get_regist_type() {
    return this->regist_type_;
}

std::string
mdns::MDnsSub::get_domain() {
    return this->domain_;
}

uint32_t
mdns::MDnsSub::get_interface_index() {
    return this->interface_index_;
}
