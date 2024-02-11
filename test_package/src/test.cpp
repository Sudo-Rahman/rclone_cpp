//
// Created by sr-71 on 05/02/2024.
//
#include <boost/thread.hpp>
#include <rclone.hpp>
#include <iostream>

using namespace boost::asio;
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

//    for (int i = 0; i < 10000; ++i)
//    {
    auto rclone = new Iridium::rclone();
    std::cout << boost::this_thread::get_id() << std::endl << std::endl;
    auto lst = std::vector<Iridium::rclone_remote>{};
    rclone->
            list_remotes(lst)
//    config()
//    lsjson(Iridium::rclone_remote("drive", Iridium::rclone_remote::remote_type::google_drive, "/"))
            .every_line([&](const std::string &line)
                        {
//                *rclone << "q";
                    std::cout << line << std::endl;
//                                std::cout << line << boost::this_thread::get_id() << std::endl << std::endl;
//                            std::cout << boost::this_thread::get_id() << std::endl;
                        })
            .finished([](int exit_code)
                      {
                          std::cout << "exit code: " << exit_code << std::endl;
                      })
            .execute()
                .wait_for_start()
                .wait_for_finish()
//            .stop()
            ;
    boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
    for (auto r: lst)
    {
        std::cout << r << std::endl;
    }
//    rclone->wait_for_finish();
//    std::cout << "i: " << i << std::endl;
//    boost::this_thread::sleep_for(boost::chrono::microseconds(100));
//rclone->stop();
//    std::cout << "exit code: " << rclone->exit_code() << std::endl;

//        delete rclone;
//    }
//    boost::this_thread::sleep_for(boost::chrono::seconds(100));

    return 0;
}