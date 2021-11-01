/*
 * query is a executable that takes a file with the query, send it to the server via TCP socket
 * and print the result received.
 *
 * Program Flow is:
 * - read file to a string
 * - connect to server via TCP socket
 * - send query length
 * - send query
 * - listen to results and print them. Results may come in many packages of a fixed size: `db_server::BUFFER_SIZE`
 *     - first byte indicates the state (not finished, finished successfully or finished with errors.
 *     - next 2 bytes indicates the length of the message (needed because `db_server::BUFFER_SIZE`
 *       bytes must be sent even if the message is shorter).
 */
#include <fstream>
#include <iostream>
#include <iterator>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "server/tcp_buffer.h"

using namespace std;
using boost::asio::ip::tcp;
namespace po = boost::program_options;

int main(int argc, char **argv) {
    // string query_file;
    string host;
    int port;
    try {
        // Parse arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "show this help message")
            ("host,h", po::value<string>(&host)->default_value("127.0.0.1"), "database server host")
            ("port,p", po::value<int>(&port)->default_value(db_server::DEFAULT_PORT), "database server port")
            // ("query-file,q", po::value<string>(&query_file)->required(), "query file")
        ;

        po::positional_options_description p;
        // p.add("query-file", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

        if (vm.count("help")) {
            cout << "Usage: query [OPTIONS]... ./path/to/query/file\n";
            cout << desc << "\n";
            return 0;
        }
        po::notify(vm);

        stringstream str_stream;
        str_stream << std::cin.rdbuf();
        string query = str_stream.str();

        boost::asio::io_service io_service;

        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({host, std::to_string(port)}));

        // Send Query
        auto query_length = query.size();

        unsigned char query_size_b[db_server::BYTES_FOR_QUERY_LENGTH];
        for (int i = 0, offset = 0; i < db_server::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
            unsigned char c = (query_length >> offset) & 0xFF;
            query_size_b[i] = c;
        }
        boost::asio::write(s, boost::asio::buffer(query_size_b, db_server::BYTES_FOR_QUERY_LENGTH));
        boost::asio::write(s, boost::asio::buffer(query.data(), query_length));

        // Read results
        unsigned char result_buffer[db_server::BUFFER_SIZE];
        do {
            boost::asio::read(s, boost::asio::buffer(result_buffer, db_server::BUFFER_SIZE));
            unsigned int reply_length = 0;
            reply_length += result_buffer[1];
            reply_length += result_buffer[2] << 8;
            std::cout.write(reinterpret_cast<char*>(result_buffer+3), reply_length-3);
        } while ( result_buffer[0] == static_cast<unsigned char>(db_server::MessageType::not_end) );

        if (result_buffer[0] == static_cast<unsigned char>(db_server::MessageType::end_success)) {
            return 0;
        } else {
            return result_buffer[1];
        }
    }
    catch (boost::system::system_error const& e) {
        std::cout << "Error connecting to server: " << e.what() << "\n";
        return static_cast<int>(db_server::ErrorCode::connection_error);
    }
    catch(const std::exception& e) {
        cerr << e.what() << "\n";
        return static_cast<int>(db_server::ErrorCode::unexpected_error);
    }
    catch(...) {
        cerr << "Exception of unexpected type!\n";
        return static_cast<int>(db_server::ErrorCode::unexpected_error);
    }
}
