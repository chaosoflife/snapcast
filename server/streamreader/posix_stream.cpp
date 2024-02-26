/***
    This file is part of snapcast
    Copyright (C) 2014-2021  Johannes Pohl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

// prototype/interface header file
#include "posix_stream.hpp"

// local headers
#include "common/aixlog.hpp"
#include "common/snap_exception.hpp"
#include "common/str_compat.hpp"

// standard headers
#include <cerrno>
#include <memory>


using namespace std;
using namespace std::chrono_literals;

namespace streamreader
{

static constexpr auto LOG_TAG = "PosixStream";
static constexpr auto kResyncTolerance = 50ms;

PosixStream::PosixStream(PcmStream::Listener* pcmListener, boost::asio::io_context& ioc, const ServerSettings& server_settings, const StreamUri& uri)
    : AsioStream<stream_descriptor>(pcmListener, ioc, server_settings, uri)
{
    if (uri_.query.find("dryout_ms") != uri_.query.end())
        dryout_ms_ = cpt::stoul(uri_.query["dryout_ms"]);
    else
        dryout_ms_ = 2000;
}


void PosixStream::connect()
{
    if (!active_)
        return;

    idle_bytes_ = 0;
    max_idle_bytes_ = sampleFormat_.rate() * sampleFormat_.frameSize() * dryout_ms_ / 1000;

    try
    {
        do_connect();
    }
    catch (const std::exception& e)
    {
        LOG(ERROR, LOG_TAG) << "Connect exception: " << e.what() << "\n";
        wait(read_timer_, 100ms, [this] { connect(); });
    }
}


void PosixStream::do_disconnect()
{
    if (stream_ && stream_->is_open())
        stream_->close();
}

} // namespace streamreader
