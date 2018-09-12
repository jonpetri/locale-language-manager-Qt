#ifndef MAINWINDOW_DEMO_H
#define MAINWINDOW_DEMO_H
/*----------------------------------------------------------------*/
/*  PROJECT:  localeLanguageManager for Qt - Demo                 */
/*----------------------------------------------------------------*/

/**
 * @class MainWindow_Demo MainWindow_Demo.h
 * A minimalist QMainWindow to demonstrate a usage of LlmQtLocaleLanguageManager
 */

#include <QMainWindow>
#include "localeLanguageManager/LlmQtLocaleLanguageManager.h"

namespace Ui {
class MainWindow_Demo;
}

class MainWindow_Demo : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow_Demo & operator=(const MainWindow_Demo&) = delete;
    MainWindow_Demo(const MainWindow_Demo&) = delete;
    explicit MainWindow_Demo(LlmQtLocaleLanguageManager * qtLanguageManager, QWidget *parent = nullptr) ;
    ~MainWindow_Demo() override;

private slots:
    void on_actionLanguage_settings1_triggered();
    void on_actionLanguage_settings2_triggered();

private:
    void updateWidgets();
    virtual void changeEvent(QEvent* event) override; // Indispensable to straight translation at running time

    Ui::MainWindow_Demo *ui;
    LlmQtLocaleLanguageManager * m_languageManager; // manage the qt translations
};

#endif // MAINWINDOW_DEMO_H
