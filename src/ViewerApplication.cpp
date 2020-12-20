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

    auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
    bootstrapTheme->setVersion(Wt::BootstrapVersion::v3);
    bootstrapTheme->setResponsive(true);
    setTheme(bootstrapTheme);
    styleSheet().addRule(".container", "height: 90vh; height: calc(var(--vh, 1vh) * 90);");

    render();

    // Refresh every 2 seconds
    auto timer = root()->addChild(std::make_unique<Wt::WTimer>());
    timer->setInterval(std::chrono::seconds(2));
    timer->timeout().connect(this, &ViewerApplication::render);
    timer->start();
}

void ViewerApplication::getMostRecentImage()
{
    Wt::Dbo::Transaction transaction{session_};
    c_ = session_.find<CameraShot>().orderBy("createddateint DESC").limit(1);

    std::cerr << "imagename: " << c_->filename << std::endl;
}

void ViewerApplication::render()
{
    getMostRecentImage();

    auto layout = std::make_unique<Wt::WContainerWidget>();
    layout->setWidth("100%");
    layout->addStyleClass("container");
    layout->setContentAlignment(Wt::AlignmentFlag::Center);
    layout->addWidget(Wt::cpp14::make_unique<Wt::WText>(c_->createddate.toString()));

    auto container = Wt::cpp14::make_unique<Wt::WContainerWidget>();
    container->addStyleClass("container");
    //container->setHeight("90vh");

    Wt::WImage *image = container->addNew<Wt::WImage>(Wt::WLink(c_->filename));
    image->setAlternateText("Eddy");
    image->setMaximumSize("100%","100%");

    layout->addWidget(std::unique_ptr<Wt::WContainerWidget>(std::move(container)));

    root()->clear();
    root()->addWidget(std::unique_ptr<Wt::WContainerWidget>(std::move(layout)));
}