/*
 * @Author: your name
 * @Date: 2020-10-01 15:25:09
 * @LastEditTime: 2020-10-03 21:01:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /gtest/2.cc
 */
#include <iostream>
template<int val>
class integer
{
public:
    using value = integer<val>;//萃取公式
    constexpr static int instance = val;
};

template<typename a,typename ...x>
class type_equal
{
public:
    constexpr static bool isequal = false;
};
template<typename a>
class type_equal<a,a>
{
public:
    constexpr static bool isequal = true;
};
template<bool,typename a>
class add_integer
{
public:
    using value = a;
};
template<typename a>
class add_integer<true,a>
{
public:
    using value = integer<a::instance + 1>;
};


template<typename val,typename ...x>
class type_count
{
public:
    using value = integer<0>;
};

template<typename val,typename a,typename ...x>
class type_count<val,a,x...> :public type_count<val,x...>
{
public:
    using parent = type_count<val,x...>;
    using value = typename add_integer<type_equal<val,a>::isequal,typename parent::value>::value;
    //这个过程是从后往前递推的过程 先进行的是double 的计算
};
int main( void )
{
    std::cout << type_count<int,int,char,int,int,double>::value::instance << std::endl;
    return 0;
}