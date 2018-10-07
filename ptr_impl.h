#ifndef __PTR_IMPL_H__
#define __PTR_IMPL_H__
#include<memory>
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
#endif // __PTR_IMPL_H__
