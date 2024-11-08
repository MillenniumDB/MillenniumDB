#include "datetime.h"

#include <cassert>
#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>
#include <system_error>

#include "graph_models/object_id.h"
#include "query/exceptions.h"


int64_t DateTime7Properties::time_on_timeline_seconds() const noexcept {
    // Previous year
    int64_t prev_year = year - 1;

    int64_t time_on_timeline = SECONDS_IN_YEAR * prev_year;

    // Account for leap years
    time_on_timeline += SECONDS_IN_DAY * (prev_year/400 - prev_year/100 + prev_year/4);

    // Add seconds in the months
    for (int64_t m = 1; m < month; m++) {
        time_on_timeline += SECONDS_IN_DAY * DateTime::days_in_month(year, m);
    }

    time_on_timeline += SECONDS_IN_DAY    * (day-1);
    time_on_timeline += SECONDS_IN_HOUR   * hour;
    time_on_timeline += SECONDS_IN_MINUTE * (minute - tz_min_offset);
    time_on_timeline += second;

    return time_on_timeline;
}


uint8_t DateTime::days_in_month(int64_t year, uint8_t month) {
    switch (month) {
        case 2: {
            if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
                return 29;
            } else {
                return 28;
            }
        }
        case 4: case 6: case 9: case 11: return 30;
        case 1: case 3: case 5: case  7: case 8: case 10: case 12: return 31;
        default: throw LogicException("called days_in_month incorrectly");
    }
}


uint64_t DateTime::from_time(const std::string& str) noexcept {
    constexpr auto NO_ERROR = std::errc();
    uint64_t ret = ObjectId::MASK_DT_TIME;

    const char* ptr = str.data();
    const char* const last = ptr + str.size();

    if (ptr + 8 > last) { return ObjectId::NULL_ID; }

    uint64_t year = 0;
    uint8_t month = 1;
    uint8_t day = 1;

    // Handle time
    uint8_t hour; auto res = std::from_chars(ptr, last, hour); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 6 > last || *ptr != ':' || hour > 23) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t min; res = std::from_chars(ptr, last, min); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || min > 59) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t sec; res = std::from_chars(ptr, last, sec); ptr = res.ptr;
    if (res.ec != NO_ERROR || sec > 59) { return ObjectId::NULL_ID; }

    uint64_t fractional_seconds = 0;
    if (ptr < last && *ptr == '.') {
        ptr++;
        if (ptr + 1 > last) { return ObjectId::NULL_ID; }
        res = std::from_chars(ptr, last, fractional_seconds); ptr = res.ptr;
        if (res.ec != NO_ERROR) { return ObjectId::NULL_ID; }
    }

    // Handle timezone
    uint8_t has_tz  = 0;
    uint8_t tz_sign = 0;
    uint8_t tz_hour = 0;
    uint8_t tz_min  = 0;
    uint8_t tz0_z = 0;

    if (ptr < last) {
        if (*ptr == '+' || *ptr == '-') {
            if (ptr < last - 6) { return ObjectId::NULL_ID; }

            tz_sign = *ptr == '-';
            ptr++;

            res = std::from_chars(ptr, last, tz_hour); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || tz_hour > 14) { return ObjectId::NULL_ID; }
            else { ptr++; }

            res = std::from_chars(ptr, last, tz_min); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr != last || tz_min > 59) { return ObjectId::NULL_ID; }

            if (tz_hour == 14 && tz_min != 0) { return ObjectId::NULL_ID; }

            has_tz = 1;
            tz0_z = 0;
        } else if (*ptr == 'Z') {
            if (ptr + 1 != last) { return ObjectId::NULL_ID; }
            has_tz = 1;
            tz0_z = 1;
        } else {
            return ObjectId::NULL_ID;
        }
    }

    // Handle years that do not fit in 14 bits using lower precision
    if (year > 0x3FFF) {
        ret |= year;
        ret |= LOW_PRES;
        return ret;
    }

    // Construct final representation
    uint64_t dt = 0;
    dt = (dt << 14) | year;
    dt = (dt <<  4) | month;
    dt = (dt <<  5) | day;
    dt = (dt <<  5) | hour;
    dt = (dt <<  6) | min;
    dt = (dt <<  6) | sec;
    dt = (dt <<  1) | has_tz;
    dt = (dt <<  1) | tz_sign;
    dt = (dt <<  5) | tz_hour;
    dt = (dt <<  6) | tz_min;
    dt = (dt <<  1) | tz0_z;

    return ret | dt;
}


