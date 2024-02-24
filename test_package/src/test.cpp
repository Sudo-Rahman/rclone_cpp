//
// Created by sr-71 on 05/02/2024.
//
#include <boost/thread.hpp>
#include <iostream>
#include <iridium/entities.hpp>
#include <iridium/parsers/file_parser.hpp>
#include <iridium/rclone_cpp.hpp>

using namespace boost::asio;
using namespace std;
using namespace iridium::rclone;

int main()
{
    process::initialize();

    //    auto rclones = std::vector<iridium::rclone *>{};
    //    for (int i = 0; i < 1000; ++i)
    //    {
    //        auto rclone = new iridium::rclone{};
    //        rclones.push_back(rclone);
    //        rclone->version().execute();
    ////        delete rclone;
    //        rclone->stop();
    //        std::cout << "i = " << i << std::endl;
    //    }

    //    for (int i = 0; i < 10000; ++i)
    //    {
    auto rclone = new process();
    std::cout << boost::this_thread::get_id() << std::endl << std::endl;
    auto lst = std::vector<entitie::remote>{};
    auto n = new int{0};
    auto remote = entitie::remote::create_shared_ptr(
        "test", entitie::remote::remote_type::google_drive, "");
    auto file = entitie::file{
                    nullptr, "/", 0, true, boost::posix_time::second_clock::local_time(),
                    remote
            };
    process::add_global_option(
        option::performance::transfers(10)
    //            option::filter(option::filter::include, "*.txt")
    );
    std::vector<std::shared_ptr<entitie::remote>> remotes;
    auto fn = [&remotes](const std::vector<std::shared_ptr<entitie::remote>> &val)
    {
        remotes = val;
    };
    auto ser = parser::file_parser(&file,
                           [](entitie::file file)
                           {
                                 std::cout << file << std::endl;
                           });
    rclone->
                    list_remotes(fn)
//    lsjson(file)
//            .every_line_parser(ser)
            //            .about(*remote, [n](const entitie::about &about)
            //            {
            //                std::cout << about << std::endl;
            //
            //            })
            //            .add_option(
            //                    option::filter(option::filter::include, "*.txt")
            ////            option::tree(option::tree::dirs_only),
            //            )
            //                    about(*remote, [n](const iridium::rclone::about
            //                    &about)
            //                    {
            //                        std::cout << about << std::endl;
            //                        (*n)++;
            //                    })
            //                    lsjson(file, [&,n](const iridium::rclone::file
            //                    &file)
            //                    {
            //                        (*n)++;
            //                        std::cout << file << std::endl;
            //                    })
            //            list_remotes(lst)
            //    config()
            //    lsjson(iridium::rclone_remote("drive",
            //    iridium::rclone_remote::remote_type::google_drive, "/"))
//            .every_line([&](const std::string&line)
//            {
//                //                *rclone << "q";
////                 std::cout << line << std::endl;
//                //                                std::cout << line <<
//                //                                boost::this_thread::get_id() <<
//                //                                std::endl << std::endl;
//                //                            std::cout << boost::this_thread::get_id()
//                //                            << std::endl;
//            })
.every_line_parser(parser::file_parser(&file,
                                       [&ser](entitie::file file)
                                       {
                                            process().lsjson(file).every_line_parser(ser).execute().wait_for_finish();
                                       }))
            .execute()
            .wait_for_start()
            .wait_for_finish()
//            .stop()
            ;


    delete rclone;
    delete n;
    return 0;
}
