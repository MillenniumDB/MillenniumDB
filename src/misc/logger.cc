#include "logger.h"

#include <ctime>

OStream::~OStream()
{
    if (config != nullptr) {
        std::unique_lock lck(mutex);

        if (config->print_category) {
            *config->os << '[' << config->category_name << ']';
        }

        if (config->print_time) {
            *config->os << '[';
            Logger::write_time(*config->os);
            *config->os << ']';
        }

        *config->os << '\n';
        *config->os << stream.str();
        *config->os << '\n';
        *config->os << std::flush;
    }
}

// --------------------- Logger ------------------------

Logger::Logger()
{
#ifdef NDEBUG
    debug_config.category_name = "DEBUG";
    debug_config.enabled = false;
    debug_config.print_category = true;
    debug_config.print_time = true;

    info_config.category_name = "INFO";
    info_config.enabled = true;
    info_config.print_category = true;
    info_config.print_time = true;

    error_config.category_name = "ERROR";
    error_config.enabled = true;
    error_config.print_category = true;
    error_config.print_time = true;
#else
    debug_config.category_name = "DEBUG";
    debug_config.enabled = true;
    debug_config.print_category = false;
    debug_config.print_time = false;

    info_config.category_name = "INFO";
    info_config.enabled = true;
    info_config.print_category = false;
    info_config.print_time = false;

    error_config.category_name = "ERROR";
    error_config.enabled = true;
    error_config.print_category = false;
    error_config.print_time = false;
#endif
    error_config.os = &std::cerr;
}

void Logger::write_time(std::ostream& os)
{
    auto time = std::time(nullptr);
    char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
    std::strftime(std::data(timeString), std::size(timeString), "%FT%TZ", std::gmtime(&time));
    os << timeString;
}

OStream Logger::get(CategoryConfig& config)
{
    if (!config.enabled)
        return OStream();

    return OStream(config);
}

void Logger::get(CategoryConfig& config, std::function<void(std::ostream&)> print_function)
{
    if (!config.enabled)
        return;

    std::unique_lock lck(OStream::mutex);

    if (config.print_category) {
        *config.os << '[' << config.category_name << "]";
    }

    if (config.print_time) {
        *config.os << '[';
        write_time(*config.os);
        *config.os << "]";
    }

    *config.os << '\n';

    print_function(*config.os);

    *config.os << '\n';
    *config.os << std::flush;
}