uint64_t DateTime::from_date(const std::string& str) noexcept {
    constexpr auto NO_ERROR = std::errc();
    uint64_t ret = ObjectId::MASK_DT_DATE;

    const char* ptr = str.data();
    const char* const last = ptr + str.size();

    if (ptr + 10 > last) { return ObjectId::NULL_ID; }

    if (*ptr == '-') {
        ret |= SIGN;
        ptr++;
    }

    // Handle date
    uint64_t year; auto res = std::from_chars(ptr, last, year); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 6 > last || *ptr != '-' || year > BIG_YEAR) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t month; res = std::from_chars(ptr, last, month); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != '-' || month < 1 || month > 12) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t day; res = std::from_chars(ptr, last, day); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr > last || day < 1 || day > days_in_month(year, month)) { return ObjectId::NULL_ID; }

    uint8_t hour = 0;
    uint8_t min = 0;
    uint8_t sec = 0;

    // Handle timezone
    uint8_t has_tz  = 0;
    uint8_t tz_sign = 0;
    uint8_t tz_hour = 0;
    uint8_t tz_min  = 0;
    uint8_t tz0_z = 0;

    if (ptr < last) {
        if (*ptr == '+' || *ptr == '-') {
            if (ptr + 6 < last) { return ObjectId::NULL_ID; }

            tz_sign = *ptr == '-';
            ptr++;

            res = std::from_chars(ptr, last, tz_hour); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || tz_hour > 14) { return ObjectId::NULL_ID; }
            else { ptr++; }

            res = std::from_chars(ptr, last, tz_min); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr != last || tz_min > 59) { return ObjectId::NULL_ID; }

            if (tz_hour == 14 && tz_min != 0) { return ObjectId::NULL_ID; }

            has_tz = 1;
            tz0_z = 0;
        } else if (*ptr == 'Z') {
            if (ptr + 1 != last) { return ObjectId::NULL_ID; }
            has_tz = 1;
            tz0_z = 1;
        } else {
            return ObjectId::NULL_ID;
        }
    }

    // Handle years that do not fit in 14 bits using lower precision
    if (year > 0x3FFF) {
        ret |= year;
        ret |= LOW_PRES;
        return ret;
    }

    // Construct final representation
    uint64_t dt = 0;
    dt = (dt << 14) | year;
    dt = (dt <<  4) | month;
    dt = (dt <<  5) | day;
    dt = (dt <<  5) | hour;
    dt = (dt <<  6) | min;
    dt = (dt <<  6) | sec;
    dt = (dt <<  1) | has_tz;
    dt = (dt <<  1) | tz_sign;
    dt = (dt <<  5) | tz_hour;
    dt = (dt <<  6) | tz_min;
    dt = (dt <<  1) | tz0_z;

    return ret | dt;
}


