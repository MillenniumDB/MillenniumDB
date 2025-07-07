#include <utility>

#include "logger.h"

std::ostream& operator<<(std::ostream& os, Category category)
{
    switch (category) {
    case Category::Query: {
        os << "Query";
        break;
    }
    case Category::LogicalPlan: {
        os << "LogicalPlan";
        break;
    }
    case Category::PhysicalPlan: {
        os << "PhysicalPlan";
        break;
    }
    case Category::ExecutionStats: {
        os << "ExecutionStats";
        break;
    }
    case Category::Error: {
        os << "Error";
        break;
    }
    case Category::Info: {
        os << "Info";
        break;
    }
    case Category::Debug: {
        os << "Debug";
        break;
    }
    default: {
        os << "InvalidCategory";
        break;
    }
    }
    return os;
}

Category string_to_category(const std::string_view category)
{
    if (category == "Query")
        return Category::Query;
    else if (category == "LogicalPlan")
        return Category::LogicalPlan;
    else if (category == "PhysicalPlan")
        return Category::PhysicalPlan;
    else if (category == "ExecutionStats")
        return Category::ExecutionStats;
    else if (category == "Error")
        return Category::Error;
    else if (category == "Info")
        return Category::Info;
    else if (category == "Debug")
        return Category::Debug;
    else
        return Category::InvalidCategory;
}

static void write_time(std::ostream& os)
{
    auto time = std::time(nullptr);
    char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
    std::strftime(std::data(timeString), std::size(timeString), "%FT%TZ", std::gmtime(&time));
    os << timeString;
}

// --------------------- OStream ------------------------

OStream::~OStream()
{
    if (mutex != nullptr && config != nullptr) {
        mutex->lock();

        if (config->print_category) {
            *config->os << '[' << category << "]";
        }

        if (config->print_time) {
            *config->os << '[';
            write_time(*config->os);
            *config->os << "]";
        }

        if (config->print_category || config->print_time) {
            *config->os << '\n';
        }

        *config->os << stream.str();
        *config->os << '\n';
        *config->os << std::flush;

        mutex->unlock();
    }
}

// --------------------- Logger ------------------------

Logger::Logger()
{
    categories = {
        {          Category::Query, {} },
        {    Category::LogicalPlan, {} },
        {   Category::PhysicalPlan, {} },
        { Category::ExecutionStats, {} },
        {          Category::Error, {} },
        {           Category::Info, {} },
        {          Category::Debug, {} },
    };

#ifdef NDEBUG
    categories[Category::Debug].enabled = false;
#else
    categories[Category::Debug].enabled = true;
#endif
    categories[Category::Error].os = &std::cerr;
}

void Logger::log(Category category, std::function<void(std::ostream&)> print_function, unsigned verbosity)
{
    auto it = categories.find(category);
    if (it == categories.end())
        return;

    auto& config = it->second;

    if (!config.enabled || verbosity > config.verbosity)
        return;

    mutex.lock();

    if (config.print_category) {
        *config.os << '[' << category << "]";
    }

    if (config.print_time) {
        *config.os << '[';
        write_time(*config.os);
        *config.os << "]";
    }

    if (config.print_category || config.print_time) {
        *config.os << '\n';
    }

    print_function(*config.os);
    *config.os << std::flush;

    mutex.unlock();
}

OStream Logger::operator()(Category category, unsigned verbosity)
{
    auto it = categories.find(category);
    if (it == categories.end())
        return {};

    auto& config = it->second;

    if (!config.enabled || verbosity > config.verbosity)
        return {};

    return OStream(mutex, category, config);
}
