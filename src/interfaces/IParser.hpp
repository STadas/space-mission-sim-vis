#pragma once

#include <QtCore>

#include "common/ParseResult.hpp"

class IParser
{
public:
    virtual ~IParser()
    {
    }

    virtual ParseResult parse(const QString &strCommand) = 0;
};