uint64_t DateTime::from_dateTime(const std::string& str) noexcept {
    constexpr auto NO_ERROR = std::errc();
    uint64_t ret = ObjectId::MASK_DT_DATETIME;

    const char* ptr = str.data();
    const char* const last = ptr + str.size();

    if (ptr + 19 > last) { return ObjectId::NULL_ID; }

    if (*ptr == '-') {
        ret |= SIGN;
        ptr++;
    }

    // Handle date and time
    uint64_t year; auto res = std::from_chars(ptr, last, year); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 15 > last || *ptr != '-' || year > BIG_YEAR) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t month; res = std::from_chars(ptr, last, month); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 12 > last || *ptr != '-' || month < 1 || month > 12) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t day; res = std::from_chars(ptr, last, day); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 9 > last || *ptr != 'T' || day < 1 || day > days_in_month(year, month)) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t hour; res = std::from_chars(ptr, last, hour); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 6 > last || *ptr != ':' || hour > 23) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t min; res = std::from_chars(ptr, last, min); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || min > 59) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t sec; res = std::from_chars(ptr, last, sec); ptr = res.ptr;
    if (res.ec != NO_ERROR || sec > 59) { return ObjectId::NULL_ID; }

    uint64_t fractional_seconds = 0;
    if (ptr < last && *ptr == '.') {
        ptr++;
        if (ptr + 1 > last) { return ObjectId::NULL_ID; }
        res = std::from_chars(ptr, last, fractional_seconds); ptr = res.ptr;
        if (res.ec != NO_ERROR) { return ObjectId::NULL_ID; }
    }

    // Handle timezone
    uint8_t has_tz  = 0;
    uint8_t tz_sign = 0;
    uint8_t tz_hour = 0;
    uint8_t tz_min  = 0;
    uint8_t tz0_z = 0;

    if (ptr < last) {
        if (*ptr == '+' || *ptr == '-') {
            if (ptr < last - 6) { return ObjectId::NULL_ID; }

            tz_sign = *ptr == '-';
            ptr++;

            res = std::from_chars(ptr, last, tz_hour); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || tz_hour > 14) { return ObjectId::NULL_ID; }
            else { ptr++; }

            res = std::from_chars(ptr, last, tz_min); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr != last || tz_min > 59) { return ObjectId::NULL_ID; }

            if (tz_hour == 14 && tz_min != 0) { return ObjectId::NULL_ID; }

            has_tz = 1;
            tz0_z = 0;
        } else if (*ptr == 'Z') {
            if (ptr + 1 != last) { return ObjectId::NULL_ID; }
            has_tz = 1;
            tz0_z = 1;
        } else {
            return ObjectId::NULL_ID;
        }
    }

    // Handle years that do not fit in 14 bits using lower precision
    if (year > 0x3FFF) {
        ret |= year;
        ret |= LOW_PRES;
        return ret;
    }

    // Construct final representation
    uint64_t dt = 0;
    dt = (dt << 14) | year;
    dt = (dt <<  4) | month;
    dt = (dt <<  5) | day;
    dt = (dt <<  5) | hour;
    dt = (dt <<  6) | min;
    dt = (dt <<  6) | sec;
    dt = (dt <<  1) | has_tz;
    dt = (dt <<  1) | tz_sign;
    dt = (dt <<  5) | tz_hour;
    dt = (dt <<  6) | tz_min;
    dt = (dt <<  1) | tz0_z;

    return ret | dt;
}


