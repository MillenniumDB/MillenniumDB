#include <cstdint>
#include <iostream>
#include <optional>
#include <thread>

#include "bin/common.h"
#include "graph_models/exceptions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "query/parser/paths/regular_path_expr.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"
#include "system/tensor_manager.h"

namespace MdbBin {

struct SystemConfig {
    bool browser = true;

    uint_fast32_t port = MDBServer::Protocol::DEFAULT_PORT;
    uint_fast32_t browser_port = MDBServer::Protocol::DEFAULT_BROWSER_PORT;
    uint_fast32_t workers = std::thread::hardware_concurrency();

    uint64_t limit = 0; // 0 means no limit
    uint64_t strings_static_buffer = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t strings_dynamic_buffer = StringManager::DEFAULT_DYNAMIC_BUFFER;
    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    uint64_t tensors_static_buffer = TensorManager::DEFAULT_STATIC_BUFFER;
    uint64_t tensors_dynamic_buffer = TensorManager::DEFAULT_DYNAMIC_BUFFER;

    PathSearchMode path_mode = PathSearchMode::BFS;

    std::chrono::seconds query_timeout = MDBServer::Protocol::DEFAULT_QUERY_TIMEOUT_SECONDS;

    std::string db_directory;

    std::string admin_user;
    std::string admin_password;
};

struct SystemOptions {
    std::optional<bool> browser;
    std::optional<std::string> admin_user;
    std::optional<std::string> admin_password;

    std::optional<uint_fast32_t> port;
    std::optional<uint_fast32_t> browser_port;
    std::optional<uint_fast32_t> workers;
    std::optional<uint64_t> limit;
    std::optional<uint64_t> strings_static_buffer;
    std::optional<uint64_t> strings_dynamic_buffer;
    std::optional<uint64_t> private_pages_buffer;
    std::optional<uint64_t> versioned_pages_buffer;
    std::optional<uint64_t> unversioned_pages_buffer;
    std::optional<uint64_t> tensors_static_buffer;
    std::optional<uint64_t> tensors_dynamic_buffer;
    std::optional<PathSearchMode> path_mode;
    std::optional<std::chrono::seconds> query_timeout;
};

inline int mdb_server(const SystemConfig& conf)
{
    auto model_id = Catalog::get_model_id(conf.db_directory);

    System system(
        conf.db_directory,
        conf.strings_static_buffer,
        conf.strings_dynamic_buffer,
        conf.versioned_pages_buffer,
        conf.private_pages_buffer,
        conf.unversioned_pages_buffer,
        conf.tensors_static_buffer,
        conf.tensors_dynamic_buffer,
        conf.workers
    );

    MDBServer::Server server;
    if (!conf.admin_user.empty()) {
        server.set_admin_user(conf.admin_user, conf.admin_password);
    }
    try {
        std::unique_ptr<ModelDestroyer> model_destroyer;
        switch (model_id) {
        case Catalog::ModelID::QUAD: {
            std::cout << "Initializing Quad Model..." << std::endl;
            model_destroyer = QuadModel::init();

            quad_model.path_mode = conf.path_mode;
            if (conf.limit != 0) {
                quad_model.MAX_LIMIT = conf.limit;
            }

            quad_model.catalog.print(std::cout);
            server.model_id = MDBServer::Protocol::QUAD_MODEL_ID;
            break;
        }
        case Catalog::ModelID::RDF: {
            std::cout << "Initializing RDF Model..." << std::endl;
            model_destroyer = RdfModel::init();

            rdf_model.path_mode = conf.path_mode;
            if (conf.limit != 0) {
                rdf_model.MAX_LIMIT = conf.limit;
            }

            rdf_model.catalog.print(std::cout);
            server.model_id = MDBServer::Protocol::RDF_MODEL_ID;
            break;
        }
        case Catalog::ModelID::GQL: {
            std::cout << "Initializing GQL Model..." << std::endl;
            model_destroyer = GQLModel::init();

            gql_model.catalog.print(std::cout);
            server.model_id = MDBServer::Protocol::GQL_MODEL_ID;
            break;
        }
        } // end switch
        server.run(conf.port, conf.browser_port, conf.browser, conf.workers, conf.query_timeout);
    } catch (const WrongModelException& e) {
        FATAL_ERROR(e.what());
    } catch (const WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what());
    }

    return EXIT_SUCCESS;
}

