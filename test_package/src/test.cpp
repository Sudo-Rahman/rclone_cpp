//
// Created by sr-71 on 05/02/2024.
//
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <rclone_cpp.hpp>
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
    auto n = new int {0};
    auto remote = Iridium::rclone_remote::create_shared_ptr("test", Iridium::rclone_remote::remote_type::google_drive, "");
    auto file = Iridium::rclone_file{nullptr, "/", 0, true, boost::posix_time::second_clock::local_time(), remote};
    rclone->
                    lsjson(file, [&,n](const Iridium::rclone_file &file)
                    {
                        (*n)++;
                        std::cout << file << std::endl;
                    })
//            list_remotes(lst)
//    config()
//    lsjson(Iridium::rclone_remote("drive", Iridium::rclone_remote::remote_type::google_drive, "/"))
//            .every_line([&](const std::string &line)
//                        {
////                *rclone << "q";
////                    std::cout << line << std::endl;
////                                std::cout << line << boost::this_thread::get_id() << std::endl << std::endl;
////                            std::cout << boost::this_thread::get_id() << std::endl;
//                        })
            .finished([&file](int exit_code)
                      {
//                          for (auto r: file.children())
//                          {
//                              std::cout << r->path() << std::endl;
//                          }
//                          std::cout << file.children().size() << std::endl;
                          std::cout << boost::this_thread::get_id() << std::endl;
                      })
            .execute()
                .wait_for_start()
                .wait_for_finish()
//            .stop()
            ;
    std::cout << sizeof(bool) << std::endl;
    boost::this_thread::sleep_for(boost::chrono::microseconds(10000));
//    rclone->wait_for_finish();
//    std::cout << "i: " << i << std::endl;
//    boost::this_thread::sleep_for(boost::chrono::microseconds(100));
//rclone->stop();
//    std::cout << "exit code: " << rclone->exit_code() << std::endl;

//        delete rclone;
//    }
//    boost::this_thread::sleep_for(boost::chrono::seconds(100));

    delete n;
    return 0;
}