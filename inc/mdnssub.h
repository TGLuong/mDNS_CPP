#ifndef MDNS_MDNSSUB_H
#define MDNS_MDNSSUB_H

#include <dns_sd.h>
#include <stdint.h>

#include <string>
#include <thread>
#include <list>
#include <map>


namespace mdns {
    class MDnsSub {
        private:
            std::string name_;
            std::string register_type_;
            std::string domain_;
            uint32_t interface_index_;

            DNSServiceRef sd_ref_domain_;
            std::list<std::string> domain_list_;
            std::thread *domain_thread_ = NULL;
            int is_domain_loop_ = 0;

            DNSServiceRef sd_ref_service_;
            std::list<std::string> service_list_;
            std::thread *service_thread_ = NULL;
            int is_service_loop_ = 0;

            DNSServiceRef sd_ref_record_;
            std::map<std::string, std::string> record_map_;
            std::thread *record_thread_ = NULL;
            int is_record_loop_ = 0;

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
                const char          *register_type, 
                const char          *reply_domain, 
                void                *context
            );

            static void RecordCallback_(
                DNSServiceRef       sd_ref,
                DNSServiceFlags     flags,
                uint32_t            interface_index,
                DNSServiceErrorType error_code,
                const char          *fullname,
                uint16_t            register_record_type,
                uint16_t            register_record_class,
                uint16_t            record_data_len,
                const void          *record_data,
                uint32_t            time_to_live,
                void                *context
            );

            static void GetHostnameCallback_(
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
            );

            static void GetIpCallback_(
                DNSServiceRef           sdRef,
                DNSServiceFlags         flags,
                uint32_t                interfaceIndex,
                DNSServiceErrorType     errorCode,
                const char              *hostname,
                const struct sockaddr   *address,
                uint32_t                time_to_live,
                void                    *context
            );

        public:
            MDnsSub(
                std::string name, 
                std::string register_type, 
                std::string domain, 
                uint32_t interface_index
            );

            ~MDnsSub();
            
            int ScanDomain(void callback(std::string));

            void RequestStopDomain();

            int ScanService(void OnAddService(std::string), void OnRemoveService(std::string));
            
            void RequestStopService();

            int ScanRecord(void OnAddRecord(std::map<std::string, std::string>), void OnRemoveRecord(std::map<std::string, std::string>));

            void RequestStopRecord();
            
            // setter

            void set_name(std::string name);

            void set_register_type(std::string regist_type);

            void set_domain(std::string domain);

            void set_interface_index(uint32_t interface_index);

            // getter

            std::string GetHostname();
        
            std::string GetIpAddress();

            std::string get_name();

            std::string get_register_type();

            std::string get_domain();

            uint32_t get_interface_index();

            long get_domain_list_size();

            std::string get_domain_list_at(int i);

            long get_service_list_size();

            std::string get_service_list_at(int i);

            long get_record_map_size();

            std::string get_record_by_key(std::string key);

    };
}
#endif // MDNS_MDNSSUB_H
