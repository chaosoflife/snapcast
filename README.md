# Snapcast

This is a fork of snapcast that removes the PosixStream:do_read method and lets AsioStream:do_read handle it. This fixed the white noise randomness and weird loud Bwoom that can happen between songs when playing through spotify.

Removed 'void PosixStream::do_read()' from 'snapcast/server/streamreader/posix_stream.cpp'
Removed 'void do_read() override;' from 'snapcast/server/streamreader/posix_stream.hpp'
Commented out 'LOG(ERROR, "AsioStream") << "Error reading message: " << ec.message() << ", length: " << length << "\n";' from 'snapcast/server/streamreader/asio_stream.hpp' (This was causing the log file to fill up and take up all space on the device)
