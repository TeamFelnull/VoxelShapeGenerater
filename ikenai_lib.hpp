//
// Created by morimori on 2021/04/27.
//
#ifndef IKENAI_LIB_HPP
#define IKENAI_LIB_HPP

namespace ikenai {
    template<class T>
    inline bool vector_contains(std::vector <T> _vector, T chek_value) {
        for (const auto &item : _vector) {
            if (item == chek_value)
                return true;
        }
        return false;
    }
}

#endif //IKENAI_LIB_HPP
