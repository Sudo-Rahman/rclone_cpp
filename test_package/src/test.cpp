//
// Created by sr-71 on 05/02/2024.
//
#include <iostream>
#include <iridium/options.hpp>
#include <iridium/entities.hpp>
#include <iridium/parsers.hpp>
#include <iridium/process.hpp>

using namespace std;
using namespace iridium::rclone;
using namespace option;
using namespace parser;
using namespace entities;

process_pool pool{10};

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
	auto lst = std::vector<remote>{};
	auto n = new int{0};
	auto remote = remote::create_shared_ptr(
		"test", remote::remote_type::google_drive, "");
	std::cout << *remote << std::endl;
	auto file = ::file{
					nullptr, "/", 0, true, std::chrono::system_clock::now(),
					remote
			};
	rclone->add_option(listing::fast_list()).add_option(logging::use_json_log());
		//	{// filter::filter_file::uptr("+ TP4.pdf", "- *.pdf", "+ *.torrent", "- *"),
		//	listing::fast_list(),
		//	logging::use_json_log()
			// logging::verbose(),
			// logging::stats("0.1"),
			// filter::max_depth(1)
			//        option::logging::progress(),
			//        option::logging::use_json_log(),
			//        option::logging::verbose()
			// option::filter::include("*.pdf"),
			// option::filter::exclude("*")
		//	}
		//);

	std::vector<std::shared_ptr<::remote>> remotes;
	auto fn = [&](const std::vector<std::shared_ptr<::remote>> &val) -> void
	{
		remotes = val;
		std::cout << "remotes = " << remotes.size() << std::endl;
	};

	auto bureau = ::file(nullptr, "/Users/sr-71/Desktop", 0, true,
	                     std::chrono::system_clock::now(), nullptr);

	auto ser = ::file_parser::ptr(&file,
	                            [](const ::file &file)
	                            {
		                            std::cout << file << std::endl;
		                            // process().lsjson(file).execute().wait_for_finish();
	                            },   file_parser::json);

	auto parser = parser::json_log_parser::create(new parser::json_log_parser([](const json_log &log)
	{
		// std::cout << log << std::endl;
	}));

	auto parserr = parser::version_parser::ptr([](const version &version) { std::cout << version << std::endl; });

	rclone->
	//list_remotes(fn)
			// version()
			// check(bureau, file)
			//    copy_to(bureau, file)
			//			lsjson(bureau)
			lsjson(file)
			.every_line_parser<::file>(ser)
			//.on_finish_parser<::file>(ser)
			//            .about(*remote, [n](const about &about)
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
			//config()
			// config_create().name("gtg").type("drive").process()
			//    lsjson(iridium::rclone_remote("drive",
			//    iridium::rclone_remote::remote_type::google_drive, "/"))
			//.every_line([&](const std::string &line)
			//{
				//                *rclone << "q";
				// std::cout << line << std::endl;
				//                                std::cout << line <<
				//                                boost::this_thread::get_id() <<
				//                                std::endl << std::endl;
				//                            std::cout << boost::this_thread::get_id()
				//                            << std::endl;
			//})
			// .every_line_parser(parser)
			// .every_line_parser(ser)
			//            .on_finish_parser(parserr)
			//.on_stop([&]
			//{
			//	std::cout << "stop" << std::endl;
			//})
			.execute()
			// .wait_for_start();
			.wait_for_finish()
	// boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	// 		 rclone->stop()
			;
	cout << "n = " << *n << endl;
	// rclone->stop();

	std::function<void(::file &)> print;

	print = [&print](::file &file)
	{
		std::cout << file << std::endl;
		if (file.nb_chilchren() > 0) { for (const auto &f: file.children()) { print(*f); } }
	};
	for (const auto &remote: remotes) { cout << *remote << endl; }

	std::cout << rclone->commands() << std::endl;

	// print(file);

	auto counter = int{0};
	for (int i = 0; i < 10; ++i)
	{
		auto proc = std::make_unique<process>();
		proc->list_remotes()
				.on_finish([&](int exit) { std::cout << counter++ << std::endl; });
		pool.add_process(std::move(proc), process_pool::priority::low);
	}

	// this_thread::sleep_for(std::chrono::milliseconds(1000));

	pool.wait();

	delete rclone;
	delete n;
	return 0;
}
