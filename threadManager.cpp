/**
 * @file threadManager.cpp
 * @author SergeiNA (you@domain.com)
 * @brief Implimintation of ThreadManager class
 * @version 1.0
 * @date 2020-01-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "threadManager.h"

/**
 * @brief Handle all work with queue of packets
 * 
 * @param packetQueue       queue of packets
 * @param conditional_v     conditional variable
 * @param conditional_mutex main mutex
 * @param quit              quite indicator (don't wait)
 * @param bulk              how process packets in the packetQueue
 */
void taskProcess(std::queue<Packet>& packetQueue,
            std::condition_variable& conditional_v,
            std::mutex& conditional_mutex,
            const std::atomic_bool& quit,
            std::function<void(const Packet&)> bulk)
{
    while (true) {

        std::unique_lock<std::mutex> lk(conditional_mutex);

        conditional_v.wait(lk, [&packetQueue,&quit]() { return !packetQueue.empty() || quit; });

        if (packetQueue.empty() && quit)
            return;

        if (!packetQueue.empty()) {
            auto packet = std::move(packetQueue.front());
            packetQueue.pop();
            lk.unlock();

            bulk(packet);
        }
    }
}



void ThreadManager::subscribe(std::shared_ptr<Observer> obs,  std::shared_ptr<Metrics> metrics){
    subs.emplace_back(std::thread(taskProcess,
                      std::ref(packetQueue),
                      std::ref(conditional_v),
                      std::ref(conditional_mutex),
                      std::cref(quit),
                      [obs, metrics](const Packet& p){
                          if(metrics)
                            metrics->collect(p.first);
                          obs->bulk(p);
                      }));
}

void ThreadManager::push(Packet packet){
    std::unique_lock<std::mutex> lk(conditional_mutex);
    packetQueue.emplace(packet);
    lk.unlock();
    conditional_v.notify_all();
}

void ThreadManager::stop(){
    conditional_v.notify_all();
    if(!quit)
        quit=true;
    for(auto&& s:subs)
        if(s.joinable())
            s.join();
}

ThreadManager::~ThreadManager(){

    ThreadManager::stop();

}
