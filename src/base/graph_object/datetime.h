#pragma once

#include <cstdint>
#include <iomanip>

class DateTime {
public:
    static constexpr uint64_t INVALID_ID = UINT64_MAX;

    static uint64_t get_datetime_id(const char* str) {
        uint64_t ret = 0;
        char* ptr    = const_cast<char*>(str);

        int64_t year = strtoll(ptr, &ptr, 10);
        // Set sign bit
        if (year < 0) {
            ret |= 1ULL << 55;
            year *= -1;
        }
        // Handle big years with lower precision
        if (year > 9'999) {
            // Set precision bit
            ret |= 1ULL << 54;
            ret |= year / 100;
        } else {
            // Handle full date
            int64_t mon  = strtoull(ptr + 1, &ptr, 10);
            int64_t day  = strtoull(ptr + 1, &ptr, 10);
            int64_t hour = strtoull(ptr + 1, &ptr, 10);
            int64_t min  = strtoull(ptr + 1, &ptr, 10);
            int64_t sec  = strtoull(ptr + 1, &ptr, 10);
            if (sec > 59 || min > 59 || hour > 23 || day > 31 || mon > 12) {
                // invalid date
                return INVALID_ID;
            }
            // Skip fractional seconds
            if (*ptr == '.') {
                strtoull(ptr + 1, &ptr, 10);
            }
            // Handle timezone
            int64_t tz_sign = 0;
            int64_t tz_hour = 0;
            int64_t tz_min  = 0;
            if (*ptr != 'Z') {
                tz_hour = strtoull(ptr + 1, &ptr, 10);
                tz_min  = strtoull(ptr + 1, &ptr, 10);
                if (tz_hour < 0) {
                    tz_sign = 1;
                    tz_hour *= -1;
                }
                if (tz_min > 59 || tz_hour > 23) {
                    // invalid timezone
                    return INVALID_ID;
                }
            }
            // Set date
            uint64_t date = 0;
            date |= year;
            date = (date << 4) | std::max(mon, int64_t(1));
            date = (date << 5) | std::max(day, int64_t(1));
            date = (date << 5) | hour;
            date = (date << 6) | min;
            date = (date << 6) | sec;
            date = (date << 1) | tz_sign;
            date = (date << 5) | tz_hour;
            date = (date << 6) | tz_min;
            ret |= date;
        }
        return ret;
    }

    uint64_t id;

    DateTime(uint64_t _id) : id(_id) { }

    inline bool operator==(const DateTime& rhs) const noexcept {
        return this->id == rhs.id;
    }

    inline bool operator!=(const DateTime& rhs) const noexcept {
        return this->id != rhs.id;
    }

    std::string get_value_string() const {
        uint64_t datetime_id = id;
        std::stringstream ss;

         // Check sign bit
        if (datetime_id & (1ULL << 55)) {
            ss << '-';
        }
        // Check precision bit
        if (datetime_id & (1ULL << 54)) {
            // Remove sign and precision bits
            datetime_id &= ~(3ULL << 54);
            ss << datetime_id
                << "00-01-01T00:00:00Z";
        }
        else {
            uint64_t tz_min  = datetime_id & 0x3F;
            datetime_id >>= 6;
            uint64_t tz_hour = datetime_id & 0x1F;
            datetime_id >>= 5;
            uint64_t tz_sign = datetime_id & 0x1;
            datetime_id >>= 1;
            uint64_t sec =  datetime_id & 0x3F;
            datetime_id >>= 6;
            uint64_t min =  datetime_id & 0x3F;
            datetime_id >>= 6;
            uint64_t hour =  datetime_id & 0x1F;
            datetime_id >>= 5;
            uint64_t day =  datetime_id & 0x1F;
            datetime_id >>= 5;
            uint64_t mon =  datetime_id & 0xF;
            datetime_id >>= 4;
            uint64_t year =  datetime_id & 0xFFFF;

            ss << std::setfill('0')
               << std::setw(4) << year << '-'
               << std::setw(2) << mon  << '-'
               << std::setw(2) << day  << 'T'
               << std::setw(2) << hour << ':'
               << std::setw(2) << min  << ':'
               << std::setw(2) << sec;

            // Check timezone
            if (tz_hour > 0 || tz_min > 0) {
                ss << ((tz_sign) ? '-' : '+')
                    << std::setw(2) << tz_hour << ':'
                    << std::setw(2) << tz_min;
            }
            else {
                ss << 'Z';
            }
        }
        return ss.str();
    }
};
