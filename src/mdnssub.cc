#include "mdnssub.h"
#include <dns_sd.h>
#include <string.h>

#include <list>
#include <map>
#include <string>
#include <thread>
#include <algorithm>


typedef void (*domain_callback_def)(std::string);

struct domain_callback_store {
    domain_callback_def call;
    std::list<std::string> *domain_list;
};

typedef void (*service_callback_def)(std::string);

struct service_callback_store {
    service_callback_def OnAddService;
    service_callback_def OnRemoveService;
    std::list<std::string> *service_list;
};

typedef void (*record_callback_def)(std::string);

struct record_callback_store {
    record_callback_def OnAddRecord;
    record_callback_def OnRemoveRecord;
    std::map<std::string> record_map;
}

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
    struct domain_callback_store *store = (struct domain_callback_store *) context;
    store->domain_list->push_back(reply_domain);
    store->call(reply_domain);
}

void
mdns::MDnsSub::RequestStopDomain_() {
    if (this->domain_loop_ != NULL) { // if thread in runing, free it
        this->is_domain_loop_ = 0;
        this->domain_loop_->join();
        delete this->domain_loop_;
    }
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
    void                *context
) 
{
    struct service_callback_store *store = (struct service_callback_store *) context;
    int buffer_len = strlen(service_name) + strlen(regist_type) + strlen(reply_domain) + 10;
    char buffer[buffer_len];
    DNSServiceConstructFullName(buffer, service_name, regist_type, reply_domain);
    if (flags == kDNSServiceFlagsAdd) {
        store->service_list->push_back(buffer);
        store->OnAddService(buffer);
    } else {
        store->service_list->remove(buffer);
        store->OnRemoveService(buffer);
    }
}

void
mdns::MDnsSub::RequestStopService_() {
    if (this->service_loop_ != NULL) { // if thread in runing, free it
        this->is_service_loop_ = 0;
        this->service_loop_->join();
        delete this->service_loop_;
    }
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
    struct record_callback_store *store = (struct record_callback_store *) contex;
    
}

void
mdns::MDnsSub::RequestStopRecord_() {
    if (this->record_loop_ != NULL) {
        this->is_record_loop_ = 0;
        this->record_loop_->join();
        delete this->record_loop_;
    }
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

mdns::MDnsSub::~MDnsSub() { 
    this->RequestStopDomain_();
    this->RequestStopService_();
    this->RequestStopRecord_();
}

int
mdns::MDnsSub::ScanDomain(void callback(std::string)) {
    this->domain_loop_ = new std::thread([this, callback] () -> void {
        struct domain_callback_store store;
        store.call = callback;
        store.domain_list = &this->domain_list_;
        
        DNSServiceEnumerateDomains(
            &sd_ref_domain_, 
            kDNSServiceFlagsBrowseDomains, 
            0, 
            DomainCallback_, 
            &store
        );

        this->is_domain_loop_ = 1;
        while (this->is_domain_loop_) DNSServiceProcessResult(this->sd_ref_domain_);
    });
    return 0;
}

int
mdns::MDnsSub::ScanService(void OnAddService(std::string), void OnRemoveService(std::string)) {
    this->service_loop_ = new std::thread([this, OnAddService, OnRemoveService] {
        struct service_callback_store store;
        store.OnAddService = OnAddService;
        store.OnRemoveService = OnRemoveService;
        store.service_list = &this->service_list_;

        DNSServiceBrowse(
            &this->sd_ref_service_, 
            0, 
            0, 
            this->regist_type_.data(),
            this->domain_.data(),
            this->ServiceCallback_, 
            &store
        );

        this->is_service_loop_ = 1;
        while (this->is_service_loop_) DNSServiceProcessResult(this->sd_ref_service_);
    });
    return 0;
}

int
mdns::MDnsSub::ScanRecord(void OnAddRecord(std::string), void OnRemoveRecord(std::string)) {
    this->record_loop_ = new std::thread([this, OnAddRecord, OnRemoveRecord] () {
        struct record_callback_store store;
        int fullname_len = strlen(service_name) + strlen(regist_type) + strlen(reply_domain) + 10;
        char fullname[fullname_len];
        DNSServiceConstructFullName(
            fullname, 
            this->name_.data(), 
            this->regist_type_.data(), 
            this->domain_.data()
        );

        store.OnAddRecord = OnAddRecord;
        store.OnRemoveRecord = OnRemoveRecord;
        store.record_map = this->record_map_;

        DNSServiceQueryRecord(
            this->sd_ref_record_,
            0,
            this->interface_index_,
            fullname,
            kDNSServiceType_TXT,
            kDNSServiceClass_IN,
            this->RecordCallback_,
            &store
        );

        this->is_record_loop_ = 1;
        while (this->is_record_loop_) DNSServiceProcessResult(this->sd_ref_record_);
    });
    return 0;
}

void
mdns::MDnsSub::Listen() {
    
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

long
mdns::MDnsSub::get_domain_list_size() {
    return this->domain_list_.size();
}

std::string
mdns::MDnsSub::get_domain_list_at(int i) {
    if (i < this->domain_list_.size()) {
        std::list<std::string>::iterator it = this->domain_list_.begin();
        std::advance(it, i);
        return *it;
    }
    return "";
}

long
mdns::MDnsSub::get_service_list_size() {
    return this->service_list_.size();
}
