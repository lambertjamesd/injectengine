#pragma once

namespace inject {
    template <std::size_t... Indices>
    struct indices {};

    template<std::size_t N, std::size_t... Is>
    struct build_indices : build_indices<N-1, N-1, Is...> {};

    template<std::size_t... Is>
    struct build_indices<0, Is...> : indices<Is...> {};
}