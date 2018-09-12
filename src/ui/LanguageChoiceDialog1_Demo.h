#ifndef LANGUAGECHOICEDIALOG1_DEMO_H
#define LANGUAGECHOICEDIALOG1_DEMO_H
/*----------------------------------------------------------------*/
/*  PROJECT:  localeLanguageManager for Qt - Demo                 */
/*----------------------------------------------------------------*/

/**
 * @class LanguageChoiceDialog1_Demo LanguageChoiceDialog1_Demo.h
 * A first example of implementation of a language settings panel, to demonstrate a usage of QtLanguageManager.
 * That one has a check box "System language" to quickly choice the system language.
 */

#include <QDialog>
#include "localeLanguageManager/LlmQtLocaleLanguageManager.h"

namespace Ui {
class LanguageChoiceDialog1_Demo;
}

class LanguageChoiceDialog1_Demo : public QDialog
{
    Q_OBJECT

public:
    LanguageChoiceDialog1_Demo & operator=(const LanguageChoiceDialog1_Demo&) = delete;
    LanguageChoiceDialog1_Demo(const LanguageChoiceDialog1_Demo&) = delete;
    explicit LanguageChoiceDialog1_Demo(LlmQtLocaleLanguageManager * qtLanguageManager, QWidget *parent = nullptr);
    ~LanguageChoiceDialog1_Demo() override;


private slots:
    void on_pbApply_clicked();
    void on_pbOk_clicked();
    void on_cbSystemLanguage_toggled(bool bValue);
    void on_pbUpdateLanguageList_clicked();


private:
    void updateWidgets();
    virtual void changeEvent(QEvent* event) override;  // Indispensable to straight translation at running time


    Ui::LanguageChoiceDialog1_Demo *ui;
    LlmQtLocaleLanguageManager * m_languageManager; // manage the qt translations


};

#endif // LANGUAGECHOICEDIALOG1_DEMO_H
