#include "EPGChannelModel.h"

#include "../react/ProgrammingGuideProps.h"

#include <asset/YiAssetTextureBase.h>
#include <utility/FileUtilities.h>
#include <utility/FollyDynamicUtilities.h>
#include <utility/InitFromValue.h>
#include <utility/YiString.h>

EPGChannelModel::EPGChannelModel()
{}

bool EPGChannelModel::Init(const folly::dynamic &value)
{
    bool ok = true;

    ok = ok && InitFromMandatoryField(m_channelId, value, "channelId");
    ok = ok && InitFromMandatoryField(m_name, value, "channelName");
    ok = ok && InitFromMandatoryField(m_channelNumber, value, "channelNumber");
    ok = ok && InitFromMandatoryField(m_callSign, value, "channelCallSign");
    ok = ok && InitFromMandatoryField(m_isFavorite, value, "isFavorite");
    ok = ok && InitFromMandatoryField(m_assets, value, "contents");

    if (value["channelLogo"].isObject())
    {
        CYIString imageUrl;
        CYIString defaultImageUrl;

        folly::dynamic channelLogo = value["channelLogo"];

        ok = ok && InitFromOptionalField(imageUrl, channelLogo, "imageUrl");
        if (ok)
        {
            m_imageUrl = CYIUrl(GetModifiedResourceFilePath(imageUrl.GetData(), &CYIAssetTextureBase::GetClassTypeInfo()));
        }

        ok = ok && InitFromOptionalField(defaultImageUrl, channelLogo, "defaultImageUrl");
        if (ok)
        {
            m_defaultImageUrl = CYIUrl(GetModifiedResourceFilePath(defaultImageUrl.GetData(), &CYIAssetTextureBase::GetClassTypeInfo()));
        }
    }

    m_channelNumberString = CYIString::FromValue(m_channelNumber);

    return ok;
}
