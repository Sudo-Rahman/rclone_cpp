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

//    auto rclones = std::vector<Iridium::rclone *>{};
//    for (int i = 0; i < 1000; ++i)
//    {
//        auto rclone = new Iridium::rclone{};
//        rclones.push_back(rclone);
//        rclone->version().execute();
////        delete rclone;
//        rclone->stop();
//        std::cout << "i = " << i << std::endl;
//    }

    for (int i = 0; i < 10000; ++i)
    {
        auto rclone = new Iridium::rclone;
    rclone->config().execute().wait_for_start();
    *rclone << "n" << Iridium::rclone::endl;
//    boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
    *rclone << "test\n";
//    rclone->wait_for_finish();
//    std::cout << "exit code: " << rclone->exit_code() << std::endl;
//    boost::this_thread::sleep_for(boost::chrono::microseconds(100));
//rclone->stop();
//    std::cout << "exit code: " << rclone->exit_code() << std::endl;

    delete rclone;
    }
    boost::this_thread::sleep_for(boost::chrono::seconds (100));

    return 0;
}