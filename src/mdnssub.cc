#include "mdnssub.h"

#include <arpa/inet.h>
#include <dns_sd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <thread>

//======================== STRUCT DEFINITION ===========================

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

typedef void (*record_callback_def)(std::map<std::string, std::string>);

struct record_callback_store {
    record_callback_def OnAddRecord;
    record_callback_def OnRemoveRecord;
    std::map<std::string, std::string> *record_map;
};

//========================= PUBLIC ==============================

mdns::MDnsSub::MDnsSub
(
    std::string name, 
    std::string register_type, 
    std::string domain, 
    uint32_t interface_index
) 
{
    this->name_ = name;
    this->register_type_ = register_type;
    this->domain_ = domain;
    this-> interface_index_ = interface_index;
}

mdns::MDnsSub::MDnsSub
(
    std::string name,
    std::string register_type
)
{
    this->name_             = name;
    this->register_type_    = register_type;
    this->domain_           = "local.";
    this->interface_index_  = 0;
}

mdns::MDnsSub::~MDnsSub() { 
    this->RequestStopDomain();
    this->RequestStopService();
    this->RequestStopRecord();
}

int
mdns::MDnsSub::ScanDomain
(
    void callback(std::string)
)
{
    this->domain_thread_ = new std::thread([this, callback] () -> void {
        struct domain_callback_store store;
        store.call = callback;
        store.domain_list = &this->domain_list_;
        
        this->TryStartScanDomain(&store);

        this->is_domain_loop_ = 1;
        while (this->is_domain_loop_) {
            if (this->CheckDaemonIsRunning() == 0) this->TryStartScanDomain(&store);
            DNSServiceProcessResult(this->sd_ref_domain_);
        }
    });
    
    return 0;
}

int
mdns::MDnsSub::ScanService
(
    void OnAddService(std::string), 
    void OnRemoveService(std::string)
)
{
    this->service_thread_ = new std::thread([this, OnAddService, OnRemoveService] {
        struct service_callback_store store;
        store.OnAddService = OnAddService;
        store.OnRemoveService = OnRemoveService;
        store.service_list = &this->service_list_;

        this->TryStartScanService(&store);

        this->is_service_loop_ = 1;
        while (this->is_service_loop_) {
            if (this->CheckDaemonIsRunning() == 0) this->TryStartScanService(&store);
            DNSServiceProcessResult(this->sd_ref_service_);
        }
    });

    return 0;
}

int
mdns::MDnsSub::ScanRecord
(
    void OnAddRecord(std::map<std::string, std::string>), 
    void OnRemoveRecord(std::map<std::string, std::string>)
)
{
    this->record_thread_ = new std::thread([this, OnAddRecord, OnRemoveRecord] () {
        struct record_callback_store store;
        int fullname_length =   strlen(this->name_.data()) + 
                                strlen(this->register_type_.data()) + 
                                strlen(this->domain_.data()) + 10;

        char fullname[fullname_length];

        DNSServiceConstructFullName(
            fullname, 
            this->name_.data(), 
            this->register_type_.data(), 
            this->domain_.data()
        );

        store.OnAddRecord = OnAddRecord;
        store.OnRemoveRecord = OnRemoveRecord;
        store.record_map = &this->record_map_;

        this->TryStartScanQuery(fullname, &store);

        this->is_record_loop_ = 1;
        while (this->is_record_loop_) {
            if (this->CheckDaemonIsRunning() == 0) this->TryStartScanQuery(fullname, &store);
            DNSServiceProcessResult(this->sd_ref_record_);
        }
    });

    return 0;
}

//======================== PUBLIC - SETTER ==========================

void
mdns::MDnsSub::set_name(std::string name) {
    this->name_ = name;
}

void
mdns::MDnsSub::set_register_type(std::string register_type) {
    this->register_type_ = register_type;
}

void
mdns::MDnsSub::set_domain(std::string domain) {
    this->domain_ = domain;
}

