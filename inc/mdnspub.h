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
            std::string register_type_;
            std::string domain_;

            uint32_t    interface_index_;
            uint32_t    time_to_live = 120;
            uint16_t    port_;
            uint8_t     is_init_txt_ = 0;
            uint8_t     is_registered_ = 0;
            
            char txt_buffer_[65535];

            int UpdateRecord_();
            
            int UpdateRecord_(int time_to_live);

            void InitRecord_();

            void DestroyRecord_();

        public:

            MDnsPub(
                std::string     name, 
                std::string     register_type, 
                std::string     domain, 
                uint16_t        port, 
                uint32_t        interface_index
            );

            MDnsPub(
                std::string     name,
                std::string     register_type,
                uint16_t        port,
                uint32_t        interface_index
            );

            MDnsPub(
                std::string     name,
                std::string     register_type,
                uint16_t        port
            );

            MDnsPub(
                std::string     name,
                std::string     register_type
            );

            ~MDnsPub();

            int AddRecordValue(std::string key, std::string value);

            int AddRecordValue(std::string key, std::string value, uint32_t time_to_live);

            int AddMapRecordValue(std::map<std::string, std::string> map_record_values);

            int AddMapRecordValue(std::map<std::string, std::string> map_record_values, uint32_t time_to_live);

            int RemoveRecordValue(std::string key);

            int Register();

            void Unregister();

            // getter

            std::string get_name();

            std::string get_register_type();

            std::string get_domain();

            uint16_t get_port();

            uint32_t get_interface_index();

            DNSServiceFlags get_flags();

            std::string get_txt_record();

            // setter

            void set_name(std::string name);

            void set_register_type(std::string regist_type);

            void set_domain(std::string domain);

            void set_port(uint16_t port);

            void set_interface_index(uint32_t interface_index);

            void set_flags(DNSServiceFlags flags);
            
    };
}

#endif // MDNS_MDNSPUB_H
