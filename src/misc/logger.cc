#include "logger.h"

#include <ctime>

#include "misc/fatal_error.h"

OStream::OStream() :
    config(nullptr)
{ }

OStream::OStream(CategoryConfig& config) :
    config(&config)
{
    stream << '\n';
}

OStream::~OStream()
{
    if (config != nullptr) {
        std::unique_lock lck(Logger::mutex);

        if (Logger::print_category) {
            *config->os << '[' << config->category_name << ']';
        }

        if (Logger::print_time) {
            *config->os << '[';
            Logger::write_time(*config->os);
            *config->os << ']';
        }

        std::string output = stream.str();

        *config->os << output;
        if (output.back() != '\n') { // output cannot be empty, always starts with '\n'
            *config->os << '\n';
        }
        *config->os << std::flush;
    }
}

Logger::Logger()
{
    print_time = false;
    print_category = false;

    debug_config.category_name = "DEBUG";
    info_config.category_name = "INFO";
    error_config.category_name = "ERROR";

    info_config.enabled = true;
    error_config.enabled = true;

#ifdef NDEBUG
    debug_config.enabled = false;
#else
    debug_config.enabled = true;
#endif

    error_config.os = &std::cerr;
}

void Logger::set_output_file(const std::string& file_path)
{
    out_file = std::ofstream(file_path, std::ios::out | std::ios::app);

    if (!out_file) {
        FATAL_ERROR("Could not open logger output file ", file_path);
    }

    debug_config.os = &out_file;
    info_config.os = &out_file;
    error_config.os = &out_file;
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

    std::unique_lock lck(mutex);

    if (print_category) {
        *config.os << '[' << config.category_name << "]";
    }

    if (print_time) {
        *config.os << '[';
        write_time(*config.os);
        *config.os << "]";
    }

    std::stringstream stream;
    stream << '\n';
    print_function(stream);

    std::string output = stream.str();
    *config.os << output;

    if (output.back() != '\n') { // output cannot be empty, always starts with '\n'
        *config.os << '\n';
    }
    *config.os << std::flush;
}
