/*
 * Copyright 2022 Google LLC.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skgpu_graphite_VulkanGraphiteTypesPriv_DEFINED
#define skgpu_graphite_VulkanGraphiteTypesPriv_DEFINED

#include "include/core/SkString.h"
#include "include/gpu/graphite/BackendTexture.h"
#include "include/gpu/graphite/vk/VulkanGraphiteTypes.h"
#include "include/private/gpu/vk/SkiaVulkan.h"

class SkStream;
class SkWStream;

namespace skgpu::graphite {

struct VulkanTextureSpec {
    VulkanTextureSpec()
            : fFlags(0)
            , fFormat(VK_FORMAT_UNDEFINED)
            , fImageTiling(VK_IMAGE_TILING_OPTIMAL)
            , fImageUsageFlags(0)
            , fSharingMode(VK_SHARING_MODE_EXCLUSIVE)
            , fAspectMask(VK_IMAGE_ASPECT_COLOR_BIT)
            , fYcbcrConversionInfo({}) {}
    VulkanTextureSpec(const VulkanTextureInfo& info)
            : fFlags(info.fFlags)
            , fFormat(info.fFormat)
            , fImageTiling(info.fImageTiling)
            , fImageUsageFlags(info.fImageUsageFlags)
            , fSharingMode(info.fSharingMode)
            , fAspectMask(info.fAspectMask)
            , fYcbcrConversionInfo(info.fYcbcrConversionInfo) {}

    bool operator==(const VulkanTextureSpec& that) const {
        return fFlags == that.fFlags &&
               fFormat == that.fFormat &&
               fImageTiling == that.fImageTiling &&
               fImageUsageFlags == that.fImageUsageFlags &&
               fSharingMode == that.fSharingMode &&
               fAspectMask == that.fAspectMask &&
               fYcbcrConversionInfo == that.fYcbcrConversionInfo;
    }

    bool isCompatible(const VulkanTextureSpec& that) const {
        // The usages may match or the usage passed in may be a superset of the usage stored within.
        return fFlags == that.fFlags &&
               fFormat == that.fFormat &&
               fImageTiling == that.fImageTiling &&
               fSharingMode == that.fSharingMode &&
               fAspectMask == that.fAspectMask &&
               (fImageUsageFlags & that.fImageUsageFlags) == fImageUsageFlags &&
               fYcbcrConversionInfo == that.fYcbcrConversionInfo;
    }

    SkString toString() const {
        return SkStringPrintf(
                "flags=0x%08X,format=%d,imageTiling=%d,imageUsageFlags=0x%08X,sharingMode=%d,"
                "aspectMask=%u",
                fFlags,
                fFormat,
                fImageTiling,
                fImageUsageFlags,
                fSharingMode,
                fAspectMask);
    }

    bool serialize(SkWStream*) const;
    static bool Deserialize(SkStream*, VulkanTextureSpec* out);

    VkImageCreateFlags         fFlags;
    VkFormat                   fFormat;
    VkImageTiling              fImageTiling;
    VkImageUsageFlags          fImageUsageFlags;
    VkSharingMode              fSharingMode;
    VkImageAspectFlags         fAspectMask;
    VulkanYcbcrConversionInfo  fYcbcrConversionInfo;
};

VulkanTextureInfo VulkanTextureSpecToTextureInfo(const VulkanTextureSpec& vkSpec,
                                                 uint32_t sampleCount,
                                                 Mipmapped mipmapped);

namespace TextureInfos {
VulkanTextureSpec GetVulkanTextureSpec(const TextureInfo&);
VkFormat GetVkFormat(const TextureInfo&);
VkImageUsageFlags GetVkUsageFlags(const TextureInfo&);
VulkanYcbcrConversionInfo GetVulkanYcbcrConversionInfo(const TextureInfo&);
}  // namespace TextureInfos

namespace BackendTextures {
VkImage GetVkImage(const BackendTexture&);
VkImageLayout GetVkImageLayout(const BackendTexture&);
uint32_t GetVkQueueFamilyIndex(const BackendTexture&);
VulkanAlloc GetMemoryAlloc(const BackendTexture&);

void SetMutableState(BackendTexture*, const skgpu::MutableTextureState&);
sk_sp<skgpu::MutableTextureState> GetMutableState(const BackendTexture&);
}  // namespace BackendTextures

}  // namespace skgpu::graphite

#endif  // skgpu_graphite_VulkanGraphiteTypesPriv_DEFINED
