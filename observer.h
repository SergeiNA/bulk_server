/**
 * @file observer.h
 * @author SergeiNA (you@domain.com)
 * @brief observer pattern implementation
 * 
 * log_observer - logging packages info files and apply fibonacci on each command
 * terminal_observer - display packages to stdout and apply factorial on each command
 * bukl - dump package of commands and apply math operation on them before show
 * @version 1.0
 * @date 2020-01-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <atomic>

typedef std::chrono::nanoseconds rawTimestamp;
typedef std::pair<std::vector<std::string>,rawTimestamp> Packet;
/**
 * @brief Observer interface
 * 
 */
class Observer {
public:
    virtual void bulk(const Packet &) = 0;
    virtual ~Observer() =default;
};
/**
 * @brief Display packages to stdout
 * 
 * input: 1,2
 *        3
 * out: bulk: 1,2
 *      bulk: 6 
 */
class terminal_observer : public Observer {
public:
    terminal_observer(std::ostream& ofs = std::cout);
    /**
     * @brief display command to stdout
     * 
     * @param pack commands with timestamp to display
     */
    void bulk(const Packet& pack) override;
private:
    bool isBegin; ///< indicate start of printing to stdout
    std::ostream& os;
};


/**
 * @brief Create and wtite to file package of commands
 * name files as bulk_n_<unixtime>.log
 * where n is a static atomic increment for each file
 */
class log_observer : public Observer {
public:
    log_observer();
    /**
     * @brief write commands to file
     * 
     * @param pack commands with timestamp to write
     */
    void bulk(const Packet& pack) override;
private:
    static std::atomic_int id_bulk;
    std::ofstream File_;
};

/// inline initializations for static atomic increment in log_observer
inline std::atomic_int log_observer::id_bulk=0;