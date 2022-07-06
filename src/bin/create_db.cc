#include <iostream>

#include "import/quad_model/on_memory/import.h"
#include "import/quad_model/on_disk/import.h"
#include "storage/filesystem.h"
#include "storage/file_manager.h"
#include "third_party/cxxopts/cxxopts.h"

using namespace std;

void exit_if(bool exit_condition, const string& msg) {
    if (exit_condition) {
        cerr << msg << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    string input_filename;
    string db_folder;
    int buffer_size;

	try {
        cxxopts::Options options("create_db", "Import a database from a text file");
        options.add_options()
            ("h,help", "Print usage")
            ("d,db-folder", "path to the database folder to be created", cxxopts::value<string>(db_folder))
            ("b,buffer-size", "set memory buffer size (in GB)", cxxopts::value<int>(buffer_size)->default_value("1"))
            ("f,file", "file path to be imported", cxxopts::value<string>(input_filename))
        ;

        options.positional_help("import-file db-folder");
        options.parse_positional({"file", "db-folder"});

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(EXIT_SUCCESS);
        }

        exit_if(input_filename.empty(), "Must specify an import file");
        exit_if(db_folder.empty(), "Must specify a db-folder");
        exit_if(input_filename.empty(), "Buffer size must be a positive number");
        exit_if(Filesystem::exists(db_folder) && !Filesystem::is_empty(db_folder),
                "Database folder already exists and it's not empty\n");

        cout << "Creating new database\n";
        cout << "  input file:  " << input_filename << "\n";
        cout << "  db folder:   " << db_folder << "\n";

        FileManager::init(db_folder);
        // Import::InMemoryImport importer(db_folder, buffer_size);
        Import::OnDiskImport importer(db_folder, buffer_size);
        importer.start_import(input_filename);

        return EXIT_SUCCESS;
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        cerr << "Exception of unknown type!\n";
        return EXIT_FAILURE;
    }
}
