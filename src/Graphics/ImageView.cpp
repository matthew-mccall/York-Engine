//
// Created by Matthew McCall on 1/7/22.
//

#include "ImageView.hpp"

namespace york::graphics {

ImageView::ImageView(Device& device, vk::Format format, vk::Image image)
    : m_device(device)
    , m_format(format)
    , m_image(image)
{
    // addDependency(m_device);
}

bool ImageView::createImpl()
{
    vk::ImageViewCreateInfo imageViewCreateInfo
    {
        {},
            m_image,
            vk::ImageViewType::e2D,
            m_format,
            {
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
            },
            {
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1 }

    };

    m_handle = m_device->createImageView(imageViewCreateInfo);

    return true;
}

void ImageView::setImage(vk::Image image)
{
    m_image = image;
}

void ImageView::destroyImpl()
{
    m_device->destroy(m_handle);
}


}