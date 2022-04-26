#include "mdnspub.h"

#include <arpa/inet.h>
#include <dns_sd.h>

#include <iostream>

// private

int
mdns::MDnsPub::UpdateRecord() {
    return DNSServiceUpdateRecord(sd_ref_, NULL, 0, TXTRecordGetLength(&txt_record_), TXTRecordGetBytesPtr(&txt_record_), ttl);
}

int
mdns::MDnsPub::UpdateRecord(int ttl) {
    return DNSServiceUpdateRecord(sd_ref_, NULL, 0, TXTRecordGetLength(&txt_record_), TXTRecordGetBytesPtr(&txt_record_), ttl);
}

// public

mdns::MDnsPub::MDnsPub
(
    DNSServiceFlags flags, 
    std::string     name, 
    std::string     regist_type, 
    std::string     domain,
    std::string     host, 
    uint16_t        port, 
    uint32_t        interface_index
)   
{
    this->flags_ = flags;
    this->name_ = name;
    this->regist_type_ = regist_type;
    this->domain_ = domain;
    this->host_ = host;
    this->port_ = htons(port);
    this->interface_index_ = interface_index;
}

mdns::MDnsPub::~MDnsPub() { }

void 
mdns::MDnsPub::InitRecord() {
    TXTRecordCreate(&this->txt_record_, sizeof(this->txt_buffer_), this->txt_buffer_);
}

int 
mdns::MDnsPub::AddRecordValue(std::string key, std::string value) {
    int status;
    status = TXTRecordSetValue(&this->txt_record_, key.data(), value.size(), value.data());
    if (status != kDNSServiceErr_NoError) {
        return status;
    }
    return this->UpdateRecord();
}

int
mdns::MDnsPub::AddRecordValue(std::string key, std::string value, uint32_t ttl) {
    int status;
    status = TXTRecordSetValue(&this->txt_record_, key.data(), value.size(), value.data());
    if (status != kDNSServiceErr_NoError) {
        return status;
    }
    return this->UpdateRecord(ttl);   
}

int 
mdns::MDnsPub::AddRecordValue(std::map<std::string, std::string> record_value) {
    int status;
    for (const auto &item : record_value) {
        status = TXTRecordSetValue(&this->txt_record_, item.first.data(), item.second.size(), item.second.data());
        if (status != kDNSServiceErr_NoError) return status;
    }
    return this->UpdateRecord();
}

int 
mdns::MDnsPub::AddRecordValue(std::map<std::string, std::string> record_value, uint32_t ttl) {
    int status;
    for (const auto &item : record_value) {
        status = TXTRecordSetValue(&this->txt_record_, item.first.data(), item.second.size(), item.second.data());
        if (status != kDNSServiceErr_NoError) return status;
    }
    return this->UpdateRecord(ttl);
}

int 
mdns::MDnsPub::RemoveRecordValue(std::string key) {

    return TXTRecordRemoveValue(&this->txt_record_, key.data());
}

int
mdns::MDnsPub::Register() {
    DNSServiceRegister(&sd_ref_, flags_, interface_index_, name_.data(), regist_type_.data(), 
                                domain_.data(), NULL, port_, TXTRecordGetLength(&txt_record_), 
                                TXTRecordGetBytesPtr(&txt_record_), NULL, NULL);
    return 0;            
}

void
mdns::MDnsPub::Unregister() {
    DNSServiceRefDeallocate(sd_ref_);
}

void
mdns::MDnsPub::DestroyRecord() {
    DNSServiceRemoveRecord(sd_ref_, record_ref_, 0);
    TXTRecordDeallocate(&txt_record_);
}

// settter

void
mdns::MDnsPub::set_name(std::string name) {
    this->name_ = name;
}

void
mdns::MDnsPub::set_regist_type(std::string regist_type) {
    this->regist_type_ = regist_type;
}

void
mdns::MDnsPub::set_domain(std::string domain) {
    this->domain_ = domain;
}

void
mdns::MDnsPub::set_host(std::string host) {
    this->host_ = host;
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

// getter

std::string
mdns::MDnsPub::get_name() {
    return this->name_;
}

std::string
mdns::MDnsPub::get_regist_type() {
    return this->regist_type_;
}

std::string
mdns::MDnsPub::get_host() {
    return this->host_;
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