uint64_t DateTime::from_dateTimeStamp(const std::string& str) noexcept {
    constexpr auto NO_ERROR = std::errc();
    uint64_t ret = ObjectId::MASK_DT_DATETIMESTAMP;

    const char* ptr = str.data();
    const char* const last = ptr + str.size();

    if (ptr + 19 > last) { return ObjectId::NULL_ID; }

    if (*ptr == '-') {
        ret |= SIGN;
        ptr++;
    }

    // Handle date and time
    uint64_t year; auto res = std::from_chars(ptr, last, year); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 15 > last || *ptr != '-' || year > BIG_YEAR) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t month; res = std::from_chars(ptr, last, month); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 12 > last || *ptr != '-' || month < 1 || month > 12) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t day; res = std::from_chars(ptr, last, day); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 9 > last || *ptr != 'T' || day < 1 || day > days_in_month(year, month)) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t hour; res = std::from_chars(ptr, last, hour); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 6 > last || *ptr != ':' || hour > 23) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t min; res = std::from_chars(ptr, last, min); ptr = res.ptr;
    if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || min > 59) { return ObjectId::NULL_ID; }
    else { ptr++; }

    uint8_t sec; res = std::from_chars(ptr, last, sec); ptr = res.ptr;
    if (res.ec != NO_ERROR || sec > 59) { return ObjectId::NULL_ID; }

    uint64_t fractional_seconds = 0;
    if (ptr < last && *ptr == '.') {
        ptr++;
        if (ptr + 1 > last) { return ObjectId::NULL_ID; }
        res = std::from_chars(ptr, last, fractional_seconds); ptr = res.ptr;
        if (res.ec != NO_ERROR) { return ObjectId::NULL_ID; }
    }

    // Handle timezone
    uint8_t has_tz  = 0;
    uint8_t tz_sign = 0;
    uint8_t tz_hour = 0;
    uint8_t tz_min  = 0;
    uint8_t tz0_z = 0;

    if (ptr < last) {
        if (*ptr == '+' || *ptr == '-') {
            if (ptr < last - 6) { return ObjectId::NULL_ID; }

            tz_sign = *ptr == '-';
            ptr++;

            res = std::from_chars(ptr, last, tz_hour); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr + 3 > last || *ptr != ':' || tz_hour > 14) { return ObjectId::NULL_ID; }
            else { ptr++; }

            res = std::from_chars(ptr, last, tz_min); ptr = res.ptr;
            if (res.ec != NO_ERROR || ptr != last || tz_min > 59) { return ObjectId::NULL_ID; }

            if (tz_hour == 14 && tz_min != 0) { return ObjectId::NULL_ID; }

            has_tz = 1;
            tz0_z = 0;
        } else if (*ptr == 'Z') {
            if (ptr + 1 != last) { return ObjectId::NULL_ID; }
            has_tz = 1;
            tz0_z = 1;
        } else {
            return ObjectId::NULL_ID;
        }
    } else {
        // No timezone
        return ObjectId::NULL_ID;
    }

    // Handle years that do not fit in 14 bits using lower precision
    if (year > 0x3FFF) {
        ret |= year;
        ret |= LOW_PRES;
        return ret;
    }

    // Construct final representation
    uint64_t dt = 0;
    dt = (dt << 14) | year;
    dt = (dt <<  4) | month;
    dt = (dt <<  5) | day;
    dt = (dt <<  5) | hour;
    dt = (dt <<  6) | min;
    dt = (dt <<  6) | sec;
    dt = (dt <<  1) | has_tz;
    dt = (dt <<  1) | tz_sign;
    dt = (dt <<  5) | tz_hour;
    dt = (dt <<  6) | tz_min;
    dt = (dt <<  1) | tz0_z;

    return ret | dt;
}


DateTime7Properties DateTime::create_7_properties() const noexcept {
    DateTime7Properties prop;

    // Handle sign, turn 0/1 into 1/-1
    int64_t sign = 1 - 2 * static_cast<int64_t>((id & SIGN) != 0);

    // Check if low precision
    if (id & LOW_PRES) {
        prop.year = sign * static_cast<int64_t>(id & BIG_YEAR);
        return prop;
    }

    // Make a copy of id
    uint64_t id_ = id;

    id_ >>= 1; // ignore tz0_rep
    uint64_t tz_min  = id_ &   0x3F; id_ >>= 6;
    uint64_t tz_hour = id_ &   0x1F; id_ >>= 5;
    uint64_t tz_sign = id_ &    0x1; id_ >>= 1;
    uint64_t has_tz  = id_ &    0x1; id_ >>= 1;
    uint64_t second  = id_ &   0x3F; id_ >>= 6;
    uint64_t minute  = id_ &   0x3F; id_ >>= 6;
    uint64_t hour    = id_ &   0x1F; id_ >>= 5;
    uint64_t day     = id_ &   0x1F; id_ >>= 5;
    uint64_t month   = id_ &    0xF; id_ >>= 4;
    uint64_t year    = id_ & 0x3FFF;


    auto subtype = ObjectId(id).get_sub_type();

    if (subtype != ObjectId::MASK_DT_TIME) {
        prop.year = sign * static_cast<int64_t>(year);
        prop.month = month;
        prop.day = day;
    }

    if (subtype != ObjectId::MASK_DT_DATE) {
        prop.hour = hour;
        prop.minute = minute;
        prop.second = second;
    }

    if (has_tz) {
        int64_t tz_sign_mult = 1 - 2 * static_cast<int64_t>(tz_sign);
        prop.tz_min_offset = tz_sign_mult * static_cast<int64_t>(tz_hour * 60 + tz_min);
    }

    return prop;
}


