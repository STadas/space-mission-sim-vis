#pragma once

enum class ConnectionErr {
    OK,
    BAD_SOCK,
    BAD_CONN,
    BAD_REQUEST,
    BAD_RESPONSE,
    BAD_DATA,
    UNKNOWN
};
