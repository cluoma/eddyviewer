//
// Created by colin on 2020-12-21.
//

#include "ImageFlip.h"

using namespace Wt;

ImageFlip::ImageFlip()
{
    visible_ = nullptr;
    next_ = nullptr;
    addStyleClass("container");
}

Wt::WImage *ImageFlip::getImage()
{
    return visible_;
}
std::string ImageFlip::getVisibleTimestamp()
{
    return visible_timestamp_;
}

void ImageFlip::push(std::string filename, std::string timestamp)
{
    Wt::WImage *nextImage = addWidget(std::make_unique<Wt::WImage>(filename));
    nextImage->setAlternateText("Eddy");
    nextImage->setMaximumSize("100%","100%");
    nextImage->hide();

    if (visible_ == nullptr && next_ == nullptr) {  // first push
        visible_ = nextImage;
        visible_->show();

        visible_timestamp_ = timestamp;
    } else if (visible_ != nullptr && next_ == nullptr) {  // second push
        next_ = nextImage;
        next_timestamp_ = timestamp;
    } else {  // third and on
        next_->show();
        removeWidget(visible_);
        visible_ = next_;
        next_ = nextImage;

        visible_timestamp_ = next_timestamp_;
        next_timestamp_ = timestamp;
    }
}
