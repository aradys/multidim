#pragma once

#include <vector>
#include <cassert>
#include <cstring>

template <typename T_type, int T_dimension = 0> class data {
	public:
		//constructors
		data(int T_dimension) {
			dimension = T_dimension;
		}

		template<typename... T_types> data(T_type size, T_types... sizes) {
			assert(T_dimension == 0 || 1 + sizeof...(T_types) == T_dimension);
			dimension = 1 + sizeof... (T_types);
			t = { size, T_type(sizes)... };
		}

		data(const data &arg) {
			dimension = arg.dimension;
			for (int i = 0; i < dimension; ++i)
				t[i] = arg.t[i];
		}

		//operators
		data &operator=(const data &source) {
			if (dimension != source.dimension) {
				t.resize(source.dimension);
				dimension = source.dimension;
			}
			for (int i = 0; i < this->dimension; ++i) {
				this->t[i] = source.t[i];
			}
			return *this;
		}

		bool operator== (const data& arg) const
		{
			for (int i = 0; i < dimension; ++ i)
				if (t[i] != arg.t[i]) return false;
			return true;
		}

		bool operator!= (const data& arg) const
		{
			return !operator==(arg);
		}

		// CRC & load & save

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
		/*
		data<T_type> load_data_from_file(std::string filename) {
			try {
				std::ifstream file;
				file.exceptions(std::ios::failbit | std::ios::badbit);
				file.open(filename, std::ios::in | std::ios::binary);
				
				infile.seekg(0, infile.end);           //set the flag at the begining
				long size = infile.tellg();            //get size of the file
				infile.seekg(0);                       //set the flag at the begining
				char* buffer = new char[size];         //allocate memory

				auto data = std::unique_ptr<data<T_type>>(new data<T_type>(array.get(), file_head.dimension));
				auto data_size = data.get()->count()*sizeof(T_type);
				file.read(static_cast<char *>(data.get()->buffer), data_size);

				// return result
				auto result = data.get();
				data.release();
				return result;
			}
			catch (...) {
				return nullptr;
			}	
		}*/

		bool save_data_to_file(data<T_type> *in, std::string filename) { //dodaæ CRC
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
		}

		// get & set
		T_type get_single_data(int i) {
			return t[i];
		}

		void set_single_data(int i, T_type number) {
			t[i] = number;
		}

		std::vector<T_type> get_data() {
			return t;
		}

		//compare
		//not ready yet
		data& get_difference(data& arg1, data& arg2) {
			assert(arg1.dimension == arg2.dimension);
			dimension = arg1.dimension;
			for (int i = 0; i < dimension; ++i) {
				t[i] = arg1.t[i] - arg2.t[i];
			}
			return *this;
		}

		//destructor
		~data() {
		}

	private:
		int dimension;
		std::vector<T_type> t;
};