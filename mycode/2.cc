/*
 * @Author: your name
 * @Date: 2020-10-01 15:25:09
 * @LastEditTime: 2020-10-03 17:48:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /gtest/2.cc
 */
#include <iostream>
#include <tuple>//元素类型
#include <string>
namespace imp
{
    //----------------------------------------------------------------

    class Error
    {
    public:
        using value = Error;
    };
    //----------------------------------------------------------------
    //类型检查是否一致
    template<typename Type1,typename Type2>
    class Equeal_Type
    {
    public:
        constexpr static bool isequal = false;
    };
    template<typename Type1>
    class Equeal_Type<Type1,Type1>
    {
    public:
        constexpr static bool isequal = true;
    };
    //----------------------------------------------------------------
    //条件萃取
    template<typename Type>
    class Obj
    {
    public:
        using value = Type;
    };

    template<bool,typename,typename Type2>
    class Cond_Type
    {
    public:
        using value = typename Type2::value;//表明是个类型
    };//原模板
    template<typename Type1,typename Type2>
    class Cond_Type<true,Type1,Type2>//特化里面必须要在类后面有<>
    {
    public:
        using value = typename Type1::value;//这样就没有Obj的事情了 直接是对象的类型
    };//偏特化
    //----------------------------------------------------------------
    //tuple
    //tuple递归继承的出口 没有类型的时候  也是主模板 
    template<typename ...types>
    class tuple
    {

    };
    template<typename a,typename ...types>//模板特化 成其中一个类型 和一个包 会调用特化因为能分成一个a 和一个包 不能分成一个a的时候会调用主模板
    class tuple<a,types...> :public tuple<types...>
    {
    public:
        using Parent = tuple<types...>;
        using type = a;
        using self = tuple<a,types...>;
        using value = self;
        tuple() = default;

        template <typename b,typename ...k>//递归调用构造函数
        tuple( b value,k ... args ) :Parent( args... ),_val( value ) {}//构造函数 args是包参数的值3,3.14 k是包类型的值int double
        type _val;//值
    };
    //----------------------------------------------------------------
    //get 
    //1.通过 type 通过类型获取值 
    //2.通过index 通过索引获取值
    //----------------------------------------------------------------
    //1.通过 type 通过类型获取值 
    template<typename val,typename ...x>
    class tuple_traits_type
    {
    public:
        using value = Error;
    };
    template<typename val,typename a,typename ...x>//val 要萃取的类型 a 拆包的type  x参数包 就是在x类型包里面找到val
    class tuple_traits_type<val,a,x...> :public tuple_traits_type<val,x...>
    {
    public:
        using parent = tuple_traits_type<val,x...>;
        using type = a;
        using element = tuple<a,x...>;//是元组
        //萃取公式
        using cond = Equeal_Type<val,type>;
        using value = typename Cond_Type<cond::isequal,element,parent>::value;//如果是同一个类型 就返回element中的value成员就是当前类
        //value是等于的时候element那时候的状态 否则就是Error类 loop无线递归
    };
    //------------------------------
    template<typename a,typename ...x>
    constexpr typename tuple_traits_type<a,x...>::value::type getbytype( tuple <x...>& val )
    {
        using type = typename tuple_traits_type<a,x...>::value;
        return static_cast< type& >( val )._val;//val是最下层的子类要向上转型成父类获取当时_val的值
    }//常量表达式
    //----------------------------------------------------------------
    //2.通过index 通过索引获取值
    template<int index,typename ...x>
    class tuple_traits_index
    {
    public:
        using value = Error;
    };
    template<int index,typename a,typename ...x>
    class tuple_traits_index<index,a,x...> :public tuple_traits_index<index - 1,x...>
    {
    public:
        using parent = tuple_traits_index<index - 1,x...>;
        using type = a;
        using element = tuple<a,x...>;

        //萃取公式
        using value = typename Cond_Type<index == 0,element,parent>::value;

    };
    //----------------------------------------------------------------
    template<int index,typename ...x>
    constexpr typename tuple_traits_index<index,x...>::value::type getbyindex( tuple <x...>& val )
    {
        using type = typename tuple_traits_index<index,x...>::value;
        return static_cast< type& >( val )._val;//val是最下层的子类要向上转型成父类获取当时_val的值
    }//常量表达式
}

int main( void )
{
    imp::tuple<int,char,double> val( 3,'a',3.14 );
    // std::cout << val._val << std::endl;//3
    //     //可以向上转型
    // imp::tuple<char,double>& res = static_cast< imp::tuple<char,double>& >( val );
    // std::cout << res._val << std::endl;

    std::cout << imp::getbytype<char>( val ) << std::endl;
    //如果萃取一个没有的类型 会无线萃取到Error类型 loop没有尽头 报错

    std::cout << imp::getbyindex<0>( val ) << std::endl;
    return 0;
}
