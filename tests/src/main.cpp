#include <gtest/gtest.h>
#include <QApplication>
#include <QtConcurrent>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    QApplication app(argc, argv);

    QtConcurrent::run([&app] {
        auto res = RUN_ALL_TESTS();

        app.exit(res);
    });

    return app.exec();
}
