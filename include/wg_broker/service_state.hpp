#pragma once

#include <simppl/struct.h>

namespace wg_broker {


struct ServiceState {
    // <Serializer>
        typedef simppl::dbus::make_serializer<
            std::string,
            std::string,
            bool,
            bool,
            bool,
            std::string
        >::type serializer_type;
    // <Serializer/>

    std::string name;
    std::string content;
    
    bool is_enabled_on_startup;
    bool is_loaded;

    bool has_error;
    std::string error_content;
};


}   // namespace wg_broker