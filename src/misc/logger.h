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
    std::string category_name;
};

class OStream {
    friend class Logger;

private:
    CategoryConfig* config;
    std::stringstream stream;

    OStream();

    OStream(CategoryConfig& config);

public:
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

    // not used by default, can be enabled by options
    std::ofstream out_file;

    static inline bool print_time = false;
    static inline bool print_category = false;
    static inline std::mutex mutex;

    Logger();

    void set_output_file(const std::string& file_path);

    void set_print_time(bool b)
    {
        print_time = b;
    }

    void set_print_category(bool b)
    {
        print_category = b;
    }

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

    void enable_debug(bool b)
    {
        debug_config.enabled = b;
    }

    void enable_error(bool b)
    {
        error_config.enabled = b;
    }

    void enable_info(bool b)
    {
        info_config.enabled = b;
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
