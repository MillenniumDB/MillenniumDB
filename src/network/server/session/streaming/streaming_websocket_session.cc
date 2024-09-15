#include "streaming_websocket_session.h"

#include "misc/logger.h"
#include "network/exceptions.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/streaming/request/streaming_quad_request_handler.h"
#include "network/server/session/streaming/request/streaming_rdf_request_handler.h"

using namespace MDBServer;
using namespace boost;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;


StreamingWebSocketSession::StreamingWebSocketSession(
    Server&                 server_,
    websocket_stream_type&& stream_,
    std::chrono::seconds    query_timeout_
) :
    server(server_),
    query_timeout(query_timeout_),
    stream(std::move(stream_))
{
    // Bind the request handler depending on the database model
    if (server.model_id == Protocol::QUAD_MODEL_ID) {
        request_handler = std::make_unique<StreamingQuadRequestHandler>(*this);
    } else if (server.model_id == Protocol::RDF_MODEL_ID) {
        request_handler = std::make_unique<StreamingRdfRequestHandler>(*this);
    } else {
        throw std::runtime_error("Unhandled ModelId" + std::to_string(server.model_id));
    }

    // Set WebSocket stream flags/options
    stream.binary(true);
    stream.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));
}


StreamingWebSocketSession::~StreamingWebSocketSession() {
    if (stream.is_open()) {
        stream.close(websocket::close_code::normal, ec);
        logger(Category::Debug) << "StreamingWebSocketSession: connection closed by destructor";
        if (ec) {
            logger(Category::Error) << "Close failed:" << ec.what();
        }
    }
}


void StreamingWebSocketSession::run(std::unique_ptr<StreamingWebSocketSession> obj) {
    if (obj->stream.is_open()) {

        auto& request_buffer = obj->request_buffer;
        obj->stream.async_read(request_buffer,
            [obj = std::move(obj)](boost::beast::error_code ec, size_t /*length*/) mutable {
                if (ec) {
                    if (ec != websocket::error::closed) {
                        logger(Category::Error) << "StreamingWebSocketSession read error: " << ec.message();
                    }
                    return;
                }

                try {
                    const auto request_bytes = boost::asio::buffer_cast<uint8_t*>(obj->request_buffer.data());
                    obj->request_handler->handle(request_bytes, obj->request_buffer.size());
                    obj->request_buffer.consume(obj->request_buffer.size());
                }
                catch (const InterruptedException& e) {
                    auto& response_writer = obj->request_handler->response_writer;
                    response_writer->write_error("Interruption exception: Query timed out");
                    response_writer->flush();

                    obj->stream.close(websocket::close_code::normal, ec);
                    logger(Category::Error) << "Interruption exception: Query timed out";
                    if (ec) {
                        logger(Category::Debug) << "Close failed:" << ec.what();
                    }
                }
                catch (const ProtocolException& e) {
                    logger(Category::Error) << "Protocol exception: " << e.what();
                }
                catch (const ConnectionException& e) {
                    logger(Category::Error) << "Connection exception: " << e.what();
                }
                catch (const std::exception& e) {
                    logger(Category::Error) << "Uncaught exception: " << e.what();
                }
                catch (...) {
                    logger(Category::Error) << "Unexpected exception!";
                }

                StreamingWebSocketSession::run(std::move(obj));
            }
        );
    }
}


void StreamingWebSocketSession::write(const uint8_t* bytes, std::size_t num_bytes) {
    stream.write(boost::asio::buffer(bytes, num_bytes), ec);

    if (ec) {
        stream.close(websocket::close_code::internal_error);
        throw ConnectionException("StreamingWebSocketSession write error: " + ec.message());
    }
}


std::mutex& StreamingWebSocketSession::get_thread_info_vec_mutex() {
    return server.thread_info_vec_mutex;
}


std::chrono::seconds StreamingWebSocketSession::get_timeout() {
    return query_timeout;
}


bool StreamingWebSocketSession::try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token) {
    return server.try_cancel(worker_idx, cancel_token);
}
