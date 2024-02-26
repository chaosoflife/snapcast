# Snapcast

This is a fork of snapcast that removes the PosixStream:do_read method and lets AsioStream:do_read handle it. This fixed the white noise randomness and weird loud Bwoom that can happen between songs when playing through spotify.
