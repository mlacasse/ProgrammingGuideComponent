#ifndef _PROGRAMMING_GUIDE_PROPS_H
#define _PROGRAMMING_GUIDE_PROPS_H

#include <youireact/props/PropTypes.h>

#include <folly/dynamic.h>

#include "../model/EPGAssetModel.h"
#include "../model/EPGChannelModel.h"

namespace yi
{
namespace react
{

struct ProgrammingGuideProps : public PropTypes
{
    std::vector<std::shared_ptr<EPGChannelModel>> channels;

    CYIDateTime startTime;

    CYIString currentChannelId;
    CYIString currentAssetId;

    double  duration;
};

} // namespace react
} // namespace yi

bool InitFromValue(std::shared_ptr<EPGChannelModel> &pModelOut, const folly::dynamic &value);

bool InitFromValue(std::shared_ptr<EPGAssetModel> &pModelOut, const folly::dynamic &value);

#endif // _PROGRAMMING_GUIDE_PROPS_H
