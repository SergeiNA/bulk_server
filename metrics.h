/**
 * @file metrics.h
 * @author SergeiNA (you@domain.com)
 * @brief Declaration of metric collectors classes
 * 
 * Metrics - interface
 * 
 * MainMetrics - collect metrics in main thread
 * 
 * SideMetrics - collect metrics in side threads (logging threads)
 * 
 * @version 1.0
 * @date 2020-01-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once
#include "observer.h"

/**
 * @brief Interface for metric collectors
 * 
 * collect - collect information:
 * number of blocks, size of blocks and optional number of handled lines
 * 
 */
class Metrics{
public:
    virtual void collect(const std::vector<std::string>& packet, size_t nstr =0)=0;
    virtual void display() const =0;
    virtual ~Metrics()=default;
};

/**
 * @brief Collect metrics from main thread
 * 
 * Number of handled lines
 * Number of blocks
 * Number of commands 
 * 
 */
class MainMetrics : public Metrics{
public:
    /**
     * @brief Construct a new Main Metrics object
     * 
     * @param ofs ostream for output information
     */
    MainMetrics(std::ostream& ofs = std::cout);
    /**
     * @brief collect metrics info
     * 
     * @param pack pack of commands
     * @param nstr number of handled lines
     */
    void collect(const std::vector<std::string>& pack, size_t nstr) override;
    /**
     * @brief Show all collected information
     * ex:
     * Main thread - 20 strings, 3 commands, 14 blocks
     * 
     */
    void display() const override;
    /// Call display()
    ~MainMetrics();
    
private:
    std::ostream& os;
    size_t n_str;
    size_t n_cmds;
    size_t n_blocks;
};

/**
 * @brief Collect metric information from loggers
 * 
 * Number of blocks
 * Number of commands 
 */
class SideMetrics : public Metrics{
public:
    /**
     * @brief Construct a new Side Metrics object
     * 
     * @param name name/ID for each metrics collector
     * @param ofs ostream for output information
     */
    SideMetrics(std::string name, std::ostream& ofs = std::cout);
    /**
     * @brief Collect metric information from loggers
     * 
     * @param pack pack of commands
     * @param nstr unused
     */
    void collect(const std::vector<std::string>& pack, [[maybe_unused]]size_t nstr=0) override;
    /**
     * @brief Show all collected information
     * ex:
     * metric_name thread - 2 blocks, 4 commands 
     * 
     */
    void display() const override;
    /// Call display()
    ~SideMetrics();
private:
    std::string name_;
    std::ostream& os;
    size_t n_cmds;
    size_t n_blocks;
};