/**
 * @file server.h
 * @author SergeiNA
 * @brief Simple async server based on boost asio
 * @version 0.1
 * @date 2020-03-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

#include <cstddef>
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include "async_queue.h"


using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, ConnectionId id);
    void start();
    ~session();
private:
    void do_read();
private:
    ConnectionId m_id;
    tcp::socket m_socket;
    std::array<char, 1024> m_data;
};

class server
{
public:
    explicit server(unsigned short port, size_t bulkSize);
private:
    void do_accept();
private:
    boost::asio::io_service m_service;
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
};