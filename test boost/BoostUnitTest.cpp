#include "stdafx.h"
#include "../inz/container.h"

BOOST_AUTO_TEST_CASE(test_dimension_size)
{
    container<int16_t> test_container(1, 2, 3);

    BOOST_CHECK(test_container.get_dimension_size() == 3);
}
BOOST_AUTO_TEST_CASE(test_set_data)
{
    container<int16_t> test_container(1, 2, 3);
    test_container.set_data(1, 2, 3, 4, 5, 6);

    BOOST_CHECK(test_container.get_one_data(3) == 4);
}
BOOST_AUTO_TEST_CASE(test_data_size)
{
    container<int16_t> test_container(1, 2, 3);
    test_container.set_data(1, 2, 3, 4, 5, 6);

    BOOST_CHECK(test_container.get_data_size() == 6);
}
BOOST_AUTO_TEST_CASE(test_set_data_from_vector)
{
    container<int16_t> test_container(5, 5, 5);
    std::vector<int16_t> test_vector;
    test_vector = random_vector<int16_t>(125, 10);
    test_container.set_data_from_vector(test_vector);

    BOOST_CHECK(test_container.get_dimension_size() == 3);
    BOOST_CHECK(test_container.get_data_size() == 125);
}
BOOST_AUTO_TEST_CASE(test_load_save)
{
    container<int16_t> test_container(1, 2, 3);
    test_container.set_data(9, 8, 7, 6, 5, 4);
    bool saving = test_container.save_to_file("container");

    auto loaded = container<int16_t>::load_from_file("container");
    container<int16_t> constructed("container");

    BOOST_CHECK(saving == 1);
    BOOST_CHECK(loaded == test_container);
    BOOST_CHECK(constructed == test_container);
}
BOOST_AUTO_TEST_CASE(test_operators)
{
    container<int16_t> test_container(1, 2, 3);
    test_container.set_data(9, 8, 7, 6, 5, 4);

    container<int16_t> changed(1, 2, 3);
    changed.set_data(0, 8, 7, 6, 5, 4);

    container<int16_t> smaller(1, 2);
    smaller.set_data(0, 8);

    container<int16_t> copied = test_container;

    BOOST_CHECK(copied == test_container);
    BOOST_CHECK(changed != test_container);
    BOOST_CHECK(smaller != test_container);
}
BOOST_AUTO_TEST_CASE(test_get_difference)
{
    container<int16_t> test_container_1(1, 2, 3);
    test_container_1.set_data(9, 8, 7, 6, 5, 4);

    container<int16_t> test_container_2(1, 2, 3);
    test_container_2.set_data(9, 8, 7, 6, 5, 1);

    container<int16_t> diff(1, 2, 3);
    diff.set_data(0, 0, 0, 0, 0, 3);

    container<int16_t> get_diff = test_container_1.get_difference(test_container_2);

    BOOST_CHECK(get_diff == diff);
}
BOOST_AUTO_TEST_CASE(test_get_max_absolute_error)
{
    container<int16_t> test_container_1(1, 2, 3);
    test_container_1.set_data(9, 8, 7, 6, 5, 4);

    container<int16_t> test_container_2(1, 2, 3);
    test_container_2.set_data(7, 6, 5, 5, 4, 3);

    double max_absolute_error = 2;

    double get_max_absolute_error = test_container_2.get_max_absolute_error(test_container_1);

    BOOST_CHECK(get_max_absolute_error == max_absolute_error);
}
BOOST_AUTO_TEST_CASE(test_get_std_dev)
{
    container<int16_t> test_container_1(1, 2, 3);
    test_container_1.set_data(9, 8, 7, 6, 5, 4);

    container<int16_t> test_container_2(1, 2, 3);
    test_container_2.set_data(7, 6, 5, 5, 4, 3);

    double std_dev = 0.5;

    double get_std_dev = test_container_1.get_std_dev(test_container_2);

    BOOST_CHECK(get_std_dev == std_dev);
}