#include "EPGAssetModel.h"

#include <asset/YiAssetTextureBase.h>
#include <utility/FileUtilities.h>
#include <utility/FollyDynamicUtilities.h>
#include <utility/InitFromValue.h>
#include <utility/YiString.h>

// Covers all platforms
#if !(defined YI_DEBUG)
#if defined (DEBUG) || defined (_DEBUG) || !(defined (NDEBUG) || defined (_NDEBUG))
#define YI_DEBUG 1
#endif
#endif

EPGAssetModel::EPGAssetModel()
{}

bool EPGAssetModel::Init(const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromMandatoryField(m_resourceId, value, "resourceId");
    ok = ok && InitFromMandatoryField(m_title, value, "title");
    ok = ok && InitFromOptionalField(m_episodeTitle, value, "episodeTitle");
    ok = ok && InitFromOptionalField(m_episodeNumber, value, "episodeNumber");
    ok = ok && InitFromOptionalField(m_seasonNumber, value, "seasonNumber");
    ok = ok && InitFromOptionalField(m_description, value, "description");
    ok = ok && InitFromOptionalField(m_parentalRating, value, "parentalRating");
    ok = ok && InitFromOptionalField(m_categories, value, "categories");
    
    if (value["consumables"].isArray())
    {
        folly::dynamic consumable = value["consumables"][0];

        ok = ok && InitFromMandatoryField(m_startTime, consumable, "startTime");
        ok = ok && InitFromMandatoryField(m_endTime, consumable, "endTime");
        ok = ok && InitFromMandatoryField(m_programChannelId, consumable, "programChannelId");
        ok = ok && InitFromOptionalField(m_duration, consumable, "duration");

        if (consumable["augmentation"].isObject())
        {
            folly::dynamic augmentation = consumable["augmentation"];

            if (augmentation["constraints"].isObject())
            {
                folly::dynamic constraints = augmentation["constraints"];

                ok = ok && InitFromOptionalField(m_isFastForwardDisabled, constraints, "isFastForwardDisabled");
                ok = ok && InitFromOptionalField(m_isRestartable, constraints, "isRestart");
                ok = ok && InitFromOptionalField(m_isExtendable, constraints, "isExtendable");
                ok = ok && InitFromOptionalField(m_isRecordable, constraints, "isRecordable");
                ok = ok && InitFromOptionalField(m_isLookback, constraints, "isLookback");
                ok = ok && InitFromOptionalField(m_isPlayable, constraints, "isPlayable");
                ok = ok && InitFromOptionalField(m_isDAI, constraints, "isDAI");
            }
        }
    }

    if (value["images"].isArray())  // Optional
    {
        for (auto &elem : value["images"])
        {
            if (elem["imageUrl"].isString())
            {
                m_imageUrls.push_back(CYIUrl(GetModifiedResourceFilePath(elem["imageUrl"].asString(), &CYIAssetTextureBase::GetClassTypeInfo())));
            }

            if (elem["defaultImageUrl"].isString())
            {
                m_imageUrls.push_back(CYIUrl(GetModifiedResourceFilePath(elem["defaultImageUrl"].asString(), &CYIAssetTextureBase::GetClassTypeInfo())));
            }
        }
    }

    return ok;
}

bool EPGAssetModel::IsOnNow() const
{
    CYIDateTime now(CYIDateTime::GetCurrentDateTime());

#if YI_DEBUG
    now = m_startTime;
    now.SetMinute(CYIDateTime::GetCurrentDateTime().GetMinute());
#endif

    return m_startTime >= now && now <= m_endTime;
}
