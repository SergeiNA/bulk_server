/**
 * @file connectionManager.h
 * @author SergeiNA
 * @brief Connection ID factory
 * @version 0.1
 * @date 2020-03-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include <stack>
#include <unordered_set>

using ConnectionId = std::uint32_t;
class connectionManager{
public:
    /**
     * @brief Get the Connection ID
     * 
     * @return ConnectionId 
     */
    ConnectionId getConnection();
    /**
     * @brief Close connection by ID
     * 
     * @param connId 
     */
    void closeConnection(ConnectionId connId);
private:
    /**
     * @brief Create a new Connection ID
     * 
     * @return ConnectionId 
     */
    ConnectionId createConnection();
private:
    std::stack<ConnectionId> awaiting;  ///< created but not active connections Id
    static ConnectionId counter;        ///< counter for a new connections id
};

inline ConnectionId connectionManager::counter = 0;
