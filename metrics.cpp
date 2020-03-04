/**
 * @file metrics.cpp
 * @author SergeiNA (you@domain.com)
 * @brief Metrics classes implementation
 * @version 1.0
 * @date 2020-01-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "metrics.h"

MainMetrics::MainMetrics(std::ostream& ofs):
    os{ofs},n_str{0},n_cmds{0},n_blocks{0} {}
    
void MainMetrics::collect(const std::vector<std::string>& pack, size_t nstr){
    n_str+=nstr;
    if(pack.empty())
        return;
    ++n_blocks;
    n_cmds+=pack.size();
}

void MainMetrics::display() const{
    os<<"Main thread - "
    <<n_str<<" strings, "<<n_cmds<<" commands, "
    <<n_blocks<<" blocks"<<std::endl;
}

MainMetrics::~MainMetrics() {
    display();
}



SideMetrics::SideMetrics(std::string name, std::ostream& ofs ):
    name_{name},os{ofs},n_cmds{0},n_blocks{0} {}

void SideMetrics::collect(const std::vector<std::string>& pack, 
        [[maybe_unused]]size_t nstr){
    if(pack.empty())
        return;
    ++n_blocks;
    n_cmds+=pack.size();
}

void SideMetrics::display() const{
    os<<name_<<" thread - "
    <<n_blocks<<" blocks, "
    <<n_cmds<<" commands"<<std::endl;
}

SideMetrics::~SideMetrics(){
    display();
}