/**
 * @file Math.cpp
 * @author SergeiNA (you@domain.com)
 * @brief Implementation of fact and fib fuctions
 * 
 * Both fuction have limitation and can throw invalid_argument
 * 
 * @version 1.0
 * @date 2020-01-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Math.h"

#include <stdexcept>

std::uint64_t fact(int n){
    if(n == 0 || n == 1)
        return 1;
    if(n<0 || n>20)
        throw std::invalid_argument("Factorial require argument 0<=arg<20");
    std::uint64_t result =1;
    for(int i = 2; i<=n; ++i)
        result*=i;
    return result;
}

std::uint64_t fib(int n){
    if(n == 1 || n == 2)
        return 1;
    if(n<=0 || n>80)
        throw std::invalid_argument("This Fibonacci function require 0<arg<80");
    return fib(n-1) + fib(n-2);
}