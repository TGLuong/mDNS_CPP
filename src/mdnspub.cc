#include "mdnspub.h"

#include <arpa/inet.h>
#include <dns_sd.h>
#include <unistd.h>

#include <iostream>



//======================== PUBLIC ===========================

mdns::MDnsPub::MDnsPub
(
    std::string     name, 
    std::string     register_type, 
    std::string     domain,
    uint16_t        port, 
    uint32_t        interface_index
)   
{
    this->name_             = name;
    this->register_type_    = register_type;
    this->domain_           = domain;
    this->port_             = htons(port);
    this->interface_index_  = interface_index;
}

mdns::MDnsPub::MDnsPub
(
    std::string     name,
    std::string     register_type,
    uint16_t        port,
    uint32_t        interface_index
)
{
    this->name_             = name;
    this->register_type_    = register_type;
    this->domain_           = "local.";
    this->port_             = htons(port);
    this->interface_index_  = interface_index;
}

mdns::MDnsPub::MDnsPub
(
    std::string     name,
    std::string     register_type,
    uint16_t        port
)
{
    this->name_             = name;
    this->register_type_    = register_type;
    this->domain_           = "local.";
    this->port_             = htons(port);
    this->interface_index_  = 0;
}

mdns::MDnsPub::MDnsPub
(
    std::string     name,
    std::string     register_type
)
{
    this->name_             = name;
    this->register_type_    = register_type;
    this->domain_           = "local.";
    this->port_             = htons(5000);
    this->interface_index_  = 0;
}

mdns::MDnsPub::~MDnsPub() {
    DestroyRecord_();
    Unregister();
}

int 
mdns::MDnsPub::AddRecordValue
(
    std::string key, 
    std::string value
) 
{
    if (is_init_txt_ == 0) {
        InitRecord_();
        is_init_txt_ = 1;
    }

    int status;
    status = TXTRecordSetValue(
        &this->txt_record_, key.data(), 
        value.size(), 
        value.data()
    );

    if (status != kDNSServiceErr_NoError) {
        return status;
    }
    
    if (this->is_registered_ == 1) this->UpdateRecord_();

    return 0;
}

int
mdns::MDnsPub::AddRecordValue
(
    std::string key, 
    std::string value, 
    uint32_t time_to_live
) 
{
    if (is_init_txt_ == 0) {
        InitRecord_();
        is_init_txt_ = 1;
    }

    int status;
    status = TXTRecordSetValue(
        &this->txt_record_, 
        key.data(), 
        value.size(), 
        value.data()
    );

    if (status != kDNSServiceErr_NoError) {
        return status;
    }

    if (this->is_registered_ == 1) 
        this->UpdateRecord_(time_to_live);

    return 0;
}

int 
mdns::MDnsPub::AddMapRecordValue
(
    std::map<std::string, std::string> map_record_value
) 
{
    if (is_init_txt_ == 0) {
        InitRecord_();
        is_init_txt_ = 1;
    }

    int status;
    for (const auto &item : map_record_value) {
        status = TXTRecordSetValue(
            &this->txt_record_, 
            item.first.data(), 
            item.second.size(), 
            item.second.data()
        );
        if (status != kDNSServiceErr_NoError) return status;
    }

    if (this->is_registered_ == 1) this->UpdateRecord_();

    return 0;
}

int 
mdns::MDnsPub::AddMapRecordValue
(
    std::map<std::string, std::string> map_record_value, 
    uint32_t time_to_live
)
{
    if (is_init_txt_ == 0) {
        InitRecord_();
        is_init_txt_ = 1;
    }

    int status;
    for (const auto &item : map_record_value) {
        status = TXTRecordSetValue(
            &this->txt_record_, 
            item.first.data(), 
            item.second.size(), 
            item.second.data()
        );
        if (status != kDNSServiceErr_NoError) return status;
    }

    if (this->is_registered_ == 1) this->UpdateRecord_(time_to_live);
    
    return 0;
}

