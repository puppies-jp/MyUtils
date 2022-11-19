#pragma once

#ifndef Blogger
#define Blogger
#include <stdexcept>
#include <string>
#include <iostream>
#include <thread>
#include <fstream>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lambda/lambda.hpp>

#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

typedef sinks::synchronous_sink<sinks::text_file_backend> sink_t;

void handler1(sinks::text_file_backend::stream_type &file);

void handler2(sinks::text_file_backend::stream_type &file);

boost::shared_ptr<sink_t> make_text_file_sink();

void init1(boost::shared_ptr<sink_t> sink);

#endif