inline std::map<std::string, std::function<std::string(SystemOptions&, const std::string&)>>
    get_optionals(bool server)
{
    std::map<std::string, std::function<std::string(SystemOptions&, const std::string&)>> opt;
    if (server) {
        opt.insert({ "admin-user", [](SystemOptions& config, const std::string& value) {
                        config.admin_user = value;
                        return "";
                    } });
        opt.insert({ "admin-password", [](SystemOptions& config, const std::string& value) {
                        config.admin_password = value;
                        return "";
                    } });
        opt.insert({ "port", [](SystemOptions& config, const std::string& value) {
                        try {
                            auto port = std::stoi(value);
                            if (port >= 1024 && port <= 65535) {
                                config.port = port;
                                return "";
                            }
                        } catch (...) {
                        }
                        return "invalid port, expected to be a integer in range 1024 to 65535";
                    } });
        opt.insert({ "browser", [](SystemOptions& config, const std::string& value) {
                        if (value == "true") {
                            config.browser = true;
                        } else if (value == "false") {
                            config.browser = false;
                        } else {
                            return "invalid value for browser, expected true or false";
                        }
                        return "";
                    } });
        opt.insert({ "browser-port", [](SystemOptions& config, const std::string& value) {
                        try {
                            auto port = std::stoi(value);
                            if (port >= 1024 && port <= 65535) {
                                config.browser_port = port;
                                return "";
                            }
                        } catch (...) {
                        }
                        return "invalid browser port, expected to be a integer in range 1024 to 65535";
                    } });
        opt.insert({ "threads", [](SystemOptions& config, const std::string& value) {
                        try {
                            auto threads = std::stoi(value);
                            if (threads > 0) {
                                config.workers = threads;
                                return "";
                            }
                        } catch (...) {
                        }
                        return "invalid worker threads, expected to be a positive integer";
                    } });
    }

    opt.insert({ "timeout", [](SystemOptions& config, const std::string& value) {
                    try {
                        auto seconds = std::stoi(value);
                        if (seconds > 0) {
                            config.query_timeout = std::chrono::seconds(seconds);
                            return "";
                        }
                    } catch (...) {
                    }
                    return "invalid timeout, expected to be a positive integer";
                } });

    opt.insert({ "strings-static", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for strings-static";
                    }
                    config.strings_static_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "strings-dynamic", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for strings-dynamic";
                    }
                    config.strings_dynamic_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "versioned-buffer", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for versioned-buffer";
                    }
                    config.versioned_pages_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "unversioned-buffer", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for unversioned-buffer";
                    }
                    config.unversioned_pages_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "private-buffer", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for private-buffer";
                    }
                    config.private_pages_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "tensors-static", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for tensors-static";
                    }
                    config.tensors_static_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "tensors-dynamic", [](SystemOptions& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for tensors-dynamic";
                    }
                    config.tensors_dynamic_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "path-mode", [](SystemOptions& config, const std::string& value) {
                    auto lc_value = to_lower(value);
                    if (lc_value == "bfs") {
                        config.path_mode = PathSearchMode::BFS;
                    } else if (lc_value == "dfs") {
                        config.path_mode = PathSearchMode::DFS;
                    } else {
                        return "invalid value for path-mode, expected BFS|DFS";
                    }
                    return "";
                } });

    return opt;
}

template<typename T>
void try_replace(T& target, const std::optional<T>& opt1, const std::optional<T>& opt2)
{
    if (opt1.has_value()) {
        target = opt1.value();
    } else if (opt2.has_value()) {
        target = opt2.value();
    }
}

