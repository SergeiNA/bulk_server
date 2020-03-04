#include "server.h"

session::session(tcp::socket socket, ConnectionId id)
        :   m_id(id),
            m_socket(std::move(socket))
{ }

void session::start() {
    do_read();
}

void session::do_read() {
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_data),
                             [this, self](boost::system::error_code ec, std::size_t length)
                             {
                                 if (!ec) {
                                     asyncq::receive(m_id, m_data.data(), length);
                                     do_read();
                                 }
                                 else
                                     asyncq::disconnect(m_id);
                             });
}

session::~session(){
    asyncq::disconnect(m_id);
}

server::server(unsigned short port, size_t bulkSize)
        :   m_acceptor(m_service, tcp::endpoint(tcp::v4(), port)),
            m_socket(m_service)
{
    asyncq::asyncInitilizer(bulkSize);
    do_accept();
    m_service.run();
}

void server::do_accept() {
    m_acceptor.async_accept(m_socket,
                            [this](boost::system::error_code ec)
                            {
                                if (!ec)
                                {
                                    std::make_shared<session>(std::move(m_socket), asyncq::connect())->start();
                                }
                                do_accept();
                            });
}