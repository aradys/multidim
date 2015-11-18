#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <cstring>
#include <memory>
#include <assert.h>
#include <omp.h>

#define CRC_INIT 0xbaba7007
#define OPENMP true

template <typename T_type, size_t T_dimension = 0> class container {

    container() {}

    //structure used for loading and saving data
    //pragma to read exact number of bytes
#pragma pack(push,1)
    struct head {
        int        magic = 'CNT';                 // check if it is container
        int        data_type;                     // type of data
        uint8_t    version = 1;                   // 1
        size_t     dimension;                     // size of dimension vector
        uint8_t    sizeof_data_type;              // size of T_type
    };
#pragma pack(pop)

    std::vector<size_t> dimension;
    std::vector<T_type> data;
    head container_head;

    // set type and size of container data in head
    void set_type_in_head() {
        if (std::is_same<T_type, float>::value){
            container_head.data_type = 'F';
            container_head.sizeof_data_type = sizeof(float);
        }
        else if (std::is_same<T_type, int16_t>::value){
            container_head.data_type = 'I16';
            container_head.sizeof_data_type = sizeof(int16_t);
        }
    }

    // return count of elements in container
    size_t count() const {
        size_t result = 1;
        for (auto index = 0u; index < dimension.size(); ++index) {
            result *= dimension[index];
        }
        return result;
    }

    //function used in 2 types of loading
    container<T_type>* load_internal(std::string filename){
        try {
            std::ifstream file;
            file.exceptions(std::ios::failbit | std::ios::badbit);
            file.open(filename, std::ios::in | std::ios::binary);

            auto read_crc = [&file]() -> uint32_t {
                uint32_t result;
                file.read(reinterpret_cast<char *>(&result), sizeof(uint32_t));
                return result;
            };

            // load header, verify CRC
            file.read(reinterpret_cast<char *>(&container_head), sizeof(container_head));
            if (read_crc() != get_crc32(&container_head, sizeof(container_head), CRC_INIT)) throw("container header CRC mismatch");
            if (container_head.sizeof_data_type != sizeof(T_type)) throw("container has invalid type");

            // load size array, verify CRC
            dimension.resize(container_head.dimension);
            auto file_dimension_size = container_head.dimension * sizeof(size_t);
            file.read(reinterpret_cast<char *>(&dimension[0]), file_dimension_size);
            if (read_crc() != get_crc32(&dimension[0], file_dimension_size, CRC_INIT)) throw("container dimension CRC mismatch");

            // create container & load data into it
            data.resize(this->count());
            auto container_size = this->count() * sizeof(T_type);
            file.read(reinterpret_cast<char *>(&data[0]), container_size);
            if (read_crc() != get_crc32(&data[0], container_size, CRC_INIT)) throw("container data CRC mismatch");

            // return result
            auto result = this;
            return result;
        }
        catch (...) {
            return false;
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        //constructors

        container(std::string filename) {
            load_internal(filename);
        }

        container(size_t T_dimension) {
            dimension.resize(T_dimension);
        }

        template<typename... T_sizes> container(size_t size, T_sizes... sizes) {
            static_assert(T_dimension == 0 || 1 + sizeof...(T_sizes) == T_dimension, "invalid number of dimensions");
//            static_assert(are_size_t_compatible<T_type, T_types...>::value, "one of arguments cannot be treated as size_t");
            dimension.resize(1 + sizeof... (T_sizes));
            dimension = { size, size_t(sizes)... };
        }

        container(const container &arg) {
            container_head = arg.container_head;
            dimension = arg.dimension;
            data = arg.data;
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
            uint32_t i = 0;
            const uint8_t *ptr = static_cast<const uint8_t *>(buffer);
            for (i; i <= (uint32_t) size - 4; i += 4) {
                crc = _mm_crc32_u32(crc, *reinterpret_cast<const uint32_t *>(ptr));
                ptr += 4;
            }
            for (i; i < (uint32_t) size; ++i) {
                crc = _mm_crc32_u32(crc, *(ptr++));
            }
            return crc;
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //load

        static container &load_from_file(std::string filename) {
            container &result = *new container;
            try {
                result.load_internal(filename);
                return result;
            }
            catch (...) {
                delete[] &result;
            }
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //save

        bool save_to_file(std::string filename)
        {
            try {
                std::ofstream file;
                file.exceptions(std::ios::failbit | std::ios::badbit);
                file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);

                auto dimension_size = dimension.size();

                container_head.dimension = dimension_size;

                set_type_in_head();

                auto write_crc = [&file](uint32_t crc) -> void {
                    file.write(reinterpret_cast<char *>(&crc), sizeof(uint32_t));
                };

                // write header with 32-bit crc
                file.write(reinterpret_cast<const char *>(&container_head), sizeof(container_head));
                write_crc(get_crc32(&container_head, sizeof(container_head), CRC_INIT));

                // write size array with 32-bit crc
                const char *file_dimension = reinterpret_cast<const char *>(&(dimension[0]));
                const auto file_dimension_size = dimension_size * sizeof(size_t);
                file.write(file_dimension, file_dimension_size);
                write_crc(get_crc32(file_dimension, file_dimension_size, CRC_INIT));

                // write data with 32-bit crc
                size_t buffer_size = count()*sizeof(T_type);
                file.write(reinterpret_cast<char *>(&(data[0])), buffer_size);
                write_crc(get_crc32(&(data[0]), buffer_size, CRC_INIT));
            }
            catch (...) {
                return false;
            }
            return true;
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // get & set

        template<typename... T_data> void set_data(T_type d, T_data... ds){
            size_t data_number = 0;
            if (dimension.size() != 0){
                data_number = this->count();
            }
            static_assert(T_dimension == 0 || 1 + sizeof...(T_data) == data_number, "different number of dimensions");
            data = {d, T_type(ds)...};
        }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //compare

        //get difference between every data in 2 containers
        container& get_difference(container& arg) {
            container &result = *new container;
            assert(dimension == arg.dimension);
            assert(typeid(data[0]) == typeid(arg.data[0]));

            int data_dim = (int) this->count();
            result.data.resize(data_dim);
            result.dimension = dimension;
#if OPENMP
#pragma omp parallel for
#endif
            for (int i = 0; i < data_dim; ++i) {
                result.data[i] = data[i] - arg.data[i];
            }
            return result;
        }

        //get mean error
        double get_mean_error(container& arg) {
            assert(dimension == arg.dimension);
            assert(typeid(data[0]) == typeid(arg.data[0]));

            double mean_error = 0, sum = 0;
            int data_dim = (int) this->count();
#if OPENMP
#pragma omp parallel for
#endif
            for (int i = 0; i < data_dim; ++i) {
                sum += std::abs(data[i] - arg.data[i]);
            }

            mean_error = sum / data_dim;

            return mean_error;
        }

        //get max absolute error of all data of 2 containers
        double get_max_absolute_error(container& arg){
            assert(dimension == arg.dimension);
            assert(typeid(data[0]) == typeid(arg.data[0]));

            double max_absolute_error = 0, absoulte_error = 0;
            int data_dim = (int) this->count();
#if OPENMP
#pragma omp parallel for
#endif
            for (int i = 0; i < data_dim; ++i) {
                absoulte_error = std::abs(data[i] - arg.data[i]);
                if (absoulte_error > max_absolute_error) max_absolute_error = absoulte_error;
            }

            return max_absolute_error;
        }

        //get standard deviation
        double get_std_dev(container& arg) {
            assert(dimension == arg.dimension);
            assert(typeid(data[0]) == typeid(arg.data[0]));

            double std_dev = 0, mean = this->get_mean_error(arg), tmp = 0;
            int data_dim = (int) this->count();
#if OPENMP
#pragma omp parallel for
#endif
            for (int i = 0; i < data_dim; ++i) {
                tmp += pow(((data[i] - arg.data[i]) - mean), 2);
            }

            return std_dev = pow( tmp / data_dim, 0.5);
        }


        //draw histogram in html
        void draw_histogram(std::string filename, float bucket_size){
            std::ofstream myfile;
            int data_dim = (int) this->count();
            myfile.open(filename);
            myfile << "<html><head><script type = \"text/javascript\" src = \"https://www.google.com/jsapi\"></script><script type = \"text/javascript\"> google.load(\"visualization\", \"1\", { packages:[\"corechart\"] }); google.setOnLoadCallback(drawChart);";
            myfile << "function drawChart() {var data = google.visualization.arrayToDataTable([['', 'Value']";
#if OPENMP
#pragma omp parallel for
#endif
            for (int i = 0; i < data_dim; i++)
                myfile << ",[, " << data[i] << "]";
            myfile << "]); var options = {title: 'Histogram',legend : { position: 'none' },histogram: { bucketSize: " << bucket_size <<" }, orientation: 'vertical',};";
            myfile << "var chart = new google.visualization.Histogram(document.getElementById('chart_div'));chart.draw(data, options);}</script></head><body><div id = \"chart_div\" style = \"width: 900px; height: 500px;\"></div>";
            myfile << "</body></html>";
            myfile.close();
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //destructor

        ~container() {
        }
};