#include "streaming_tcp_session.h"

#include "misc/logger.h"
#include "network/exceptions.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "network/server/session/streaming/request/streaming_gql_request_handler.h"
#include "network/server/session/streaming/request/streaming_quad_request_handler.h"
#include "network/server/session/streaming/request/streaming_rdf_request_handler.h"

using namespace MDBServer;
using namespace boost;

StreamingTCPSession::StreamingTCPSession(
    Server& server,
    boost::asio::ip::tcp::socket&& socket_,
    std::chrono::seconds query_timeout_
) :
    StreamingSession(server, false),
    query_timeout(query_timeout_),
    socket(std::move(socket_))
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
}

StreamingTCPSession::~StreamingTCPSession()
{
    if (socket.is_open()) {
        socket.close();
    }
}

void StreamingTCPSession::start_decode_chunk()
{
    decoded_chunks.clear();

    auto self = this->shared_from_this();

    // read initial chunk size
    boost::asio::async_read(
        socket,
        request_buffer.prepare(2),
        boost::asio::transfer_all(),
        [self](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                if (ec != asio::error::eof) {
                    logger.error() << "StreamingTCPSession start_decode_chunk error: " << ec.message();
                }
                return;
            }

            // read chunk size
            self->request_buffer.commit(2);
            auto initial_chunk_size_bytes = static_cast<const uint8_t*>(self->request_buffer.data().data());
            const uint16_t initial_chunk_size = (static_cast<uint16_t>(initial_chunk_size_bytes[0]) << 8)
                                              | initial_chunk_size_bytes[1];
            self->request_buffer.consume(2);

            // decode next chunk
            self->decode_chunk(initial_chunk_size);
        }
    );
}

void StreamingTCPSession::decode_chunk(std::size_t chunk_size)
{
    if (decoded_chunks.size() + chunk_size > Protocol::MAX_REQUEST_BYTES) {
        // max request size reached
        close_with_error("Protocol exception: Protocol::MAX_REQUEST_BYTES reached");
        return;
    }

    if (chunk_size == 0) {
        // no more chunks to decode, handle request
        assert(request_buffer.size() == 0);

        try {
            request_handler->handle(decoded_chunks.data(), decoded_chunks.size());
        } catch (const InterruptedException& e) {
            close_with_error("Query timed out");
            return;
        } catch (const ProtocolException& e) {
            close_with_error("Protocol exception: " + std::string(e.what()));
            return;
        } catch (const ConnectionException& e) {
            logger.error() << "Connection exception: " << e.what();
            return;
        } catch (const std::exception& e) {
            logger.error() << "Uncaught exception: " << e.what();
            return;
        } catch (...) {
            logger.error() << "Unexpected exception!";
            return;
        }

        run();
        return;
    }

    auto self = this->shared_from_this();

    // read current chunk + next chunk size
    boost::asio::async_read(
        socket,
        request_buffer.prepare(chunk_size + 2),
        boost::asio::transfer_all(),
        [self, chunk_size](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                if (ec != asio::error::eof) {
                    logger.error() << "StreamingTCPSession decode_chunk error: " << ec.message();
                }
                return;
            }

            // append decoded chunk
            self->request_buffer.commit(chunk_size + 2);
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
        }
    );
}

void StreamingTCPSession::run()
{
    if (socket.is_open()) {
        start_decode_chunk();
    }
}

void StreamingTCPSession::write(const uint8_t* bytes, std::size_t num_bytes)
{
    asio::write(socket, asio::buffer(bytes, num_bytes), asio::transfer_all(), ec);

    if (ec) {
        socket.close();
        throw ConnectionException("StreamingTCPSession write error: " + ec.message());
    }
}

std::mutex& StreamingTCPSession::get_thread_info_vec_mutex()
{
    return server.thread_info_vec_mutex;
}

std::chrono::seconds StreamingTCPSession::get_timeout()
{
    return query_timeout;
}

bool StreamingTCPSession::try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token)
{
    return server.try_cancel(worker_idx, cancel_token);
}

void StreamingTCPSession::close_with_error(const std::string& msg)
{
    logger.error() << msg;
    request_handler->response_writer->write_error(msg);
    request_handler->response_writer->flush();

    socket.close(ec);
    if (ec) {
        logger.debug() << "Close failed:" << ec.what();
    }
}
