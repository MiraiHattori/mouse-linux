#include "mouse_reader.hpp"

int MouseReader::m_mouse_fd_in;
uint8_t MouseReader::m_buf[BUFSIZE];
