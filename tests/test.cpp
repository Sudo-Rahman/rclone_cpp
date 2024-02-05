//
// Created by sr-71 on 05/02/2024.
//
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <rclone.hpp>
#include <iostream>

using namespace boost::asio;
using namespace std;



int main()
{

    Iridium::rclone::initialize("/opt/homebrew/bin/rclone");

    auto rclones = std::vector<Iridium::rclone *>{};
    for (int i = 0; i < 10000; ++i)
    {
        auto rclone = new Iridium::rclone{};
        rclones.push_back(rclone);
        rclone->version().execute().wait_for_start().wait_for_finish();
        std::cout << "i = " << i << std::endl;
    }


    boost::this_thread::sleep_for(boost::chrono::seconds(1));


    return 0;
}