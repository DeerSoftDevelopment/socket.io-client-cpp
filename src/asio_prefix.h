#pragma once

#if SIO_TLS
#include <boost/asio/ssl/context.hpp>
#endif

#include <boost/asio/steady_timer.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/io_service.hpp>

namespace asio {
    using namespace boost::asio;
    
    // See note above about boost <1.49 compatibility
    #if (BOOST_VERSION/100000) == 1 && ((BOOST_VERSION/100)%1000) > 48
        // Using boost::asio >=1.49 so we use chrono and steady_timer
        template <typename T>
        bool is_neg(T duration) {
            return duration.count() < 0;
        }

        // If boost believes it has std::chrono available it will use it
        // so we should also use it for things that relate to boost, even
        // if the library would otherwise use boost::chrono.
        #if defined(BOOST_ASIO_HAS_STD_CHRONO)
            inline std::chrono::milliseconds milliseconds(long duration) {
                return std::chrono::milliseconds(duration);
            }
        #else
            inline lib::chrono::milliseconds milliseconds(long duration) {
                return lib::chrono::milliseconds(duration);
            }
        #endif
    #else
        // Using boost::asio <1.49 we pretend a deadline timer is a steady
        // timer and wrap the negative detection and duration conversion
        // appropriately.
        typedef boost::asio::deadline_timer steady_timer;
        
        template <typename T>
        bool is_neg(T duration) {
            return duration.is_negative();
        }
        inline boost::posix_time::time_duration milliseconds(long duration) {
            return boost::posix_time::milliseconds(duration);
        }
    #endif
    
    using boost::system::error_code;
    namespace errc = boost::system::errc;
} // namespace asio
