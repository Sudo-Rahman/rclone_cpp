//
// Created by sr-71 on 05/02/2024.
//
#include <boost/thread.hpp>
#include <iostream>
#include <iridium/entities.hpp>
#include <iridium/parsers/file_parser.hpp>
#include <iridium/process.hpp>

using namespace boost::asio;
using namespace std;
using namespace iridium::rclone;

auto main() -> int
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
    std::mutex m;
	auto fn = [&](const std::vector<std::shared_ptr<entitie::remote>>& val) {
//        std::lock_guard<std::mutex> lock(m);
//        remotes = val;
    };
	auto ser = parser::file_parser(&file,
	                               [](const entitie::file&)
	                               {
		                               // std::cout << file << std::endl;
	                               });
	rclone->
			list_remotes(fn)
			// lsjson(file)
//			 .every_line_parser(ser)
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
			                                       [&ser](const entitie::file& file)
			                                       {
				                                       process().lsjson(file).every_line_parser(ser).execute().
						                                       wait_for_finish();
			                                       }))
			.execute()
			.wait_for_start()
			.wait_for_finish()
//			            .stop()
			;

	std::function<void(entitie::file&)> print;

	print = [&print](entitie::file& file)
	{
		std::cout << file << std::endl;
		if (file.nb_chilchren() > 0) { for (const auto& f: file.children()) { print(*f); } }
	};
	for (const auto &remote : remotes)
	{
		cout << *remote << endl;
	}

	print(file);

    process_pool pool{10};

    for (int i = 0; i < 100; ++i)
    {
        auto proc = new process();
        proc->list_remotes().every_line([&](const std::string& line) { std::cout << line << std::endl; });
        pool.add_process(std::unique_ptr<process>(proc));
    }

    std::cout << "stop" << std::endl;

    pool.wait();

	delete rclone;
	delete n;
	return 0;
}
