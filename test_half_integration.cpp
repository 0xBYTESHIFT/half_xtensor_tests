#include <iostream>

#include "half/half.hpp"
using half_float::half;

namespace xtl{

inline half select(const bool& cond, const half& v1, const half& v2) noexcept {
	return cond ? v1 : v2;
}

template<class T>
inline std::common_type_t<T, half> select(const bool& cond, const T& v1, const half& v2) noexcept {
	return cond ? v1 : v2;
}

template<class T>
inline std::common_type_t<half, T> select(const bool& cond, const half& v1, const T& v2) noexcept {
	return cond ? v1 : v2;
}

}

#include <xtensor/xarray.hpp>
#include <xtensor/xpad.hpp>
#include <xtensor/xslice.hpp>
#include <xtensor/xmasked_view.hpp>
#include <xtensor/xio.hpp>
#include <vector>
#include <chrono>

int main() {
    using half_float::half;

    half h_flt = (float)0;
    half h_dpl = (double)0;
    half h_int = (int)0;
    half h_int2 = 0;
    using xarr = xt::xarray<half>;
    xarr arange = xt::arange<half>((half)0, (half)1024, (half)1);
    std::cout << arange << std::endl;
    {
    xarr xarr00 = arange/2;
    xarr xarr01 = arange/2.f;
    xarr xarr10 = xarr00*2;
    xarr xarr11 = xarr01*2.f;
    assert(xarr00 == xarr01);
    assert(xarr10 == xarr11 == arange);
    std::cout << xarr00 << std::endl;
    std::cout << xarr11 << std::endl;
    }
    auto zeroes = xt::eval(xt::equal(arange, (half)0));
    auto cast = xt::eval(xt::cast<int>(arange));
    xarr whered = xt::where(xt::equal(cast%3, 0), (half)0, arange);
    auto masked = xt::masked_view(arange, arange <= xt::mean(arange));
    assert(zeroes.at(0));
    assert(cast.at(0) == 0 && cast.at(1) == 1);
    assert(whered.at(3) == 0 && whered.at(2) != 0);
    std::cout << zeroes << std::endl;
    std::cout << cast << std::endl;
    std::cout << whered << std::endl;
    std::cout << masked << std::endl;
    return 0;
}