#include "container.h"
#include <algorithm>
#include <chrono>

int main() {

    container<float,3> container1(3,3,3);
    container<int16_t> container2(1,2,3);
    container2.set_data(9,8,7,6,5,4);
    container<int16_t> container3(container2);

    //saving & loading
    bool saving = container2.save_to_file("container2");

    auto myf1 = container<int16_t>::load_from_file("container2");
    container<int16_t> myf2("container2");

    //big container testing
    const auto time_1 = std::chrono::high_resolution_clock::now();

    container<int16_t> kontener1(10, 1, 1);
    std::vector<int16_t> vector1(10);
    std::generate(vector1.begin(), vector1.end(), std::rand);
    kontener1.set_data_from_vector(vector1);

    const auto time_2 = std::chrono::high_resolution_clock::now();

    container<int16_t> kontener2(10, 1, 1);
    std::vector<int16_t> wektor2(10);
    std::generate(wektor2.begin(), wektor2.end(), std::rand);
    kontener2.set_data_from_vector(wektor2);

    const auto time_3 = std::chrono::high_resolution_clock::now();

    //container for testing histogram
    container<int16_t> container100(2, 2, 5, 5);
    container100.set_data(62, 71, 36, 37, 40, 52, 21, 28, 24, 18, 8, 81, 18, 40, 23, 39, 66, 88, 15, 89, 42, 76, 12, 74, 44, 59, 72, 82, 76, 93, 50, 36, 50, 29, 62, 71, 36, 70, 86, 15, 54, 7, 75, 27, 65, 31, 51, 65, 96, 68, 69, 4, 7, 16, 28, 45, 66, 92, 10, 96, 13, 54, 22, 25, 19, 52, 87, 99, 96, 80, 51, 97, 34, 22, 4, 99, 11, 41, 50, 68, 93, 67, 13, 66, 48, 61, 55, 39, 99, 38, 52, 27, 75, 39, 45, 44, 51, 18, 91, 67);

    const auto time1 = std::chrono::high_resolution_clock::now();

    auto diff = kontener1.get_difference(kontener2);

    const auto time2 = std::chrono::high_resolution_clock::now();

    auto abs_err = kontener1.get_max_absolute_error(kontener2);

    const auto time3 = std::chrono::high_resolution_clock::now();

    auto mean_err = kontener1.get_mean_error(kontener2);

    const auto time4 = std::chrono::high_resolution_clock::now();

    auto std_dev = kontener1.get_std_dev(kontener2);

    const auto time5 = std::chrono::high_resolution_clock::now();

    container100.draw_histogram("histogram.html");

    const auto time6 = std::chrono::high_resolution_clock::now();

    container100.draw_linear("linear.html");

    const auto time7 = std::chrono::high_resolution_clock::now();

    std::ofstream wyniki;
    wyniki.open("wyniki.txt");

    wyniki << "random_vector 1: " << std::chrono::duration<double, std::milli>(time_2 - time_1).count() << "\n";
    wyniki << "random_vector 2: " << std::chrono::duration<double, std::milli>(time_3 - time_2).count() << "\n";
    wyniki << "get_difference: " << std::chrono::duration<double, std::milli>(time2-time1).count() << "\n";
    wyniki << "get_max_absolute_error: " << std::chrono::duration<double, std::milli>(time3 - time2).count() << "\n";
    wyniki << "get_mean_error: " << std::chrono::duration<double, std::milli>(time4 - time3).count() << "\n";
    wyniki << "get_std_dev: " << std::chrono::duration<double, std::milli>(time5 - time4).count() << "\n";
    wyniki << "draw_histogram: " << std::chrono::duration<double, std::milli>(time6 - time5).count() << "\n";
    wyniki << "draw_linear: " << std::chrono::duration<double, std::milli>(time7 - time6).count() << "\n";

    system("pause");
    return 0;
}
