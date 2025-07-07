#pragma once

#include <cstdint>
#include <string>

#include "graph_models/object_id.h"

static constexpr int64_t SECONDS_IN_YEAR   = 31536000;
static constexpr int64_t SECONDS_IN_DAY    =    86400;
static constexpr int64_t SECONDS_IN_HOUR   =     3600;
static constexpr int64_t SECONDS_IN_MINUTE =       60;

class DateTime7Properties {
public:
    int64_t year;
    int64_t month;
    int64_t day;

    int64_t hour;
    int64_t minute;
    int64_t second;

    int64_t tz_min_offset;

    DateTime7Properties() :
        year(1), month(1), day(1),
        hour(0), minute(0), second(0),
        tz_min_offset(0) { }

    int64_t time_on_timeline_seconds() const noexcept;
};

enum class DateTimeComparisonMode { Normal, Strict, StrictEquality };

class DateTime {
private:
    static constexpr uint64_t SIGN        = 0x0080'0000'0000'0000ULL;
    static constexpr uint64_t LOW_PRES    = 0x0040'0000'0000'0000ULL;

    static constexpr uint64_t BIG_YEAR    = 0x003F'FFFF'FFFF'FFFFULL;

    static constexpr uint64_t DATE        = 0x003F'FFFF'8000'0000ULL;
    static constexpr uint64_t YEAR        = 0x003F'FF00'0000'0000ULL;
    static constexpr uint64_t MONTH       = 0x0000'00F0'0000'0000ULL;
    static constexpr uint64_t DAY         = 0x0000'000F'8000'0000ULL;

    static constexpr uint64_t TIME        = 0x0000'0000'7FFF'C000ULL;
    static constexpr uint64_t HOUR        = 0x0000'0000'7C00'0000ULL;
    static constexpr uint64_t MINUTE      = 0x0000'0000'03F0'0000ULL;
    static constexpr uint64_t SECOND      = 0x0000'0000'000F'C000ULL;

    static constexpr uint64_t HAS_TZ      = 0x0000'0000'0000'2000ULL;
    static constexpr uint64_t TZ_SIGN     = 0x0000'0000'0000'1000ULL;

    static constexpr uint64_t TZ          = 0x0000'0000'0000'0FFEULL;
    static constexpr uint64_t TZ_HOUR     = 0x0000'0000'0000'0F80ULL;
    static constexpr uint64_t TZ_MINUTE   = 0x0000'0000'0000'007EULL;

    static constexpr uint64_t TZ0_Z       = 0x0000'0000'0000'0001ULL;

    static constexpr uint64_t WITHOUT_TZ  = 0x00FF'FFFF'FFFF'C000ULL;

public:
    uint64_t id;

    constexpr DateTime(uint64_t id) noexcept : id(id) { }
    constexpr DateTime(ObjectId oid) noexcept : id(oid.id) { }

    static uint8_t days_in_month(int64_t year, uint8_t month);

    static uint64_t from_time(const std::string& str) noexcept;
    static uint64_t from_date(const std::string& str) noexcept;
    static uint64_t from_dateTime(const std::string& str) noexcept;
    static uint64_t from_dateTime(const std::string& str, bool* error) noexcept;
    static uint64_t from_dateTimeStamp(const std::string& str) noexcept;

    static uint64_t from_zoned_time(const std::string& str) noexcept;
    static uint64_t from_local_time(const std::string& str) noexcept;
    static uint64_t from_zoned_datetime(const std::string& str) noexcept;
    static uint64_t from_local_datetime(const std::string& str) noexcept;

    bool equal(const DateTime& rhs, bool* error) const noexcept;


    template<DateTimeComparisonMode mode>
    int64_t compare(const DateTime& rhs, bool* error = nullptr) const noexcept;

    int64_t MQL_compare(const DateTime& rhs) const noexcept;

    DateTime7Properties create_7_properties() const noexcept;

    int64_t time_on_timeline_years() const noexcept;

    int64_t get_year(bool* error) const noexcept;
    int64_t get_month(bool* error) const noexcept;
    int64_t get_day(bool* error) const noexcept;
    int64_t get_hour(bool* error) const noexcept;
    int64_t get_minute(bool* error) const noexcept;
    int64_t get_second(bool* error) const noexcept;

    bool has_tz() const noexcept;
    int64_t get_tz_min_offset(bool* error) const noexcept;

    std::string get_tz() const noexcept;
    std::string get_timezone(bool* error) const noexcept;
    std::string get_value_string() const noexcept;
    std::string get_datatype_string() const noexcept;
};
