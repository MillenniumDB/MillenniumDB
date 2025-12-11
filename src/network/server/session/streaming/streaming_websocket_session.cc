#include "streaming_websocket_session.h"

#include "misc/logger.h"
#include "network/exceptions.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/streaming/request/streaming_gql_request_handler.h"
#include "network/server/session/streaming/request/streaming_quad_request_handler.h"
#include "network/server/session/streaming/request/streaming_rdf_request_handler.h"

using namespace MDBServer;
using namespace boost;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

StreamingWebSocketSession::StreamingWebSocketSession(
    Server& server_,
    websocket_stream_type&& stream_,
    std::chrono::seconds query_timeout_,
    bool write_authorized
) :
    StreamingSession(server_, write_authorized),
    query_timeout(query_timeout_),
    stream(std::move(stream_))
{
    // Bind the request handler depending on the database model
    if (server.model_id == Protocol::QUAD_MODEL_ID) {
        request_handler = std::make_unique<StreamingQuadRequestHandler>(*this);
    } else if (server.model_id == Protocol::RDF_MODEL_ID) {
        request_handler = std::make_unique<StreamingRdfRequestHandler>(*this);
    } else if (server.model_id == Protocol::GQL_MODEL_ID) {
        request_handler = std::make_unique<StreamingGQLRequestHandler>(*this);
    } else {
        throw std::runtime_error("Unhandled ModelId: " + std::to_string(server.model_id));
    }

    // Set WebSocket stream flags/options
    stream.binary(true);
    stream.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));
}

StreamingWebSocketSession::~StreamingWebSocketSession()
{
    if (stream.is_open()) {
        stream.close(websocket::close_code::normal, ec);
        logger(Category::Debug) << "StreamingWebSocketSession: connection closed by destructor";
        if (ec) {
            logger(Category::Error) << "Close failed:" << ec.what();
        }
    }
}

void StreamingWebSocketSession::start_decode_chunk()
{
    decoded_chunks.clear();

    auto self = this->shared_from_this();

    // read initial chunk size
    async_read_nbytes(2, [self]() {
        // read chunk size
        auto initial_chunk_size_bytes = static_cast<const uint8_t*>(self->request_buffer.data().data());
        const uint16_t initial_chunk_size = (static_cast<uint16_t>(initial_chunk_size_bytes[0]) << 8)
                                          | initial_chunk_size_bytes[1];
        self->request_buffer.consume(2);

        // decode next chunk
        self->decode_chunk(initial_chunk_size);
    });
}

void StreamingWebSocketSession::decode_chunk(std::size_t chunk_size)
{
    if (decoded_chunks.size() + chunk_size > Protocol::MAX_REQUEST_BYTES) {
        // max request size reached
        close_with_error("Protocol exception: Protocol::MAX_REQUEST_BYTES reached");
        return;
    }

    if (chunk_size == 0) {
        // no more chunks to decode, handle request
        // cannot assert that request_buffer is empty like TCP, ws could have sent two another request in one message

        try {
            request_handler->handle(decoded_chunks.data(), decoded_chunks.size());
        } catch (const InterruptedException& e) {
            close_with_error("Interruption exception: Query timed out");
            return;
        } catch (const ProtocolException& e) {
            close_with_error("Protocol exception: " + std::string(e.what()));
            return;
        } catch (const ConnectionException& e) {
            logger(Category::Error) << "Connection exception: " << e.what();
            return;
        } catch (const std::exception& e) {
            logger(Category::Error) << "Uncaught exception: " << e.what();
            return;
        } catch (...) {
            logger(Category::Error) << "Unexpected exception!";
            return;
        }

        run();
        return;
    }

    auto self = this->shared_from_this();

    // read current chunk + next chunk size
    async_read_nbytes(chunk_size + 2, [self, chunk_size]() {
        // append decoded chunk
        auto chunk_bytes = static_cast<const uint8_t*>(self->request_buffer.data().data());
        const auto old_size = self->decoded_chunks.size();
        self->decoded_chunks.resize(old_size + chunk_size);
        std::memcpy(self->decoded_chunks.data() + old_size, chunk_bytes, chunk_size);

        // read next size
        const auto next_chunk_size_bytes = chunk_bytes + chunk_size;
        const uint16_t next_chunk_size = (static_cast<uint16_t>(next_chunk_size_bytes[0]) << 8)
                                       | next_chunk_size_bytes[1];
        self->request_buffer.consume(chunk_size + 2);

        // decode next chunk
        self->decode_chunk(next_chunk_size);
    });
}

void StreamingWebSocketSession::run()
{
    if (stream.is_open()) {
        start_decode_chunk();
    }
}

void StreamingWebSocketSession::write(const uint8_t* bytes, std::size_t num_bytes)
{
    stream.write(boost::asio::buffer(bytes, num_bytes), ec);

    if (ec) {
        stream.close(websocket::close_code::internal_error);
        throw ConnectionException("StreamingWebSocketSession write error: " + ec.message());
    }
}

std::mutex& StreamingWebSocketSession::get_thread_info_vec_mutex()
{
    return server.thread_info_vec_mutex;
}

std::chrono::seconds StreamingWebSocketSession::get_timeout()
{
    return query_timeout;
}

bool StreamingWebSocketSession::try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token)
{
    return server.try_cancel(worker_idx, cancel_token);
}

template<typename OnRead>
void StreamingWebSocketSession::async_read_nbytes(std::size_t n, OnRead&& on_read)
{
    if (request_buffer.size() >= n) {
        // enough data
        std::forward<OnRead>(on_read)();
        return;
    }

    auto self = this->shared_from_this();

    stream.async_read(
        request_buffer,
        [self, n, on_read_ = std::forward<OnRead>(on_read)](
            const boost::system::error_code& ec,
            std::size_t /*bytes_transferred*/
        ) {
            if (ec) {
                if (ec != websocket::error::closed) {
                    logger(Category::Error) << "StreamingWebSocketSession read error: " << ec.message();
                }
                return;
            }

            // keep reading
            self->async_read_nbytes(n, std::move(on_read_));
        }
    );
}

void StreamingWebSocketSession::close_with_error(const std::string& msg)
{
    logger(Category::Error) << msg;
    request_handler->response_writer->write_error(msg);
    request_handler->response_writer->flush();

    stream.close(websocket::close_code::normal, ec);
    if (ec) {
        logger(Category::Error) << "Close failed:" << ec.what();
    }
}
