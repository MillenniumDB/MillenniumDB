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
    StreamingSession(server),
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

void StreamingTCPSession::run()
{
    if (socket.is_open()) {
        auto self = this->shared_from_this();

        asio::async_read(
            socket,
            request_buffer.prepare(sizeof(uint32_t)),
            boost::asio::transfer_all(),
            [self](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
                if (ec) {
                    if (ec != asio::error::eof) {
                        logger(Category::Error) << "StreamingTCPSession read error: " << ec.message();
                    }
                    return;
                }

                self->request_buffer.commit(sizeof(uint32_t));
                const auto request_size_bytes = asio::buffer_cast<const uint8_t*>(self->request_buffer.data()
                );
                uint32_t request_size = 0;
                request_size |= request_size_bytes[0] << 24;
                request_size |= request_size_bytes[1] << 16;
                request_size |= request_size_bytes[2] << 8;
                request_size |= request_size_bytes[3];
                self->request_buffer.consume(sizeof(uint32_t));

                self->read_request(request_size);
            }
        );
    }
}

void StreamingTCPSession::read_request(uint32_t request_size)
{
    auto self = this->shared_from_this();
    asio::async_read(
        socket,
        request_buffer.prepare(request_size),
        // boost::asio::transfer_all(),
        boost::asio::transfer_exactly(request_size),
        [self](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            if (ec) {
                if (ec != asio::error::eof) {
                    logger(Category::Error) << "StreamingTCPSession read error: " << ec.message();
                }
                return;
            }

            try {
                self->request_buffer.commit(bytes_transferred);
                const auto request_bytes = asio::buffer_cast<const uint8_t*>(self->request_buffer.data());
                self->request_handler->handle(request_bytes, self->request_buffer.size());
            } catch (const InterruptedException& e) {
                auto& response_writer = self->request_handler->response_writer;
                response_writer->write_error("Interruption exception: Query timed out");
                response_writer->flush();

                self->socket.close();
                logger(Category::Error) << "Interruption exception: Query timed out";
                if (ec) {
                    logger(Category::Debug) << "Close failed:" << ec.what();
                }
            } catch (const ProtocolException& e) {
                logger(Category::Error) << "Protocol exception: " << e.what();
            } catch (const ConnectionException& e) {
                logger(Category::Error) << "Connection exception: " << e.what();
            } catch (const std::exception& e) {
                logger(Category::Error) << "Uncaught exception: " << e.what();
            } catch (...) {
                logger(Category::Error) << "Unexpected exception!";
            }

            self->request_buffer.consume(bytes_transferred);
            self->run();
        }
    );
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
