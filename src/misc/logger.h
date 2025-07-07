#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>

enum class Category {
    Query,
    LogicalPlan,
    PhysicalPlan,
    ExecutionStats,
    Error,
    Info,
    Debug,
    InvalidCategory,
};

struct CategoryConfig {
    std::ostream* os = &std::cout;
    bool enabled = true;
    bool print_time = false;
    bool print_category = false;
    unsigned verbosity = 0;
};

class OStream {
    friend class Logger;

private:
    std::mutex* mutex;
    Category category;
    CategoryConfig* config;
    std::stringstream stream;

    OStream() :
        mutex(nullptr),
        category(Category::InvalidCategory),
        config(nullptr)
    { }

    OStream(std::mutex& mutex, Category category, CategoryConfig& config) :
        mutex(&mutex),
        category(category),
        config(&config)
    { }

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
private:
    std::mutex mutex;

public:
    std::map<Category, CategoryConfig> categories;
    std::map<std::string, std::ofstream> ofstreams;

    Logger();

    void log(Category category, std::function<void(std::ostream&)>, unsigned verbosity = 0);

    OStream operator()(Category category, unsigned verbosity = 0);
};

inline Logger logger;
