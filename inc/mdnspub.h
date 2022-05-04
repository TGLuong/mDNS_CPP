#ifndef MDNS_MDNSPUB_H
#define MDNS_MDNSPUB_H

#include <dns_sd.h>
#include <stdint.h>

#include <string>
#include <map>

namespace mdns {
    class MDnsPub {
        private:
            DNSServiceRef   sd_ref_;
            DNSRecordRef    record_ref_;
            DNSServiceFlags flags_ = 0;
            TXTRecordRef    txt_record_;

            std::string name_;
            std::string regist_type_;
            std::string domain_;

            uint32_t    interface_index_;
            uint32_t    ttl = 120;
            uint16_t    port_;
            uint8_t     is_init_txt = 0;
            
            char txt_buffer_[65535];

            int UpdateRecord_();
            
            int UpdateRecord_(int ttl);

            void InitRecord_();

            void Unregister_();

            void DestroyRecord_();

        public:
            MDnsPub(
                std::string     name, 
                std::string     regist_type, 
                std::string     domain, 
                uint16_t        port, 
                uint32_t        interface_index
            );

            ~MDnsPub();

            int AddRecordValue(std::string key, std::string value);

            int AddRecordValue(std::string key, std::string value, uint32_t ttl);

            int AddRecordValue(std::map<std::string, std::string> record_value);

            int AddRecordValue(std::map<std::string, std::string> record_value, uint32_t ttl);

            int RemoveRecordValue(std::string key);

            int Register();

            // getter

            std::string get_name();

            std::string get_regist_type();

            std::string get_domain();

            uint16_t get_port();

            uint32_t get_interface_index();

            DNSServiceFlags get_flags();

            std::string get_txt_record();

            // setter

            void set_name(std::string name);

            void set_regist_type(std::string regist_type);

            void set_domain(std::string domain);

            void set_port(uint16_t port);

            void set_interface_index(uint32_t interface_index);

            void set_flags(DNSServiceFlags flags);
            
    };
}

#endif // MDNS_MDNSPUB_H