int64_t DateTime::time_on_timeline_years() const noexcept {
    auto low_pres = (id & LOW_PRES) != 0;
    int64_t years;

    if (low_pres) {
        years = id & BIG_YEAR;
    } else {
        years = (id & YEAR) >> 40;
    }

    int64_t sign = (id & SIGN) != 0;
    int64_t sign_mult = 1 - 2 * sign;
    return sign_mult * years;
}


int64_t DateTime::get_year(bool* error) const noexcept {
    if (ObjectId(id).get_sub_type() == ObjectId::MASK_DT_TIME) {
        *error = true;
        return 0;
    }
    *error = false;

    int64_t sign = 1 - 2 * ((id & SIGN) != 0); // TURN 0/1 into 1/-1
    if (id & LOW_PRES) {
        return sign * static_cast<int64_t>(id & BIG_YEAR);
    } else {
        return sign * static_cast<int64_t>((id & YEAR) >> 40);
    }
}


int64_t DateTime::get_month(bool* error) const noexcept {
    if (ObjectId(id).get_sub_type() == ObjectId::MASK_DT_TIME) {
        *error = true;
        return 0;
    }
    *error = false;

    if (id & LOW_PRES) {
        return 1;
    } else {
        return (id & MONTH) >> 36;
    }
}


int64_t DateTime::get_day(bool* error) const noexcept {
    if (ObjectId(id).get_sub_type() == ObjectId::MASK_DT_TIME) {
        *error = true;
        return 0;
    }
    *error = false;

    if (id & LOW_PRES) {
        return 1;
    } else {
        return (id & DAY) >> 31;
    }
}


int64_t DateTime::get_hour(bool* error) const noexcept {
    if (ObjectId(id).get_sub_type() == ObjectId::MASK_DT_DATE) {
        *error = true;
        return 0;
    }
    *error = false;

    if (id & LOW_PRES) {
        return 0;
    } else {
        return (id & HOUR) >> 26;
    }
}


int64_t DateTime::get_minute(bool* error) const noexcept {
    if (ObjectId(id).get_sub_type() == ObjectId::MASK_DT_DATE) {
        *error = true;
        return 0;
    }
    *error = false;

    if (id & LOW_PRES) {
        return 0;
    } else {
        return (id & MINUTE) >> 20;
    }
}


int64_t DateTime::get_second(bool* error) const noexcept {
    if (ObjectId(id).get_sub_type() == ObjectId::MASK_DT_DATE) {
        *error = true;
        return 0;
    }
    *error = false;

    if (id & LOW_PRES) {
        return 0;
    } else {
        return (id & SECOND) >> 14;
    }
}


int64_t DateTime::get_tz_min_offset(bool* error) const noexcept {
    if (id & LOW_PRES) {
        *error = true;
        return 0;
    }

    const uint64_t has_tz = (id & HAS_TZ) >> 13;
    if (!has_tz) {
        *error = true;
        return 0;
    }

    *error = false;
    const uint64_t tz_sign      = (id & TZ_SIGN)   >> 12;
    const uint64_t tz_hour      = (id & TZ_HOUR)   >>  7;
    const uint64_t tz_minute    = (id & TZ_MINUTE) >>  1;
    const int64_t  tz_sign_mult = 1 - 2 * static_cast<int64_t>(tz_sign);
    return tz_sign_mult * static_cast<int64_t>(tz_hour * 60 + tz_minute);
}


std::string DateTime::get_tz() const noexcept {
    if (id & LOW_PRES) {
        // No timezone stored for low precision
        return "";
    }

    uint64_t has_tz = (id & HAS_TZ) >> 13;
    if (!has_tz) {
        // No timezone specified
        return "";
    }

    uint64_t tz_hour   = (id & TZ_HOUR)   >>  7;
    uint64_t tz_minute = (id & TZ_MINUTE) >>  1;
    if (tz_hour == 0 && tz_minute == 0) {
        uint64_t tz0_z   = (id & TZ0_Z);
        if (tz0_z) {
            // Special timezone 0 representation
            return "Z";
        }
    }

    uint64_t tz_sign  = (id & TZ_SIGN) >> 12;
    std::stringstream ss;
    ss << std::setfill('0')
        << ((tz_sign) ? '-' : '+')
        << std::setw(2) << tz_hour << ':'
        << std::setw(2) << tz_minute;
    return ss.str();
}


