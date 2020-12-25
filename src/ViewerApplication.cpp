//
// Created by colin on 2020-12-19.
//

#include "ViewerApplication.h"

#include <memory>

ViewerApplication::ViewerApplication(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool& dbpool)
        : WApplication(env)
{
    // Setup db Session
    session_.setConnectionPool(dbpool);
    session_.mapClass<CameraShot>("camerashot");

    setTitle("Eddy Viewer");

    // Setup theming and styling
    auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
    bootstrapTheme->setVersion(Wt::BootstrapVersion::v3);
    bootstrapTheme->setResponsive(true);
    setTheme(bootstrapTheme);
    styleSheet().addRule(".container", "height: 90vh; height: calc(var(--vh, 1vh) * 90);");
    styleSheet().addRule("body", "background-color: black;");

    // Setup page layout
    auto layout = std::make_unique<Wt::WContainerWidget>();
    layout->setWidth("100%");
    layout->addStyleClass("container");
    layout->setContentAlignment(Wt::AlignmentFlag::Center);

    // Add timestamp label to layout
    dateLabel_ = layout->addWidget(std::make_unique<Wt::WText>());
    dateLabel_->decorationStyle().setForegroundColor(Wt::WColor("white"));

    // Add the image to the layout
    image_ = layout->addWidget(std::make_unique<ImageFlip>());

    // add layout to root
    root()->addWidget(std::unique_ptr<Wt::WContainerWidget>(std::move(layout)));

    update();

    // Refresh every 2 seconds
    auto timer = root()->addChild(std::make_unique<Wt::WTimer>());
    timer->setInterval(std::chrono::seconds(2));
    timer->timeout().connect(this, &ViewerApplication::update);
    timer->start();
}

void ViewerApplication::getMostRecentImage()
{
    Wt::Dbo::Transaction transaction{session_};
    c_ = session_.find<CameraShot>().orderBy("createddateint DESC").limit(1);
}

void ViewerApplication::update()
{
    getMostRecentImage();
//    dateLabel_->setText(c_->createddate.toString());
    image_->push(c_->filename, c_->createddate.toLocalTime().toString().narrow());
    dateLabel_->setText(image_->getVisibleTimestamp());
}