inline SystemConfig get_system_config(const std::string& db_directory, const SystemOptions& args)
{
    auto config_path = db_directory + "/db.config";
    std::fstream config_fs(config_path, std::ios::in | std::ios::binary);
    if (!config_fs.is_open()) {
        WARN("Could not open database config: ", config_path);
    }

    std::vector<std::pair<std::string, std::string>> params;
    std::string line;
    while (std::getline(config_fs, line)) {
        size_t comment_pos = line.find('#');

        // Extract the part before the comment
        std::string no_comment = (comment_pos == std::string::npos) ? line : line.substr(0, comment_pos);

        size_t delimiter_pos = no_comment.find('=');

        if (delimiter_pos == std::string::npos) {
            continue;
        }

        std::string key = no_comment.substr(0, delimiter_pos);
        std::string value = no_comment.substr(delimiter_pos + 1);

        // trim key
        size_t key_first = key.find_first_not_of(" \t\n\r");
        if (key_first == std::string::npos) {
            continue;
        }
        size_t key_last = key.find_last_not_of(" \t\n\r");
        key = key.substr(key_first, (key_last - key_first + 1));

        // trim value
        size_t value_first = value.find_first_not_of(" \t\n\r");
        if (value_first == std::string::npos) {
            continue;
        }
        size_t value_last = value.find_last_not_of(" \t\n\r");
        value = value.substr(value_first, (value_last - value_first + 1));

        params.push_back({ key, value });
    }

    SystemOptions db_config;
    auto opt = get_optionals(true);

    for (auto&& [key, value] : params) {
        if (auto opt_found = opt.find(key); opt_found != opt.end()) {
            auto error = opt_found->second(db_config, value);
            if (!error.empty()) {
                FATAL_ERROR("Error in database config: ", error);
            }
        } else {
            WARN("Ignored key in database config: ", key);
        }
    }

    SystemConfig res;
    res.db_directory = db_directory;

    try_replace(res.browser, args.browser, db_config.browser);
    try_replace(res.admin_user, args.admin_user, db_config.admin_user);
    try_replace(res.admin_password, args.admin_password, db_config.admin_password);
    try_replace(res.port, args.port, db_config.port);
    try_replace(res.browser_port, args.browser_port, db_config.browser_port);
    try_replace(res.limit, args.limit, db_config.limit);
    try_replace(res.strings_static_buffer, args.strings_static_buffer, db_config.strings_static_buffer);
    try_replace(res.strings_dynamic_buffer, args.strings_dynamic_buffer, db_config.strings_dynamic_buffer);
    try_replace(res.private_pages_buffer, args.private_pages_buffer, db_config.private_pages_buffer);
    try_replace(res.versioned_pages_buffer, args.versioned_pages_buffer, db_config.versioned_pages_buffer);
    try_replace(res.unversioned_pages_buffer, args.unversioned_pages_buffer, db_config.unversioned_pages_buffer);
    try_replace(res.tensors_dynamic_buffer, args.tensors_dynamic_buffer, db_config.tensors_dynamic_buffer);
    try_replace(res.tensors_static_buffer, args.tensors_static_buffer, db_config.tensors_static_buffer);
    try_replace(res.path_mode, args.path_mode, db_config.path_mode);
    try_replace(res.query_timeout, args.query_timeout, db_config.query_timeout);

    return res;
}

inline SystemConfig parse_system_config(const std::vector<std::string>& args, bool server)
{
    auto opt = get_optionals(server);
    std::map<std::string, std::string> aliases;

    aliases.insert({ "-t", "--timeout" });

    std::string db_dir = "";

    if (server) {
        aliases.insert({ "-p", "--port" });
        aliases.insert({ "-j", "--threads" });
        aliases.insert({ "--workers", "--threads" });
    }

    size_t i = 0;
    SystemOptions config;

    while (i < args.size()) {
        const std::string* arg = &args[i];
        auto found_alias = aliases.find(*arg);
        if (found_alias != aliases.end()) {
            arg = &found_alias->second;
        }

        if (arg->data()[0] == '-') {
            if (arg->size() < 2 || arg->data()[1] != '-') {
                FATAL_ERROR("unrecognized option: ", args[i]);
            }

            auto clean_arg = arg->substr(2);
            if (auto opt_found = opt.find(clean_arg); opt_found != opt.end()) {
                i++;
                if (i >= args.size()) {
                    FATAL_ERROR("expected value for ", args[i]);
                }
                auto error = opt_found->second(config, args[i]);
                if (!error.empty()) {
                    FATAL_ERROR(error);
                }
            } else {
                FATAL_ERROR("unrecognized option: ", args[i]);
            }
        } else {
            if (!db_dir.empty()) {
                FATAL_ERROR("unexpected positional argument: ", args[i], "==", db_dir, '_', i);
            }
            db_dir = args[i];
        }
        i++;
    }

    if (db_dir.empty()) {
        FATAL_ERROR("missing directory folder argument.\nRun mdb --help for info.");
    }

    return get_system_config(db_dir, config);
}

inline SystemConfig parse_profile_config(const std::string& db_dir, const std::vector<std::string>& args)
{
    SystemOptions config;

    auto opt = get_optionals(false);
    std::map<std::string, std::string> aliases;

    aliases.insert({ "-t", "--timeout" });

    size_t i = 0;

    while (i < args.size()) {
        const std::string* arg = &args[i];
        auto found_alias = aliases.find(*arg);
        if (found_alias != aliases.end()) {
            arg = &found_alias->second;
        }

        if (auto opt_found = opt.find(*arg); opt_found != opt.end()) {
            i++;
            if (i >= args.size()) {
                FATAL_ERROR("expected value for ", args[i]);
            }
            auto error = opt_found->second(config, args[i]);
            if (!error.empty()) {
                FATAL_ERROR(error);
            }
        } else {
            FATAL_ERROR("unrecognized option: ", args[i]);
        }
        i++;
    }

    return get_system_config(db_dir, config);
}
} // namespace MdbBin
