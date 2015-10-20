#pragma once

#include <stdint.h>
#include <vector>
#include <cstring>

template <typename T_type, size_t T_dimension = 0> class container {

/*    template<typename T_first, typename... T_rest> struct are_size_t_compatible{
        static const bool value = are_size_t_compatible<T_first>::value && are_size_t_compatible<T_rest...>::value;
    };*/

    struct head {
        uint8_t    magic[3];             // check if it is container
        uint8_t    data_type;            // type of data
        uint8_t    version;              // 1
        size_t     dimension;            // 1
    };

    std::vector<T_type> dimension;
    std::vector<T_type> data;
    static head container_head;

    public:

        //constructors

        container(size_t T_dimension) {
            dimension.resize(T_dimension);
        }

        template<typename... T_types> container(T_type size, T_types... sizes) {
            static_assert(T_dimension == 0 || 1 + sizeof...(T_types) == T_dimension, "invalid number of dimensions");
//            static_assert(are_size_t_compatible<T_type, T_types...>::value, "one of arguments cannot be treated as size_t");
            dimension.resize(1 + sizeof... (T_types));
            dimension = { size, T_type(sizes)... };
        }

        container(const container &arg) {
            dimension = arg.dimension;
            for (int i = 0; i < arg.data.size; ++i)
                data[i] = arg.data[i];
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //operators

        container &operator=(const container &source) {
            if (dimension != source.dimension) {
                dimension.resize(source.dimension);
                dimension = source.dimension;
            }
            for (int i = 0; i < data.size; ++i) {
                data[i] = source.data[i];
            }
            return *this;
        }

        bool operator== (const container& arg) const
        {
            if (dimension != arg.dimension) {
                return false;
            } else {
                for (int i = 0; i < data.size; ++i)
                    if (data[i] != arg.data[i]) return false;
            }
            return true;
        }

        bool operator!= (const container& arg) const
        {
            return !operator==(arg);
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CRC

        uint32_t get_crc32(const void* buffer, size_t size, uint32_t crc_divisor) {
            uint32_t crc = 0;
            const uint8_t *ptr = static_cast<const uint8_t *>(buffer);
            for (int i = 0; i <= size - 4; i =+ 4) {
                crc = _mm_crc32_u32(crc, *reinterpret_cast<const uint32_t *>(ptr));
                ptr += 4;
            }
            for (i, i <= size, ++i) {
                crc = _mm_crc32_u32(crc, *(ptr++));
            }
            return crc;
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //load

        /*
        container<T_type> load_container_from_file(std::string filename) {
            try {
                std::ifstream file;
                file.exceptions(std::ios::failbit | std::ios::badbit);
                file.open(filename, std::ios::in | std::ios::binary);
                
                infile.seekg(0, infile.end);           //set the flag at the begining
                long size = infile.tellg();            //get size of the file
                infile.seekg(0);                       //set the flag at the begining
                char* buffer = new char[size];         //allocate memory

                auto container = std::unique_ptr<container<T_type>>(new container<T_type>(array.get(), file_head.dimension));
                auto container_size = container.get()->count()*sizeof(T_type);
                file.read(static_cast<char *>(container.get()->buffer), container_size);

                // return result
                auto result = container.get();
                container.release();
                return result;
            }
            catch (...) {
                return nullptr;
            }    
        }*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //save

        /*
        bool save_container_to_file(container<T_type> *in, std::string filename) { //dodaæ CRC
            try {
                std::ofstream file;
                file.exceptions(std::ios::failbit | std::ios::badbit);
                file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);

                size_t buffer_size = 1;
                for (int i; i < in->dimension, ++i){
                    buffer_size *= in->t[i];
                }
                
                file.write(static_cast<char *>(in->buffer), buffer_size);
            }
            catch(...) {
                return false;
            }
            return true;
        }*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // get & set

        //not ready at all


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //compare

        //not ready yet
        container& get_difference(container& arg1, container& arg2) {
            static_assert(arg1.dimension == arg2.dimension, "dimensions are different, cannot compare");
            for (int i = 0; i < dimension; ++i) {
                data[i] = arg1.data[i] - arg2.data[i];
            }
            return *this;
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //destructor

        ~container() {
        }
};