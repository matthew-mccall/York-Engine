//
// Created by Matthew McCall on 1/7/22.
//

#ifndef YORK_GRAPHICS_IMAGEVIEW_HPP
#define YORK_GRAPHICS_IMAGEVIEW_HPP

#include "Handle.hpp"
#include "Device.hpp"

#include <vulkan/vulkan.hpp>

namespace york::graphics {

class ImageView : public Handle<vk::ImageView> {
public:
    ImageView(Device& device, vk::Format format, vk::Image image);

protected:
    bool createImpl() override;
    void setImage(vk::Image image);
    void destroyImpl() override;

private:
    Device& m_device;
    vk::Format m_format;
    vk::Image m_image;
};

}


#endif // YORK_GRAPHICS_IMAGEVIEW_HPP
