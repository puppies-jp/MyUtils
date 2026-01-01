
# boostについて

本ページではboostの使い方をまとめる

[とりあえずsample](https://github.com/puppies-jp/MyUtils/tree/gh-pages/Clang/src/boost)

## boost loggerについて

boostライブラリが提供するloggerである。
`front`と`back`のシンクと呼ばれる出口を作成することで、
ファイル出力、標準出力などの切り替えを実装者側で必要としないloggerを作成できる。
また、ファイルへのloggingにおいてのopen/closeをフックすることもできる。
以下にサンプルを載せる

基本的な手順は2つである。

1. sinkを作成し、frontend,backendの設定を行う(open/close/mode/formatなど)
2. coreに作成したsinkを登録する

以上

```cpp

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
    boost::shared_ptr<logging::core> core = logging::core::get();
    core->add_sink(sink);
}
```
