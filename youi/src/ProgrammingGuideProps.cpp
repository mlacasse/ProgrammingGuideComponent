#include "ProgrammingGuideProps.h"

#include <utility/FollyDynamicUtilities.h>
#include <utility/InitFromValue.h>

#define LOG_TAG "ProgrammingGuideProps"

bool yi::react::InitFromValue(ChannelProps &channel, const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromMandatoryField(channel.channelId, value, "channelId");
    ok = ok && InitFromMandatoryField(channel.name, value, "channelName");
    ok = ok && InitFromMandatoryField(channel.channelNumber, value, "channelNumber");
    ok = ok && InitFromMandatoryField(channel.callSign, value, "channelCallSign");
    ok = ok && InitFromMandatoryField(channel.isFavorite, value, "isFavorite");
    ok = ok && InitFromMandatoryField(channel.schedule, value, "contents");

    if (value["channelLogo"].isObject() && value["channelLogo"]["imageUrl"].isString())
    {
        channel.imageUrl = CYIUrl(value["channelLogo"]["imageUrl"].asString());
    }

    return ok;
}

bool yi::react::InitFromValue(ScheduleProps &timeslot, const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromMandatoryField(timeslot.resourceId, value, "resourceId");
    ok = ok && InitFromMandatoryField(timeslot.title, value, "title");
    ok = ok && InitFromOptionalField(timeslot.episodeTitle, value, "episodeTitle");
    ok = ok && InitFromOptionalField(timeslot.episodeNumber, value, "episodeNumber");
    ok = ok && InitFromOptionalField(timeslot.seasonNumber, value, "seasonNumber");
    ok = ok && InitFromOptionalField(timeslot.description, value, "description");
    ok = ok && InitFromOptionalField(timeslot.parentalRating, value, "parentalRating");

    if (value["consumables"].isObject())
    {
        folly::dynamic consumables = value["consumables"];

        ok = ok && InitFromOptionalField(timeslot.duration, consumables, "duration");
        ok = ok && InitFromOptionalField(timeslot.startTime, consumables, "startTime");
        ok = ok && InitFromOptionalField(timeslot.endTime, consumables, "endTime");
        
        if (consumables["augmentation"].isObject())
        {
            folly::dynamic augmentation = consumables["augmentation"];

            ok = ok && InitFromOptionalField(timeslot.isRestartable, augmentation, "isRestart");
        }
    }

    auto it = value.find("genres"); // Optional
    if (it != value.items().end() && it->second.type() == folly::dynamic::ARRAY)
    {
        for(auto &elem : it->second)
        {
            timeslot.genres.push_back(elem.asString());
        }
    }

    it = value.find("categories");  // Optional
    if (it != value.items().end() && it->second.type() == folly::dynamic::ARRAY)
    {
        for(auto &elem : it->second)
        {
            timeslot.categories.push_back(elem.asString());
        }
    }

    if (value["images"].isArray())  // Optional
    {
        for (auto &elem : value["images"])
        {
            if (elem.isObject() && elem["imageUrl"].isString())
            {
                timeslot.imageUrls.push_back(CYIUrl(elem["imageUrl"].asString()));
            }
        }
    }

    return ok;
}
