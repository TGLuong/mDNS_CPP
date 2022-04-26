#include "mdnssub.h"

static void mdns::MDnsSub::DomainCallback_
(
    DNSServiceRef       sd_ref, 
    DNSServiceFlags     flags, 
    uint32_t            interface_index, 
    DNSServiceErrorType error_code, 
    const char          *reply_domain, 
    void                *context
) 
{
    
}

static void mdns::MDnsSub::ServiceCallback_
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

static void mdns::MDnsSub::RecordCallback_
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

mdns::MDnsSub::MDnsSub
(
    std::string name, 
    std::string regist_type, 
    std::string domain, 
    DNSServiceFlags flags, 
    uint32_t interface_index
) 
{
    this->name_ = name;
    this->regist_type_ = regist_type;
    this->domain_ = domain;
    this->flags_ = flags;
    this-> interface_index_ = interface_index;
}

int
mdns::MDnsSub::ScanDomain(void callback()) {

}

int
mdns::MDnsSub::ScanService(void callback()) {

}

int
mdns::MDnsSub::ScanRecord(void callback()) {

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
mdns::MDnsSub::set_flags(DNSServiceFlags flags) {
    this->flags_ = flags;
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

DNSServiceFlags
mdns::MDnsSub::get_flags() {
    return this->flags_;
}

uint32_t
mdns::MDnsSub::get_interface_index() {
    return this->interface_index_;
}
