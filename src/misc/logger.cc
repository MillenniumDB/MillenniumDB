#include <cerrno>
#include <filesystem>
#include <utility>

#include "logger.h"
#include "third_party/toml/toml.h"


std::ostream& operator<<(std::ostream& os, Category category) {
    switch (category) {
    case Category::Query:          { os << "Query";           break; }
    case Category::LogicalPlan:    { os << "LogicalPlan";     break; }
    case Category::PhysicalPlan:   { os << "PhysicalPlan";    break; }
    case Category::ExecutionStats: { os << "ExecutionStats";  break; }
    case Category::Error:          { os << "Error";           break; }
    case Category::Info:           { os << "Info";            break; }
    case Category::Debug:          { os << "Debug";           break; }
    default:                       { os << "InvalidCategory"; break; }
    }
    return os;
}

Category string_to_category(const std::string_view category) {
    if (category == "Query")                return Category::Query;
    else if (category == "LogicalPlan")     return Category::LogicalPlan;
    else if (category == "PhysicalPlan")    return Category::PhysicalPlan;
    else if (category == "ExecutionStats")  return Category::ExecutionStats;
    else if (category == "Error")           return Category::Error;
    else if (category == "Info")            return Category::Info;
    else if (category == "Debug")           return Category::Debug;
    else                                    return Category::InvalidCategory;
}


static void write_time(std::ostream& os) {
    auto time = std::time(nullptr);
    char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
    std::strftime(std::data(timeString), std::size(timeString), "%FT%TZ", std::gmtime(&time));
    os << timeString;
}


// --------------------- OStream ------------------------


OStream::~OStream() {
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


Logger::Logger() {
    categories = {
        { Category::Query,          {} },
        { Category::LogicalPlan,    {} },
        { Category::PhysicalPlan,   {} },
        { Category::ExecutionStats, {} },
        { Category::Error,          {} },
        { Category::Info,           {} },
        { Category::Debug,          {} },
    };

#ifdef NDEBUG
    categories[Category::Debug].enabled = false;
#else
    categories[Category::Debug].enabled = true;
#endif
    categories[Category::Error].os = &std::cerr;
}


bool Logger::read_config(const std::string& path) {
    toml::table table;

    try {
        table = toml::parse_file(path);
    } catch (const toml::parse_error& err) {
        std::cerr << "Config file parsing failed:\n" << err << ".\n";
        return true;
    }

    // std::cout << table << "\n";
    auto logger_table = table["logger"].as_table();

    if (logger_table == nullptr) {
        std::cerr << "Config file parsing failed: \"logger\" is not a table.\n";
        return true;
    }

    for (auto [category_, config_node] : *logger_table) {
        auto config_table = config_node.as_table();

        if (config_table == nullptr) {
            std::cerr << "Config file parsing failed: \"" << category_ << "\" is not a table.\n";
            return true;
        }

        auto category = string_to_category(category_.str());
        if (category == Category::InvalidCategory) {
            std::cerr << "Config file parsing failed: \"" << category_ << "\" is not a valid logger category.\n";
            std::cerr << "Valid categories are: Query, LogicalPlan, PhysicalPlan, ExecutionStats, Error, Info and Debug.\n";
            return true;
        }

        for (auto [key, value] : *config_table) {
            if (key == "file") {
                auto val = value.as_string();

                if (val == nullptr) {
                    std::cerr << "Config file parsing failed: " << category << "." << key << " is not a string.\n";
                    return true;
                }

                auto path = val->get();
                if (std::filesystem::exists(path) && !std::filesystem::is_regular_file(path)) {
                    std::cerr << "Config file parsing failed, Not a file: " << path << ".\n";
                    return true;
                }

                if (ofstreams.find(path) == ofstreams.end()) {
                    auto path_ = std::filesystem::path(path);
                    if (path_.has_parent_path()) {
                        auto parent = path_.parent_path();
                        if (!std::filesystem::exists(parent)) {
                            std::error_code ec;
                            std::filesystem::create_directories(parent, ec);
                            if (ec) {
                                std::cerr << "Config file parsing failed, " << ec.message() << ": " << parent << ".\n";
                                return true;
                            }
                        }
                        if (!std::filesystem::is_directory(parent)) {
                            std::cerr << "Config file parsing failed, Not a directory: " << parent << ".\n";
                            return true;
                        }
                    }
                    errno = 0;
                    ofstreams[path] = std::ofstream(path, std::ofstream::app);
                    if (errno) {
                        std::cerr << "Config file parsing failed, "<< strerror(errno) << ": " << path << ".\n";
                        return true;
                    }
                }

                logger.categories[category].os = &ofstreams[path];
            } else if (key == "enabled") {
                auto val = value.as_boolean();
                if (val == nullptr) {
                    std::cerr << "Config file parsing failed: " << category << "." << key << " is not a boolean.\n";
                    return true;
                }
                logger.categories[category].enabled = val->get();
            } else if (key == "print_time") {
                auto val = value.as_boolean();
                if (val == nullptr) {
                    std::cerr << "Config file parsing failed: " << category << "." << key << " is not a boolean.\n";
                    return true;
                }
                logger.categories[category].print_time = val->get();
            } else if (key == "print_category") {
                auto val = value.as_boolean();
                if (val == nullptr) {
                    std::cerr << "Config file parsing failed: " << category << "." << key << " is not a boolean.\n";
                    return true;
                }
                logger.categories[category].print_category = val->get();
            } else if (key == "verbosity") {
                auto val = value.as_integer();
                if (val == nullptr) {
                    std::cerr << "Config file parsing failed: " << category << "." << key << " is not an integer.\n";
                    return true;
                }
                logger.categories[category].verbosity = val->get();
            } else {
                std::cerr << "Config file parsing failed: \"" << key << "\" is not a valid logger config key.\n";
                std::cerr << "Valid keys are: file, enabled, print_time, print_category, verbosity.\n";
                return true;
            }
        }
    }
    return false;
}


void Logger::log(Category category, std::function<void(std::ostream&)> print_function, unsigned verbosity) {
    auto it = categories.find(category);
    if (it == categories.end()) return;

    auto& config = it->second;

    if (!config.enabled || verbosity > config.verbosity) return;

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


OStream Logger::operator()(Category category, unsigned verbosity) {
    auto it = categories.find(category);
    if (it == categories.end()) return {};

    auto& config = it->second;

    if (!config.enabled || verbosity > config.verbosity) return {};

    return OStream(mutex, category, config);
}
