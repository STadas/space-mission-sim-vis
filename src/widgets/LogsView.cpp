#include "LogsView.hpp"

LogsView::LogsView(QWidget *parent)
    : QPlainTextEdit(parent)
{
    this->setReadOnly(true);
}

LogsView::~LogsView()
{
}
