#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

struct CategoryConfig {
    std::ostream* os = &std::cout;
    bool enabled = true;
    bool print_time = false;
    bool print_category = false;
    std::string category_name;
};

class OStream {
    friend class Logger;

private:
    CategoryConfig* config;
    std::stringstream stream;

    OStream() :
        config(nullptr)
    { }

    OStream(CategoryConfig& config) :
        config(&config)
    { }

public:
    static inline std::mutex mutex;

    ~OStream();

    template<typename T>
    OStream& operator<<(T&& obj)
    {
        if (config != nullptr)
            stream << obj;
        return *this;
    }

    template<typename T>
    OStream& operator<<(T& obj)
    {
        if (config != nullptr)
            stream << obj;
        return *this;
    }
};

static_assert(!std::is_move_constructible<OStream>());
static_assert(!std::is_move_assignable<OStream>());
static_assert(!std::is_copy_constructible<OStream>());
static_assert(!std::is_copy_assignable<OStream>());

class Logger {
public:

    CategoryConfig debug_config;
    CategoryConfig error_config;
    CategoryConfig info_config;

    Logger();

    void debug(std::function<void(std::ostream&)> f)
    {
        return get(debug_config, f);
    }

    void error(std::function<void(std::ostream&)> f)
    {
        return get(error_config, f);
    }

    void info(std::function<void(std::ostream&)> f)
    {
        return get(info_config, f);
    }

    OStream debug()
    {
        return get(debug_config);
    }

    OStream error()
    {
        return get(error_config);
    }

    OStream info()
    {
        return get(info_config);
    }

    static void write_time(std::ostream& os);

private:
    OStream get(CategoryConfig& config);

    void get(CategoryConfig& config, std::function<void(std::ostream&)> print_function);
};

inline Logger logger;
