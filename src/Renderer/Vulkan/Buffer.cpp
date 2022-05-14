//
// Created by Matthew McCall on 5/13/22.
//

#include "boost/uuid/uuid_hash.hpp"
#include "york/Containers.hpp"

#include "Buffer.hpp"

struct AllocatorMapKey {
    york::vulkan::Instance& instance;
    york::vulkan::Device& device;
};

struct AllocatorMapKeyHash {
    std::size_t operator()(const AllocatorMapKey& key) const
    {
        std::size_t hash { 0 };

        boost::hash_combine(hash, key.instance.getID());
        boost::hash_combine(hash, key.device.getID());

        return hash;
}
};

struct AllocatorMapKeyEqual {
    bool operator()(const AllocatorMapKey& left, const AllocatorMapKey& right) const
    {
        return (left.instance == right.instance) && (left.device == right.device);
    }
};

namespace {

    york::UnorderedMap<AllocatorMapKey, unsigned , AllocatorMapKeyHash, AllocatorMapKeyEqual> s_referenceCount;
    york::UnorderedMap<AllocatorMapKey, VmaAllocator, AllocatorMapKeyHash, AllocatorMapKeyEqual> s_allocators;

}

namespace york::vulkan
{

Buffer::Buffer(Instance& instance, Device& device, std::size_t size)
    : m_instance(instance)
    , m_device(device)
    , m_size(size)
{
    addDependency(m_instance);
    addDependency(m_device);

    if (s_allocators.find({m_instance, m_device}) == s_allocators.end()) {

        assert(!(s_referenceCount[{m_instance, m_device}]));

        VmaAllocatorCreateInfo createInfo {};

        createInfo.vulkanApiVersion = VK_API_VERSION_1_1;
        createInfo.physicalDevice = *m_device.getPhysicalDevice();
        createInfo.device = *m_device;
        createInfo.instance = *m_instance;

        vmaCreateAllocator(&createInfo, &s_allocators[{m_instance, m_device}]);
    }

    m_allocator = s_allocators[{m_instance, m_device}];
    s_referenceCount[{m_instance, m_device}]++;
}

bool Buffer::createImpl()
{
    vk::BufferCreateInfo createInfo {{}, m_size, vk::BufferUsageFlagBits::eVertexBuffer, vk::SharingMode::eExclusive};

    VmaAllocationCreateInfo allocationCreateInfo {
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        VMA_MEMORY_USAGE_AUTO,
        static_cast<VkMemoryPropertyFlags>(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
    };

    vmaCreateBuffer(
        m_allocator,
        reinterpret_cast<const VkBufferCreateInfo*>(&createInfo),
        &allocationCreateInfo,
        reinterpret_cast<VkBuffer*>(&m_handle),
        &m_allocation,
        &m_allocationInfo);

    return true;
}

void Buffer::destroyImpl()
{
    vmaDestroyBuffer(m_allocator, m_handle, m_allocation);
    s_referenceCount[{m_instance, m_device}]--;

    if (!s_referenceCount[{m_instance, m_device}]) {
        vmaDestroyAllocator(m_allocator);
    }
}

}

