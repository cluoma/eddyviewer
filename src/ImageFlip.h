//
// Created by colin on 2020-12-21.
//

#ifndef EDDYVIEWER_IMAGEFLIP_H
#define EDDYVIEWER_IMAGEFLIP_H

#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WText.h>

class ImageFlip : public Wt::WContainerWidget
{
public:
    ImageFlip();
    void push(std::string filename, std::string timestamp);
    Wt::WImage * getImage();
    std::string getVisibleTimestamp();

private:
    Wt::WImage *visible_;
    Wt::WImage *next_;

    std::string visible_timestamp_;
    std::string next_timestamp_;
};


#endif //EDDYVIEWER_IMAGEFLIP_H
