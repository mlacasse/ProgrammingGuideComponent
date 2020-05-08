#include "ProgrammingGuideProps.h"

bool InitFromValue(std::shared_ptr<EPGChannelModel> &pModelOut, const folly::dynamic &value)
{
    pModelOut = std::make_shared<EPGChannelModel>();
    return pModelOut->Init(value);
}

bool InitFromValue(std::shared_ptr<EPGAssetModel> &pModelOut, const folly::dynamic &value)
{
    pModelOut = std::make_shared<EPGAssetModel>();
    return pModelOut->Init(value);
}
