#define BOOST_TEST_MODULE JsonLogTest

#include <boost/test/included/unit_test.hpp>
#include <iridium/entities.hpp>
#include <string>
#include <chrono>

using namespace iridium::rclone::entities;

// Helper function to create a sample json_log object
auto create_sample_json_log() -> json_log
{
	json_log log;
	log.set_level(json_log::log_level::info);
	log.set_message("Test message");
	log.set_source("Test source");
	log.set_object("Test object");
	log.set_object_type("Test object type");
	log.set_time(std::chrono::system_clock::now());
	log.set_stats(std::make_unique<json_log::stats>());
	return log;
}

BOOST_AUTO_TEST_SUITE(Suite)

	BOOST_AUTO_TEST_CASE(test_setters)
	{
		json_log log;

		log.set_level(json_log::log_level::info);
		BOOST_TEST(log.level() == json_log::log_level::info);

		log.set_message("Test message");
		BOOST_TEST(log.message() == "Test message");

		log.set_source("Test source");
		BOOST_TEST(log.source() == "Test source");

		log.set_object("Test object");
		BOOST_TEST(log.object() == "Test object");

		log.set_object_type("Test object type");
		BOOST_TEST(log.object_type() == "Test object type");

		auto now = std::chrono::system_clock::now();
		log.set_time(now);
		BOOST_TEST(log.time() == now);

		auto stats = std::make_unique<json_log::stats>();
		auto pointer = stats.get();
		log.set_stats(std::move(stats));
		BOOST_TEST(log.get_stats() == pointer);
	}

	BOOST_AUTO_TEST_CASE(test_default_constructor)
	{
		json_log log;
		BOOST_TEST(log.level() == json_log::log_level::no_level);
		BOOST_TEST(log.message() == "");
		BOOST_TEST(log.source() == "");
		BOOST_TEST(log.object() == "");
		BOOST_TEST(log.object_type() == "");
		BOOST_TEST(log.get_stats() == nullptr);
	}

	BOOST_AUTO_TEST_CASE(test_copy_constructor)
	{
		json_log log1 = create_sample_json_log();
		json_log log2 = log1;

		BOOST_TEST(log2.level() == log1.level());
		BOOST_TEST(log2.message() == log1.message());
		BOOST_TEST(log2.source() == log1.source());
		BOOST_TEST(log2.object() == log1.object());
		BOOST_TEST(log2.object_type() == log1.object_type());
		BOOST_TEST(log2.get_stats() != nullptr);
	}

	BOOST_AUTO_TEST_CASE(test_copy_assignment_operator)
	{
		json_log log1 = create_sample_json_log();
		json_log log2;
		log2 = log1;

		BOOST_TEST(log2.level() == log1.level());
		BOOST_TEST(log2.message() == log1.message());
		BOOST_TEST(log2.source() == log1.source());
		BOOST_TEST(log2.object() == log1.object());
		BOOST_TEST(log2.object_type() == log1.object_type());
		BOOST_TEST(log2.get_stats() != nullptr);
	}

	BOOST_AUTO_TEST_CASE(test_move_constructor)
	{
		json_log log1 = create_sample_json_log();
		json_log log2 = std::move(log1);

		BOOST_TEST(log2.level() == json_log::log_level::info);
		BOOST_TEST(log2.message() == "Test message");
		BOOST_TEST(log2.source() == "Test source");
		BOOST_TEST(log2.object() == "Test object");
		BOOST_TEST(log2.object_type() == "Test object type");
		BOOST_TEST(log2.get_stats() != nullptr);
	}

	BOOST_AUTO_TEST_CASE(test_move_assignment_operator)
	{
		json_log log1 = create_sample_json_log();
		json_log log2;
		log2 = std::move(log1);

		BOOST_TEST(log2.level() == json_log::log_level::info);
		BOOST_TEST(log2.message() == "Test message");
		BOOST_TEST(log2.source() == "Test source");
		BOOST_TEST(log2.object() == "Test object");
		BOOST_TEST(log2.object_type() == "Test object type");
		BOOST_TEST(log2.get_stats() != nullptr);
	}

	BOOST_AUTO_TEST_CASE(test_string_to_level)
	{
		BOOST_TEST(json_log::string_to_level("info") == json_log::log_level::info);
		BOOST_TEST(json_log::string_to_level("warning") == json_log::log_level::warning);
		BOOST_TEST(json_log::string_to_level("error") == json_log::log_level::error);
		BOOST_TEST(json_log::string_to_level("no_level") == json_log::log_level::no_level);
	}

	BOOST_AUTO_TEST_CASE(test_level_to_string)
	{
		BOOST_TEST(json_log::level_to_string(json_log::log_level::info) == "info");
		BOOST_TEST(json_log::level_to_string(json_log::log_level::warning) == "warning");
		BOOST_TEST(json_log::level_to_string(json_log::log_level::error) == "error");
		BOOST_TEST(json_log::level_to_string(json_log::log_level::no_level) == "no_level");
	}

	BOOST_AUTO_TEST_CASE(test_stream_insertion_operator)
	{
		json_log log = create_sample_json_log();
		std::ostringstream oss;
		oss << log;
		std::string output = oss.str();
		BOOST_TEST(output.find("Test message") != std::string::npos);
	}

BOOST_AUTO_TEST_SUITE_END()
