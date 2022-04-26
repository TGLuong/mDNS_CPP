#ifndef MDNS_MDNSSUB_H
#define MDNS_MDNSSUB_H

#include <dns_sd.h>
#include <stdint.h>

#include <string>


namespace mdns {
    class MDnsSub {
        private:
            DNSServiceRef sd_ref_;
            DNSServiceFlags flags_;
            std::string name_;
            std::string regist_type_;
            std::string domain_;
            uint32_t interface_index_;

            static void DomainCallback_(
                DNSServiceRef       sd_ref, 
                DNSServiceFlags     flags, 
                uint32_t            interface_index, 
                DNSServiceErrorType error_code, 
                const char          *reply_domain, 
                void                *context
            );
            static void ServiceCallback_(
                DNSServiceRef       sd_ref, 
                DNSServiceFlags     flags, 
                uint32_t            interface_index, 
                DNSServiceErrorType error_code, 
                const char          *service_name, 
                const char          *regist_type, 
                const char          *reply_domain, 
                char                *context
            );
            static void RecordCallback_(
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
            );

        public:
            MDnsSub(std::string name, std::string regist_type, std::string domain, DNSServiceFlags flags, uint32_t interface_index);
            ~MDnsSub();
            
            int ScanDomain(void callback());
            int ScanService(void callback());
            int ScanRecord(void callback());

            // setter
            void set_name(std::string name);
            void set_regist_type(std::string regist_type);
            void set_domain(std::string domain);
            void set_flags(DNSServiceFlags flags);
            void set_interface_index(uint32_t interface_index);

            // getter
            std::string get_name();
            std::string get_regist_type();
            std::string get_domain();
            DNSServiceFlags get_flags();
            uint32_t get_interface_index();
    };
}
#endif // MDNS_MDNSSUB_H
