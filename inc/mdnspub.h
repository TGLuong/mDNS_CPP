#ifndef MDNS_MDNSPUB_H
#define MDNS_MDNSPUB_H

#include <dns_sd.h>
#include <string>
#include <stdint.h>

namespace mdns {
    class MDnsPub {
        private:
            DNSServiceRef sd_ref_;
            DNSRecordRef record_ref_;
            TXTRecordRef txt_record_;
            std::string name_;
            std::string regist_type_;
            std::string host_;
            std::string domain_;
            uint16_t port_;
            uint32_t interface_index_;
            DNSServiceFlags flags_;
            char txt_buffer_[65535];
            uint32_t ttl = 120;

        public:
            MDnsPub(DNSServiceFlags flags, std::string name, std::string regist_type, std::string domain, 
                    std::string host, uint16_t port, uint32_t interface_index);
            ~MDnsPub();
            void InitRecord();
            int AddRecordValue(std::string key, std::string value);
            int RemoveRecordValue(std::string key);
            int Regist();
            void Unregist();
            void DestroyRecord();
            int UpdateRecord();
            int UpdateRecord(int ttl);

            // getter
            std::string get_name();
            std::string get_regist_type();
            std::string get_host();
            std::string get_domain();
            uint16_t get_port();
            uint32_t get_interface_index();
            DNSServiceFlags get_flags();
            std::string get_txt_record();

            // setter
            void set_name(std::string name);
            void set_regist_type(std::string regist_type);
            void set_domain(std::string domain);
            void set_host(std::string host);
            void set_port(uint16_t port);
            void set_interface_index(uint32_t interface_index);
            void set_flags(DNSServiceFlags flags);
            
    };
}

#endif // MDNS_MDNSPUB_H