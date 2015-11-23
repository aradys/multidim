#pragma once

//CRC
uint32_t get_crc32(const void* buffer, size_t size, uint32_t crc_divisor) {
    uint32_t crc = 0;
    uint32_t i = 0;
    const uint8_t *ptr = static_cast<const uint8_t *>(buffer);
    for (i; i <= (uint32_t)size - 4; i += 4) {
        crc = _mm_crc32_u32(crc, *reinterpret_cast<const uint32_t *>(ptr));
        ptr += 4;
    }
    for (i; i < (uint32_t)size; ++i) {
        crc = _mm_crc32_u32(crc, *(ptr++));
    }
    return crc;
}

//generating random containers
template<typename T_type> std::vector<T_type> random_vector(uint32_t size_of_vector, uint32_t range) {
    std::vector<T_type> result;
#if OPENMP
#pragma omp parallel for
#endif
    for (uint32_t i = 0; i < size_of_vector;++i) {
        result.push_back( rand() % range);
    }
    return result;
}