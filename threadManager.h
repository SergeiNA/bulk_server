/**
 * @file threadManager.h
 * @author SergeiNA (you@domain.com)
 * @brief ThreadManager is implementation of multithreadind queue with several workers
 * 
 * ThreadManager able to add subscribers as a observers classes (loggers)
 * in different threads
 * @version 1.0
 * @date 2020-01-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>
#include <future>
#include <memory>

#include "metrics.h"

class ThreadManager{

public:
    /**
     * @brief Add subscribers, add metrics, create a new observer thread
     * 
     * @param obs       observer class (logger)
     * @param metrics   metrics colletor
     */
    void subscribe(std::shared_ptr<Observer> obs, std::shared_ptr<Metrics> metrics = nullptr);
    /**
     * @brief Add package to queue, notify all observer thread to wake up
     * 
     * @param packet packet of commands with timestamp
     */
    void push(Packet packet);
    /**
     * @brief Stop all thread and wait until all thread do threir work
     * 
     */
    void stop();
    /**
     * @brief Destroy the Thread Manager object
     * 
     * Call stop()
     */
    ~ThreadManager();

private:
    std::atomic_bool quit = false;          ///< indicate  don't wait for a new packets
    std::condition_variable conditional_v;  ///< conditioanl variable for wake up all threads
    std::mutex conditional_mutex;           ///< main mutex
    std::vector<std::thread> subs;          ///< vector of subscribers in individual threads
    std::queue<Packet> packetQueue;         ///< queue with input packet
};