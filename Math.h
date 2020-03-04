/**
 * @file Math.h
 * @author SergeiNA (you@domain.com)
 * @brief Naive math for imitate CPU loading
 * @version 1.0
 * @date 2020-01-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once
#include <cstdint>

/**
 * @brief iterative factorial function
 * 
 * Limitation: 0 <= input number < 20
 * 
 * @param n input nubmber
 * @return std::uint64_t final factorial number
 */
std::uint64_t fact(int n);

/**
 * @brief recursive fibonacci function
 * 
 * Limitation: 0 < input number < 80
 * 
 * @param n input nubmber
 * @return std::uint64_t final fibonacci number
 */
std::uint64_t fib(int n);