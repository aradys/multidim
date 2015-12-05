#pragma once

//generating random containers
template<typename T_type> std::vector<T_type> random_vector(int64_t size_of_vector, int64_t range) {
    std::vector<T_type> result;
    for (int64_t i = 0; i < size_of_vector; ++i) {
        result.push_back(rand() % range);
    }
    return result;
}