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
    bool isRestartable;

    int64_t duration;

    int64_t episodeNumber;
    int64_t seasonNumber;
    
    std::vector<std::string> categories{};
    std::vector<std::string> genres{};

    std::string parentalRating;
    std::string episodeTitle;
    std::string description;
    std::string resourceId; // content identifier
    std::string title;

    CYIDateTime startTime;
    CYIDateTime endTime;

    std::vector<CYIUrl> imageUrls;
} ScheduleProps;

bool InitFromValue(ScheduleProps &schedule, const folly::dynamic &value);

typedef struct
{
    uint32_t channelNumber;
    
    std::string description;
    std::string channelId;
    std::string callSign;
    std::string name;

    CYIUrl imageUrl;
    
    bool isFavorite;
    
    std::vector<ScheduleProps> schedule;
} ChannelProps;

bool InitFromValue(ChannelProps &channel, const folly::dynamic &value);

struct ProgrammingGuideProps : public PropTypes
{
    std::vector<ChannelProps> channels;
};

} // namespace react
} // namespace yi

#endif // _PROGRAMMING_GUIDE_PROPS_H
