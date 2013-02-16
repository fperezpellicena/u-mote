#include "FirFilterBuildTime.h"

FirFilterBuildTime::FirFilterBuildTime()
: dateTime(__DATE__ " " __TIME__)
{
}

FirFilterBuildTime::~FirFilterBuildTime()
{
}

const char* FirFilterBuildTime::GetDateTime()
{
    return dateTime;
}

