#include "container.h"
#include <algorithm>
#include <chrono>

int main() {
    /*
    container<float,3> container1(3);
    container<int16_t> container2(1,2,3);
    container2.set_data(9,8,7,6,5,4);
    container<int16_t> container3(container2);

    //saving & loading
    bool saving = container2.save_to_file("container2");

    auto myf1 = container<int16_t>::load_from_file("container2");
    container<int16_t> myf2("container2");
*/
    //big container testing
    const auto time_1 = std::chrono::high_resolution_clock::now();

    container<int16_t> container1000_a(1000, 1000, 100);
    std::vector<int16_t> vector1000_a = random_vector<int16_t>(100000000, 100);
    container1000_a.set_data_from_vector(vector1000_a);
    std::cout << vector1000_a.max_size() << "\n";

    const auto time_2 = std::chrono::high_resolution_clock::now();

    container<int16_t> container1000_b(1000, 1000, 100);
    std::vector<int16_t> myVector(100000000);
    std::generate(myVector.begin(), myVector.end(), std::rand);
    container1000_b.set_data_from_vector(myVector);

    const auto time_3 = std::chrono::high_resolution_clock::now();

    //container for testing histogram
    container<int16_t> container100(2, 2, 5, 5);
    container100.set_data(62, 71, 36, 37, 40, 52, 21, 28, 24, 18, 8, 81, 18, 40, 23, 39, 66, 88, 15, 89, 42, 76, 12, 74, 44, 59, 72, 82, 76, 93, 50, 36, 50, 29, 62, 71, 36, 70, 86, 15, 54, 7, 75, 27, 65, 31, 51, 65, 96, 68, 69, 4, 7, 16, 28, 45, 66, 92, 10, 96, 13, 54, 22, 25, 19, 52, 87, 99, 96, 80, 51, 97, 34, 22, 4, 99, 11, 41, 50, 68, 93, 67, 13, 66, 48, 61, 55, 39, 99, 38, 52, 27, 75, 39, 45, 44, 51, 18, 91, 67);

    const auto time1 = std::chrono::high_resolution_clock::now();

    auto diff = container1000_a.get_difference(container1000_b);

    const auto time2 = std::chrono::high_resolution_clock::now();

    auto abs_err = container1000_a.get_max_absolute_error(container1000_b);

    const auto time3 = std::chrono::high_resolution_clock::now();

    auto mean_err = container1000_a.get_mean_error(container1000_b);

    const auto time4 = std::chrono::high_resolution_clock::now();

    auto std_dev = container1000_a.get_std_dev(container1000_b);

    const auto time5 = std::chrono::high_resolution_clock::now();

    container1000_a.draw_histogram("histogram1.html", 7.5);

    const auto time6 = std::chrono::high_resolution_clock::now();

    std::cout << "random_vector 1: " << std::chrono::duration<double, std::milli>(time_2 - time_1).count() << ".\n";
    std::cout << "random_vector 2: " << std::chrono::duration<double, std::milli>(time_3 - time_2).count() << ".\n";
    std::cout << "get_difference: " << std::chrono::duration<double, std::milli>(time2-time1).count() << ".\n";
    std::cout << "get_max_absolute_error: " << std::chrono::duration<double, std::milli>(time3 - time2).count() << ".\n";
    std::cout << "get_mean_error: " << std::chrono::duration<double, std::milli>(time4 - time3).count() << ".\n";
    std::cout << "get_std_dev: " << std::chrono::duration<double, std::milli>(time5 - time4).count() << ".\n";
    std::cout << "draw_histogram: " << std::chrono::duration<double, std::milli>(time6 - time5).count() << ".\n";

    system("pause");
    return 0;
}