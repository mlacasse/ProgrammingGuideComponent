#ifndef _PROGRAMMING_GUIDE_PROPS_H
#define _PROGRAMMING_GUIDE_PROPS_H

#include <youireact/props/PropTypes.h>

#include <folly/dynamic.h>

namespace yi
{
namespace react
{

typedef struct
{
    uint32_t channelNumber;
    
    std::string description;
    std::string resourceId; // channel identifier
    std::string callSign;
    std::string format;
    std::string name;

    CYIUrl imageUrl;
} ChannelProps;

bool InitFromValue(ChannelProps &channel, const folly::dynamic &value);

typedef struct
{
    bool isRestartable;

    uint32_t duration;
    
    std::vector<std::string> categories{};
    std::vector<std::string> genres{};

    std::string parentalRating;
    std::string description;
    std::string resourceId; // content identifier
    std::string channelId;
    std::string title;

    CYIDateTime startTime;
    CYIDateTime endTime;

    CYIUrl imageUrl;
} TimeslotProps;

bool InitFromValue(TimeslotProps &timeslot, const folly::dynamic &value);

struct ProgrammingGuideProps : public PropTypes
{
    std::vector<ChannelProps> channels;
    std::vector<TimeslotProps> schedules;
};

} // namespace react
} // namespace yi

#endif // _PROGRAMMING_GUIDE_PROPS_H