std::string DateTime::get_timezone(bool* error) const noexcept {
    if (id & LOW_PRES) {
        *error = true;
        return "";
    }

    uint64_t has_tz = (id & HAS_TZ) >> 13;
    if (!has_tz) {
        // No timezone specified
        *error = true;
        return "";
    }

    *error = false;

    uint64_t tz_hour   = (id & TZ_HOUR)   >>  7;
    uint64_t tz_minute = (id & TZ_MINUTE) >>  1;
    if (tz_hour == 0 && tz_minute == 0) {
        return "PT0S";
    }

    uint64_t tz_sign  = (id & TZ_SIGN) >> 12;
    std::stringstream ss;

    if (tz_sign) {
        ss << "-";
    }
    ss << "PT";
    if (tz_hour != 0) {
        ss << tz_hour << "H";
    }
    if (tz_minute != 0) {
        ss << tz_minute << "M";
    }

    return ss.str();
}


std::string DateTime::get_value_string() const noexcept {
    uint64_t datetime_id = id;
    std::stringstream ss;

        // Check sign bit
    if (datetime_id & SIGN) {
        ss << '-';
    }

    // Check if low precision
    if (datetime_id & LOW_PRES) {
        // Remove sign and precision bits
        ss << (datetime_id & BIG_YEAR)
            << "-01-01T00:00:00";
        return ss.str();
    }

    uint64_t tz0_z  = datetime_id & 0x1;
    datetime_id >>= 1;
    uint64_t tz_min  = datetime_id & 0x3F;
    datetime_id >>= 6;
    uint64_t tz_hour = datetime_id & 0x1F;
    datetime_id >>= 5;
    uint64_t tz_sign = datetime_id & 0x1;
    datetime_id >>= 1;
    uint64_t has_tz = datetime_id & 0x1;
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
    uint64_t year =  datetime_id & 0x3FFF;
    datetime_id >>= 14;

    auto subtype = ObjectId(id).get_sub_type();

    ss << std::setfill('0');
    if (subtype != ObjectId::MASK_DT_TIME) {
        ss << std::setw(4) << year << '-'
           << std::setw(2) << mon  << '-'
           << std::setw(2) << day;
    }

    // Only print time separator if we have both a date and a time
    if (subtype == ObjectId::MASK_DT_DATETIME || subtype == ObjectId::MASK_DT_DATETIMESTAMP) {
        ss << 'T';
    }

    if (subtype != ObjectId::MASK_DT_DATE) {
        ss << std::setw(2) << hour << ':'
           << std::setw(2) << min  << ':'
           << std::setw(2) << sec;
    }

    // Check timezone
    if (has_tz) {
        if (tz_hour == 0 && tz_min == 0 && tz0_z) {
            ss << 'Z';
        } else {
            ss << ((tz_sign) ? '-' : '+')
                << std::setw(2) << tz_hour << ':'
                << std::setw(2) << tz_min;
        }
    }

    return ss.str();
}


std::string DateTime::get_datatype_string() const noexcept {
    switch (ObjectId(id).get_sub_type()) {
        case ObjectId::MASK_DT_DATE:          return "http://www.w3.org/2001/XMLSchema#date";
        case ObjectId::MASK_DT_DATETIME:      return "http://www.w3.org/2001/XMLSchema#dateTime";
        case ObjectId::MASK_DT_TIME:          return "http://www.w3.org/2001/XMLSchema#time";
        case ObjectId::MASK_DT_DATETIMESTAMP: return "http://www.w3.org/2001/XMLSchema#dateTimeStamp";
        default: return "INVALID DT SUBTYPE";
    }
}


