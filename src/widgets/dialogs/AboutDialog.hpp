#pragma once

#include <QtCore>
#include <QtWidgets>

#include "common/Resources.hpp"

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent, Resources *const resources);
    ~AboutDialog();

private:
    Resources *const resources_;

    void addLicense(QFormLayout *layout, const QString &name,
                    const QString &licensePath, const QString &website = "");
};
