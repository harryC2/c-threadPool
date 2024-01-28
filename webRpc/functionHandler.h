/*
    此文件定义一个FunctionHandler类，运用bind与call公用方法，实现不同类型的函数绑定；
    利用std::map<string,std::function<void*(&1,&2)>>绑定对象
*/

#pragma once

#include <string>
#include "../serialize/dataStream.h"
using namespace  web_rpc::serialize;

#include <map>
#include <functional>
using namespace std;

namespace web_rpc{
namespace webrpc{

class FunctionHandler
{
public:
    FunctionHandler(){};
    ~FunctionHandler(){};
    template<class F>
    void bind(const std::string& funcName,F func);

    void call(const std::string& funcName,DataStream& in,DataStream& out);

private:
template<class F>
void  warp(F func,DataStream& in,DataStream& out);

template <typename R, typename... Args>
void wrap_impl(R (*func)(Args...), DataStream &in, DataStream &out);

template <typename R, typename... Args>
void wrap_impl(std::function<R(Args...)> func, DataStream &in, DataStream &out);

template <typename Tuple, std::size_t... I>
Tuple get_args(DataStream &ds, std::index_sequence<I...>);

template <typename Tuple, std::size_t Id>
void get_arg(DataStream &ds, Tuple &t);

template <typename R, typename F, typename Tuple>
typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
call_impl(F func, Tuple args);

template <typename R, typename F, typename Tuple>
typename std::enable_if<!std::is_same<R, void>::value, R>::type
call_impl(F func, Tuple args);

template <typename R, typename F, typename Tuple>
auto invoke(F &&func, Tuple &&t);

template <typename R, typename F, typename Tuple, std::size_t... Index>
auto invoke_impl(F &&func, Tuple &&t, std::index_sequence<Index...>);

private:
/* data */
std::map<std::string, std::function<void(DataStream &, DataStream &)>> m_handlers;
};

template<class F>
void FunctionHandler::bind(const std::string& funcName,F func)
{
    //F std::string (*func)(const std::string &name)
    m_handlers[funcName] = std::bind(&FunctionHandler::warp<F>,this,func,std::placeholders::_1,std::placeholders::_2);// 绑定了一个类的函数，并占用了两个参数占位符
}
template<class F>
void FunctionHandler::warp(F func,DataStream& in,DataStream& out)
{
    //F std::string (*func)(const std::string &name)
    wrap_impl(func,in,out);
}

//// wrap_impl函数模板，接受一个函数指针R (*func)(Args...)，以及两个DataStream类型的参数in和out。
//// 这是可变参数 直接封帐了一个函数指针，类型  参数都有了；
template <typename R, typename... Args>
void FunctionHandler::wrap_impl(R (*func)(Args...), DataStream &in, DataStream &out)
{
    //F std::string (*func)(const std::string &name)
    // 封装 返回值R,参数为args...的 函数
    wrap_impl(std::function<R(Args...)>(func), in, out);
}

//这个都不太像模板编程 这个就是 返回值与参数的类型
template <typename R, typename... Args>
void FunctionHandler::wrap_impl(std::function<R(Args...)> func, DataStream &in, DataStream &out)
{
    using args_type = std::tuple<typename std::decay<Args>::type...>;
    constexpr auto size = std::tuple_size<typename std::decay<args_type>::type>::value;
    args_type args = get_args<args_type>(in,std::make_index_sequence<size>{});
    auto ret = call_impl<R>(func,args);
    out<<ret;
}
template <typename Tuple, std::size_t... I>
Tuple FunctionHandler::get_args(DataStream &ds, std::index_sequence<I...>)
{
    Tuple t;
    initializer_list<int>{((get_arg<Tuple, I>(ds, t)), 0)...};
    return t;
}

template <typename Tuple, std::size_t Id>
void FunctionHandler::get_arg(DataStream &ds, Tuple &t)
{
    ds >> std::get<Id>(t);
}
/*
简化重写
// 将函数指针func包装成另一个函数的调用，并将结果写入到DataStream out中。
template <typename R, typename... Args>
void FunctionHandler::wrap_impl(R (*func)(Args...), DataStream &in, DataStream &out)
{
    // 创建一个参数类型为Args的tuple对象args。
    std::tuple<Args...> args;

    // 从DataStream in中逐个读取参数，并存储到args中。
    for (std::size_t i = 0; i < sizeof...(Args); ++i) {
        in >> std::get<i>(args);
    }

    // 调用函数指针func，并传入参数args，将返回值保存到ret中。
    R ret = func(std::get<Args>(args)...);

    // 将ret写入到DataStream out中。
    out << ret
*/
inline void FunctionHandler::call(const std::string & name, DataStream & in, DataStream & out)
{
    auto func = m_handlers[name];
    func(in, out);
}

template<typename R, typename F, typename Tuple>
typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
FunctionHandler::call_impl(F func, Tuple args)
{
    invoke<R>(func, args);
    return 0;
}

template<typename R, typename F, typename Tuple>
typename std::enable_if<!std::is_same<R, void>::value, R>::type
FunctionHandler::call_impl(F func, Tuple args)
{
    return invoke<R>(func, args);
}

template<typename R, typename F, typename Tuple>
auto FunctionHandler::invoke(F && func, Tuple && t)
{
    constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
    return invoke_impl<R>(std::forward<F>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}

template<typename R, typename F, typename Tuple, std::size_t... Index>
auto FunctionHandler::invoke_impl(F && func, Tuple && t, std::index_sequence<Index...>)
{
    return func(std::get<Index>(std::forward<Tuple>(t))...);
}



}
}