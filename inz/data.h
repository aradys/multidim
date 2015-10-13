#pragma once
#include <new>
#include <stdexcept>
#include <utility>
#include <cstring>

typedef enum
{
	int8_t,
	uint8_t,
	int16_t,
	uint16_t,
	int32_t,
	uint32_t,
	int64_t,
	uint64_t,
	binary16,
	binary32,
	binary64
} T_type;

template <typename T_type, int T_dimension = 0> class data {
	public:
		int dimension;
		T_type *t;
		data(int T_dimension) {
			dimension = T_dimension;
			for (int i = 0; i < dimension; ++i) {
				t[i] = 0;
			}
		}

		template<typename... T_sizes> data(size_t size, T_sizes... sizes) {
			assert(T_dimension == 0 || 1 + sizeof...(T_sizes) == T_dimension, "invalid number of dimensions");
			dimension = 1 + sizeof... (T_sizes);
			t[0] = size;
			for (int i = 0; i < dimension; ++i) {
				t[i] = ...sizes;
			}
		}
	private:
};