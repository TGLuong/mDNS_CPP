#ifndef MDNS_MDNSSUB_H
#define MDNS_MDNSSUB_H

#include <dns_sd.h>
#include <stdint.h>

#include <string>
#include <vector>
#include <thread>


namespace mdns {
    class MDnsSub {
        private:
            std::string name_;
            std::string regist_type_;
            std::string domain_;
            uint32_t interface_index_;

            DNSServiceRef sd_ref_domain_;
            std::vector<std::string> domain_vector_;
            std::thread *domain_loop_ = NULL;
            int is_domain_loop_ = 1;

            DNSServiceRef sd_ref_service_;
            std::vector<std::string> service_vector_;
            std::thread *service_loop_ = NULL;
            int is_service_loop_ = 1;

            static void DomainCallback_(
                DNSServiceRef       sd_ref, 
                DNSServiceFlags     flags, 
                uint32_t            interface_index, 
                DNSServiceErrorType error_code, 
                const char          *reply_domain, 
                void                *context
            );

            void RequestStopDomain_();

            static void ServiceCallback_(
                DNSServiceRef       sd_ref, 
                DNSServiceFlags     flags, 
                uint32_t            interface_index, 
                DNSServiceErrorType error_code, 
                const char          *service_name, 
                const char          *regist_type, 
                const char          *reply_domain, 
                void                *context
            );

            void RequestStopService_();

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
            MDnsSub(
                std::string name, 
                std::string regist_type, 
                std::string domain, 
                uint32_t interface_index
            );
            ~MDnsSub();
            
            int ScanDomain(void callback(std::string));

            int ScanService(void OnAddService(std::string), void OnRemoveService(std::string));

            int ScanRecord(void callback());

            void Listen();

            // setter

            void set_name(std::string name);

            void set_regist_type(std::string regist_type);

            void set_domain(std::string domain);

            void set_interface_index(uint32_t interface_index);

            // getter

            std::string get_name();

            std::string get_regist_type();

            std::string get_domain();

            uint32_t get_interface_index();

            long get_domain_vector_size();

            std::string get_domain_vector_at(int i);

            long get_service_vector_size();

    };
}
#endif // MDNS_MDNSSUB_H
