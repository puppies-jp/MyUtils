#include "boost_log.h"

using namespace std;

// typedef sinks::synchronous_sink<sinks::text_file_backend> sink_t;
void fileOpeningHandler(sinks::text_file_backend::stream_type &file)
{
    std::cout << "Opened" << std::endl;
}

void fileClosingHandler(sinks::text_file_backend::stream_type &file)
{
    std::cout << "Closed" << std::endl;
}

boost::shared_ptr<sink_t> make_text_file_sink()
{
    // backend shink作成
    boost::shared_ptr<sinks::text_file_backend> backend =
        boost::make_shared<sinks::text_file_backend>(
            keywords::file_name = "file.log",                              //"file_%H%M%S_%N.log"//
            keywords::target_file_name = "file_%H%M%S.log",                // ファイル名フォーマット設定
            keywords::rotation_size = 5 * 1024,                            // サイズ設定
            keywords::enable_final_rotation = true,                        // ローテーション設定
            keywords::open_mode = std::ios_base::out | std::ios_base::app, // 追記モードとかの設定
            // keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0),
            keywords::time_based_rotation = /*　時間によるローテーション設定　*/
            sinks::file::rotation_at_time_interval(boost::posix_time::hours(1)));
    // backend -> sink作成
    boost::shared_ptr<sink_t> sink(new sink_t(backend));
    // sinkのフォーマッタ設定
    sink->set_formatter(
        expr::format("%1%:[%2%]:%3%") % expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") % logging::trivial::severity % expr::smessage);

    // Set header and footer writing functors
    sink->locked_backend()->set_open_handler(&fileOpeningHandler);
    sink->locked_backend()->set_close_handler(&fileClosingHandler);

    // 出力先回りとかのスキャン設定
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "./",
        keywords::max_files = 3));
    // Upon restart, scan the directory for files matching the file_name pattern
    sink->locked_backend()->scan_for_files(sinks::file::scan_matching, "file_%H%M%S.log.txt");

    return sink;
}

void init1(boost::shared_ptr<sink_t> sink)
{
    // boost logger init
    cout << "📕📕📕📕📕Start logging 2nd" << endl;
    boost::shared_ptr<logging::core> core = logging::core::get();
    core->add_sink(sink);
}
