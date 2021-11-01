#include <chrono>
#include <climits>
#include <experimental/filesystem>
#include <iostream>

#include <boost/program_options.hpp>

#include "base/binding/binding_id.h"
#include "base/ids/var_id.h"
#include "relational_model/execution/binding_id_iter/index_nested_loop_join.h"
#include "relational_model/execution/binding_id_iter/index_scan.h"
#include "relational_model/models/quad_model/import/bulk_import.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv) {
    string input_filename;
    string db_folder;
    int buffer_size;

	try {
        // Parse arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "show this help message")
            ("db-folder,d", po::value<string>(&db_folder)->required(), "set database folder path")
            ("buffer-size,b", po::value<int>(&buffer_size)->default_value(BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE),
                "set buffer pool size")
            ("filename,f", po::value<string>(&input_filename)->required(), "import filename")
        ;

        po::positional_options_description p;
        p.add("filename",  1);
        p.add("db-folder", 1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

        if (vm.count("help")) {
            cout << "Usage: create_db ./path/to/import_file.txt ./path/to/new_db [OPTIONS]\n";
            cout << desc << "\n";
            return 0;
        }
        po::notify(vm);

        // Validate params
        if (buffer_size < 0) {
            cerr << "Buffer size cannot be a negative number.\n";
            return 1;
        }

        { // check db_folder is empty or does not exists
            namespace fs = std::experimental::filesystem;
            if (fs::exists(db_folder) && !fs::is_empty(db_folder)) {
                cout << "Database folder already exists and it's not empty\n";
                return EXIT_FAILURE;
            }
        }

        cout << "Creating new database\n";
        cout << "  input file:  " << input_filename << "\n";
        cout << "  db folder:   " << db_folder << "\n";
        cout << "  buffer size: " << buffer_size << "\n\n";

        auto start = chrono::system_clock::now();
        cout << "Initializing system...\n";
        {
            QuadModel model(db_folder, buffer_size, 0, 0);

            // to measure time initializing the model
            auto end_model = chrono::system_clock::now();
            chrono::duration<float, milli> model_duration = end_model - start;
            cout << "  done in " << model_duration.count() << " ms\n\n";

            // start the import
            auto import = BulkImport(input_filename, model);
            import.start_import();

        }
        auto end = chrono::system_clock::now();
        chrono::duration<float, milli> duration = end - start;
        cout << "Total duration: " << duration.count() << " ms\n";

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
