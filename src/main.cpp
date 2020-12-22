/*
 * Copyright (C) 2008 Emweb bv, Herent, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <sys/stat.h>
#include <experimental/filesystem>

#include <Wt/WServer.h>
#include <Wt/WResource.h>
#include <Wt/Http/Response.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/SqlConnection.h>
#include <Wt/Dbo/SqlConnectionPool.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/FixedSqlConnectionPool.h>

#include "CameraShot.h"
#include "ViewerApplication.h"

static std::unique_ptr<Wt::Dbo::SqlConnectionPool> createConnectionPool(const std::string& sqliteDb)
{
    auto connection = std::make_unique<Wt::Dbo::backend::Sqlite3>(sqliteDb);
    connection->setProperty("show-queries", "true");
    connection->setDateTimeStorage(Wt::Dbo::SqlDateTimeType::DateTime, Wt::Dbo::backend::DateTimeStorage::PseudoISO8601AsText);
    return std::make_unique<Wt::Dbo::FixedSqlConnectionPool>(std::move(connection), 20);
}

class RestGetHello : public Wt::WResource
{
public:
    RestGetHello(Wt::Dbo::SqlConnectionPool& dbpool)
        : connectionPool_(dbpool) {};
    virtual ~RestGetHello()
    {
    }

private:
    Wt::Dbo::SqlConnectionPool& connectionPool_;

protected:
    virtual void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
    {
        // Setup database connection and map Class to table
        Wt::Dbo::Session session;
        session.setConnectionPool(connectionPool_);
        session.mapClass<CameraShot>("camerashot");

        // First-time run: create tables and directories
        try {
            //session.dropTables(); /* uncomment during testing if desired */
            session.createTables();
        }
        catch (Wt::Dbo::Exception &e) {
            std::cerr << e.what();
        }


        if (mkdir("./eddypics", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1)
        {
            if (errno == EEXIST)
                std::cerr << "'eddypics' directory already exists\n";
            else
            {
                response.setStatus(500);
                return;
            }
        }

        // Get current time
        auto serverdate = Wt::WDateTime::currentDateTime();
        int64_t serverdateint = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        // Find picture and put it where we want
        const Wt::Http::UploadedFile *file = request.getUploadedFile("picture");
        if (file == nullptr)
        {
            response.setStatus(400);
            std::cerr << "No 'picture' included in POST request\n";
        }
        std::string new_filename = "eddypics/" + std::to_string(serverdateint) + ".jpg";
        std::experimental::filesystem::copy(
                file->spoolFileName(),
                new_filename,
                std::experimental::filesystem::copy_options::overwrite_existing);

        // Get createddate timestamp
        auto createddate_param = request.getParameter("createddate");
        if (createddate_param == nullptr)
        {
            response.setStatus(400);
            std::cerr << "No 'createddate' included in POST request\n";
        }
        int64_t createddateint = strtoll((*createddate_param).c_str(), nullptr, 10);
        Wt::WDateTime createddate;
        createddate.setTime_t(createddateint);

        dbo::Transaction transaction{session};
        auto c = std::make_unique<CameraShot>();
        c->filename = new_filename;
        c->serverdate = serverdate;
        c->serverdateint = serverdateint;
        c->createddate = createddate;
        c->createddateint = createddateint;
        dbo::ptr<CameraShot> userPtr = session.add(std::move(c));
    }
};

int main(int argc, char **argv)
{
    try
    {
        Wt::WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

        std::unique_ptr<Wt::Dbo::SqlConnectionPool> appdbpool
                = createConnectionPool(server.appRoot() + "eddyviewer.db");

        RestGetHello getHello(*appdbpool);
        server.addResource(&getHello, "/api/picture/new");

        server.addEntryPoint(Wt::EntryPointType::Application, [&appdbpool](const Wt::WEnvironment &env) {
            return Wt::cpp14::make_unique<ViewerApplication>(env, *appdbpool);
        });

        server.run();
    }
    catch (Wt::WServer::Exception& e)
    {
        std::cerr << "Wt Exception: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
