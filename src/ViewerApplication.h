//
// Created by colin on 2020-12-19.
//

#ifndef EDDYVIEWER_VIEWERAPPLICATION_H
#define EDDYVIEWER_VIEWERAPPLICATION_H


#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WTimer.h>

#include "CameraShot.h"
#include "ImageFlip.h"

class ViewerApplication : public Wt::WApplication
{
public:
    ViewerApplication(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool& dbpool);

private:
    void update();
    void getMostRecentImage();

    Wt::Dbo::Session session_;
    Wt::Dbo::ptr<CameraShot> c_;

    Wt::WText * dateLabel_;
    ImageFlip * image_;
};


#endif //EDDYVIEWER_VIEWERAPPLICATION_H