// returns negative number if lhs < rhs,
// returns 0 if lhs == rhs
// returns positive number if lhs > rhs
// For operators use Normal mode
// For expression comparison use Strict mode.
// For expression equality use StrictEquality mode.
template<DateTimeComparisonMode mode>
int64_t DateTime::compare(const DateTime& rhs, [[maybe_unused]] bool* error) const noexcept {
    if constexpr (mode != DateTimeComparisonMode::Normal) {
        assert(error != nullptr);
        *error = false;
    }
    auto& lhs = *this;

    // Optimization
    if (lhs.id == rhs.id) {
        return 0;
    }

    auto lhs_subtype = ObjectId(lhs.id).get_sub_type();
    auto rhs_subtype = ObjectId(rhs.id).get_sub_type();

    auto lhs_is_date_time = lhs_subtype == ObjectId::MASK_DT_DATETIME || lhs_subtype == ObjectId::MASK_DT_DATETIMESTAMP;
    auto rhs_is_date_time = rhs_subtype == ObjectId::MASK_DT_DATETIME || rhs_subtype == ObjectId::MASK_DT_DATETIMESTAMP;

    if (lhs_subtype == rhs_subtype || (lhs_is_date_time && rhs_is_date_time)) {
        auto lhs_low_pres = (lhs.id & LOW_PRES) != 0;
        auto rhs_low_pres = (rhs.id & LOW_PRES) != 0;

        if (lhs_low_pres || rhs_low_pres) {
            auto lhs_time_on_timeline = lhs.time_on_timeline_years();
            auto rhs_time_on_timeline = rhs.time_on_timeline_years();
            return lhs_time_on_timeline - rhs_time_on_timeline;
        }

        auto diff = (lhs.create_7_properties().time_on_timeline_seconds()
                   - rhs.create_7_properties().time_on_timeline_seconds());

        if constexpr (mode != DateTimeComparisonMode::Normal) {
            if ((lhs.id & HAS_TZ) != (rhs.id & HAS_TZ)) {
                if (std::abs(diff) <= SECONDS_IN_HOUR * 14) {
                    *error = true;
                }
            }
        }

        return diff;
    }


    if constexpr (mode == DateTimeComparisonMode::Normal) {
        return static_cast<int64_t>(lhs_subtype) - static_cast<int64_t>(rhs_subtype);
    } else if constexpr (mode == DateTimeComparisonMode::Strict) {
        *error = true;
        return 0;
    } else if constexpr (mode == DateTimeComparisonMode::StrictEquality) {
        return -1;
    }
}


int64_t DateTime::MQL_compare(const DateTime& rhs) const noexcept {
    auto& lhs = *this;

    // Optimization
    if (lhs.id == rhs.id) {
        return 0;
    }

    auto lhs_subtype = ObjectId(lhs.id).get_sub_type();
    auto rhs_subtype = ObjectId(rhs.id).get_sub_type();

    auto lhs_is_convertible_to_date_time = lhs_subtype != ObjectId::MASK_DT_TIME;
    auto rhs_is_convertible_to_date_time = rhs_subtype != ObjectId::MASK_DT_TIME;

    if (lhs_subtype == rhs_subtype || (lhs_is_convertible_to_date_time && rhs_is_convertible_to_date_time)) {
        auto lhs_low_pres = (lhs.id & LOW_PRES) != 0;
        auto rhs_low_pres = (rhs.id & LOW_PRES) != 0;

        if (lhs_low_pres || rhs_low_pres) {
            auto lhs_time_on_timeline = lhs.time_on_timeline_years();
            auto rhs_time_on_timeline = rhs.time_on_timeline_years();
            return lhs_time_on_timeline - rhs_time_on_timeline;
        }

        auto diff = (lhs.create_7_properties().time_on_timeline_seconds()
                   - rhs.create_7_properties().time_on_timeline_seconds());

        return diff;
    }

    // case for comparing date/dateTime/dateTimeStamp with time
    return lhs_is_convertible_to_date_time ? 1 : -1;
    // return static_cast<int64_t>(lhs_subtype) - static_cast<int64_t>(rhs_subtype);
}


template int64_t DateTime::compare<DateTimeComparisonMode::Normal>(const DateTime& rhs, bool* error) const noexcept;
template int64_t DateTime::compare<DateTimeComparisonMode::Strict>(const DateTime& rhs, bool* error) const noexcept;
template int64_t DateTime::compare<DateTimeComparisonMode::StrictEquality>(const DateTime& rhs, bool* error) const noexcept;