int 
mdns::MDnsPub::RemoveRecordValue(std::string key) {
    return TXTRecordRemoveValue(&this->txt_record_, key.data());
}

int
mdns::MDnsPub::Register() {
    int status;

    this->is_registered_ = 0;

    while (1) {
        status = DNSServiceRegister(
            &sd_ref_, 
            flags_, 
            interface_index_, 
            name_.data(), 
            register_type_.data(), 
            domain_.data(), 
            NULL, 
            port_, 
            TXTRecordGetLength(&txt_record_), 
            TXTRecordGetBytesPtr(&txt_record_), 
            NULL, 
            NULL
        );

        if (status != 0) {
            printf("ERROR, retry register!\n");
            sleep(1);
        } else {
            break;
        }
    }

    if (status == 0) this->is_registered_ = 1;

    return status;            
}

void
mdns::MDnsPub::Unregister() {
    DNSServiceRefDeallocate(sd_ref_);
}

//========================= PUBLIC - SETTER ==============================

void
mdns::MDnsPub::set_name(std::string name) {
    this->name_ = name;
}

void
mdns::MDnsPub::set_register_type(std::string register_type) {
    this->register_type_ = register_type;
}

void
mdns::MDnsPub::set_domain(std::string domain) {
    this->domain_ = domain;
}

void 
mdns::MDnsPub::set_port(uint16_t port) {
    this->port_ = htons(port);
}

void 
mdns::MDnsPub::set_interface_index(uint32_t interface_index) {
    this->interface_index_ = interface_index;
}

void
mdns::MDnsPub::set_flags(DNSServiceFlags flags) {
    this->flags_ = flags;
}

//======================= PUBLIC - GETTER =========================

std::string
mdns::MDnsPub::get_name() {
    return this->name_;
}

std::string
mdns::MDnsPub::get_register_type() {
    return this->register_type_;
}

std::string 
mdns::MDnsPub::get_domain() {
    return this->domain_;
}

uint16_t
mdns::MDnsPub::get_port() {
    return ntohs(port_);
}

uint32_t
mdns::MDnsPub::get_interface_index() {
    return this->interface_index_;
}

DNSServiceFlags
mdns::MDnsPub::get_flags() {
    return this->flags_;
}

std::string
mdns::MDnsPub::get_txt_record() {
    txt_buffer_[TXTRecordGetLength(&txt_record_)] = 0;
    return txt_buffer_;
}

//======================================== PRIVATE ====================================================

int
mdns::MDnsPub::UpdateRecord_() {
    int status;
    
    int daemon_is_running = this->CheckDaemonIsRunning();

    if ( daemon_is_running == 0 ) {
        this->Register();
    }

    status =  DNSServiceUpdateRecord(
        sd_ref_, 
        NULL, 
        0, 
        TXTRecordGetLength(&txt_record_), 
        TXTRecordGetBytesPtr(&txt_record_), 
        this->time_to_live
    );

    return 0;
}

int
mdns::MDnsPub::UpdateRecord_(int time_to_live) {
    int daemon_is_running = this->CheckDaemonIsRunning();
    
    if ( daemon_is_running == 0 ) {
        this->Register();
    }
    
    int status = DNSServiceUpdateRecord(
        sd_ref_, 
        NULL, 
        0, 
        TXTRecordGetLength(&txt_record_), 
        TXTRecordGetBytesPtr(&txt_record_), 
        time_to_live
    );

    return 0;
}

void 
mdns::MDnsPub::InitRecord_() {
    TXTRecordCreate(
        &this->txt_record_, 
        sizeof(this->txt_buffer_), 
        this->txt_buffer_
    );
}

void
mdns::MDnsPub::DestroyRecord_() {
    DNSServiceRemoveRecord(
        sd_ref_, 
        record_ref_, 
        0
    );
    TXTRecordDeallocate(&txt_record_);
}

int
mdns::MDnsPub::CheckDaemonIsRunning() {
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