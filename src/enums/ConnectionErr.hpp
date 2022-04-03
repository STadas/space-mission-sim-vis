#pragma once

enum class ConnectionErr {
    Ok,
    BadSock,
    BadConn,
    BadRequest,
    BadResponse,
    BadData,
    Unknown
};