void
mdns::MDnsSub::set_interface_index(uint32_t interface_index) {
    this->interface_index_ = interface_index;
}

//======================= PUCLIC - GETTER ==============================

std::string
mdns::MDnsSub::GetHostname() {
    DNSServiceRef sd_ref;
    std::string scan_result;
    int status = DNSServiceResolve(
        &sd_ref,
        kDNSServiceFlagsForceMulticast,
        this->interface_index_,
        this->name_.data(),
        this->register_type_.data(),
        this->domain_.data(),
        this->GetHostnameCallback_,
        &scan_result
    );

    printf("status: %d\n", status);

    DNSServiceProcessResult(sd_ref);

    return scan_result;
}

std::string
mdns::MDnsSub::GetIpAddress() {
    DNSServiceRef sd_ref;
    std::string ip_address;

    DNSServiceGetAddrInfo(
        &sd_ref,
        kDNSServiceFlagsForceMulticast,
        this->interface_index_,
        kDNSServiceProtocol_IPv4,
        this->GetHostname().data(),
        this->GetIpCallback_,
        &ip_address
    );

    DNSServiceProcessResult(sd_ref);

    return ip_address;
}

std::string
mdns::MDnsSub::get_name() {
    return this->name_;
}

std::string
mdns::MDnsSub::get_register_type() {
    return this->register_type_;
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

std::string
mdns::MDnsSub::get_service_list_at(int i) {
    if (i < this->service_list_.size()) {
        std::list<std::string>::iterator it = this->service_list_.begin();
        std::advance(it, i);
        return *it;
    }
    return "";
}

long
mdns::MDnsSub::get_record_map_size() {
    return this->record_map_.size();
}

std::string
mdns::MDnsSub::get_record_by_key(std::string key) {
    std::map<std::string, std::string>::iterator itr = this->record_map_.find(key);
    if (itr != this->record_map_.end()) {
        return itr->second;
    } else {
        return "";
    }
}

//======================= PRIVATE =========================

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
mdns::MDnsSub::RequestStopDomain() {
    if (this->domain_thread_ != NULL) { // if thread in runing, free it
        this->is_domain_loop_ = 0;
        this->domain_thread_->join();
        delete this->domain_thread_;
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
    const char          *register_type, 
    const char          *reply_domain, 
    void                *context
) 
{
    struct service_callback_store *store = (struct service_callback_store *) context;
    int buffer_len = strlen(service_name) + strlen(register_type) + strlen(reply_domain) + 10;
    char buffer[buffer_len];
    DNSServiceConstructFullName(buffer, service_name, register_type, reply_domain);
    if (flags == kDNSServiceFlagsAdd) {
        store->service_list->push_back(buffer);
        store->OnAddService(buffer);
    } else {
        store->service_list->remove(buffer);
        store->OnRemoveService(buffer);
    }
}

void
mdns::MDnsSub::RequestStopService() {
    if (this->service_thread_ != NULL) { // if thread in runing, free it
        this->is_service_loop_ = 0;
        this->service_thread_->join();
        delete this->service_thread_;
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
    uint16_t            register_record_type,
    uint16_t            register_record_class,
    uint16_t            record_data_length,
    const void          *record_data,
    uint32_t            time_to_live,
    void                *context
)
{
    struct record_callback_store *store = (struct record_callback_store *) context;
    char key[10000];
    char value[10000];
    const void *value_pointer;
    uint8_t value_length, record_count;
    std::map<std::string, std::string> record_map;

    record_count = TXTRecordGetCount(record_data_length, record_data);

    for (int i = 0; i < record_count; i++) {
        memset(key, 0, sizeof(key));
        memset(value, 0 , sizeof(value));
        TXTRecordGetItemAtIndex(
            record_data_length,
            record_data,
            i,
            100,
            key,
            &value_length,
            &value_pointer
        );

        memcpy(value, value_pointer, value_length);

        record_map.insert(std::pair<std::string, std::string>(key, value));

        if (flags == kDNSServiceFlagsAdd) {
            std::map<std::string, std::string>::iterator itr = store->record_map->find(key);
            if (itr != store->record_map->end()) {
                itr->second = value;
            } else {
                store->record_map->insert(std::pair<std::string, std::string>(key, value));
            }
        } else {
            store->record_map->erase(key);
        }
    }
    if (flags == kDNSServiceFlagsAdd) {
        store->OnAddRecord(record_map);
    } else {
        store->OnRemoveRecord(record_map);
    }
}

void
mdns::MDnsSub::RequestStopRecord() {
    if (this->record_thread_ != NULL) {
        this->is_record_loop_ = 0;
        this->record_thread_->join();
        delete this->record_thread_;
    }
}

void
mdns::MDnsSub::GetHostnameCallback_
(
    DNSServiceRef       sdRef,
    DNSServiceFlags     flags,
    uint32_t            interfaceIndex,
    DNSServiceErrorType errorCode,
    const char          *fullname,
    const char          *hosttarget,
    uint16_t            port,
    uint16_t            txt_length,
    const unsigned char *txt_record,
    void                *context
)
{
    std::string * scan_result = (std::string *) context;
    (*scan_result) = hosttarget;
}

void 
mdns::MDnsSub::GetIpCallback_
(
    DNSServiceRef           sdRef,
    DNSServiceFlags         flags,
    uint32_t                interfaceIndex,
    DNSServiceErrorType     errorCode,
    const char              *hostname,
    const struct sockaddr   *address,
    uint32_t                time_to_live,
    void                    *context
)
{
    std::string * ip_address = (std::string *) context;
    struct sockaddr_in * addr_in = (struct sockaddr_in *) address;
    (* ip_address) = inet_ntoa(addr_in->sin_addr);
}

void
mdns::MDnsSub::TryStartScanQuery
(
    std::string fullname,
    void        *store
)
{
    int status;
    while (1) {
        status = DNSServiceQueryRecord(
            &this->sd_ref_record_,
            kDNSServiceFlagsForceMulticast,
            this->interface_index_,
            fullname.data(),
            kDNSServiceType_TXT,
            kDNSServiceClass_IN,
            this->RecordCallback_,
            store
        );

        if (status != kDNSServiceErr_NoError) {
            printf("ERROR: can't start scan query, retry scan!\n");
            sleep(1);
        } else {
            break;
        }
    }
}

void
mdns::MDnsSub::TryStartScanService
(
    void *store
)
{
    int status;
    while (1) {
        status = DNSServiceBrowse(
            &this->sd_ref_service_, 
            0, 
            this->interface_index_, 
            this->register_type_.data(),
            this->domain_.data(),
            this->ServiceCallback_, 
            store
        );
        if (status != kDNSServiceErr_NoError) {
            printf("ERROR: can't scan service, retry scan\n");
            sleep(1);
        } else {
            break;
        }
    }
}

void
mdns::MDnsSub::TryStartScanDomain
(
    void *store
)
{
    int status;
    while (1) {
        status = DNSServiceEnumerateDomains(
            &this->sd_ref_domain_, 
            kDNSServiceFlagsBrowseDomains, 
            this->interface_index_, 
            DomainCallback_, 
            store
        );
        if (status != kDNSServiceErr_NoError) {
            printf("ERROR: can't scan domain, retry scan\n");
            sleep(1);
        } else {
            break;
        }
    }
}

int
mdns::MDnsSub::CheckDaemonIsRunning() {
    uint32_t property; 
    uint32_t size;
    int status = DNSServiceGetProperty(
        kDNSServiceProperty_DaemonVersion, 
        &property, 
        &size
    );
    if (status == 0) {
        return 1;
    } else {
        return 0;
    }
}