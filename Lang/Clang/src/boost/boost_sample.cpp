#include "boost_log.h"

using namespace std;

void check_boostlog()
{
    // boost logger 実行
    cout << "Start Check boost_log" << endl;
    init1(make_text_file_sink());
    src::severity_logger<severity_level> lg;
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    cout << "End Check boost_log" << endl;
}


int main()
{
    check_boostlog();
    return 0;
}

