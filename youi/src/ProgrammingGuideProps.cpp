#include "ProgrammingGuideProps.h"

#include <utility/FollyDynamicUtilities.h>

bool yi::react::InitFromValue(ChannelProps &channel, const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromMandatoryField(channel.callSign, value, "callSign");
    ok = ok && InitFromMandatoryField(channel.channelNumber, value, "channelNumber");
    ok = ok && InitFromOptionalField(channel.description, value, "description");
    ok = ok && InitFromOptionalField(channel.format, value, "format");
    ok = ok && InitFromOptionalField(channel.imageUrl, value, "imageUrl");
    ok = ok && InitFromMandatoryField(channel.name, value, "name");
    ok = ok && InitFromMandatoryField(channel.resourceId, value, "resourceId");

    return ok;
}

bool yi::react::InitFromValue(TimeslotProps &timeslot, const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromOptionalField(timeslot.categories, value, "categories");
    ok = ok && InitFromMandatoryField(timeslot.channelId, value, "channelId");
    ok = ok && InitFromOptionalField(timeslot.description, value, "description");
    ok = ok && InitFromMandatoryField(timeslot.duration, value, "duration");
    ok = ok && InitFromMandatoryField(timeslot.endTime, value, "endTime");
    ok = ok && InitFromOptionalField(timeslot.genres, value, "genres");
    ok = ok && InitFromMandatoryField(timeslot.imageUrl, value, "imageUrl");
    ok = ok && InitFromMandatoryField(timeslot.isRestartable, value, "isRestart");
    ok = ok && InitFromOptionalField(timeslot.parentalRating, value, "parentalRating");
    ok = ok && InitFromMandatoryField(timeslot.resourceId, value, "resourceId");
    ok = ok && InitFromMandatoryField(timeslot.startTime, value, "startTime");
    ok = ok && InitFromMandatoryField(timeslot.title, value, "title");

    return ok;
}
