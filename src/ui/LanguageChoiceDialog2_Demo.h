#ifndef LANGUAGECHOICEDIALOG2_DEMO_H
#define LANGUAGECHOICEDIALOG2_DEMO_H
/*----------------------------------------------------------------*/
/*  PROJECT:  localeLanguageManager for Qt - Demo                 */
/*----------------------------------------------------------------*/

/**
 * @class LanguageChoiceDialog2_Demo LanguageChoiceDialog2_Demo.h
 * A second example of implementation of a language settings panel, to demonstrate a usage of LlmQtLocaleLanguageManager.
 * That one has no check box "System language" but there is a \<SYSTEM\> flag in the combo box which can be chosen.
 */

#include <QDialog>
#include "localeLanguageManager/LlmQtLocaleLanguageManager.h"

namespace Ui {
class LanguageChoiceDialog2_Demo;
}

class LanguageChoiceDialog2_Demo : public QDialog
{
    Q_OBJECT

public:
    LanguageChoiceDialog2_Demo & operator=(const LanguageChoiceDialog2_Demo&) = delete;
    LanguageChoiceDialog2_Demo(const LanguageChoiceDialog2_Demo&) = delete;
    explicit LanguageChoiceDialog2_Demo(LlmQtLocaleLanguageManager * qtLanguageManager, QWidget *parent = nullptr);
    ~LanguageChoiceDialog2_Demo() override;


private slots:
    void on_pbApply_clicked();
    void on_pbOk_clicked();
    void on_pbUpdateLanguageList_clicked();


private:
    void updateWidgets();
    virtual void changeEvent(QEvent* event) override;  // Indispensable to straight translation at running time


    Ui::LanguageChoiceDialog2_Demo *ui;
    LlmQtLocaleLanguageManager * m_languageManager; // manage the qt translations
};

#endif // LANGUAGECHOICEDIALOG2_DEMO_H
