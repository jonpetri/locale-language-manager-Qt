#include "LanguageChoiceDialog2_Demo.h"
#include "ui_LanguageChoiceDialog2_Demo.h"

#include <QComboBox>
#include <QMessageBox>


//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog2 :: Constructor / Destructor
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 * @param [in] qtLanguageManager A QtLanguageManager ready to do translations.
 * @param [in] parent
 */
LanguageChoiceDialog2_Demo::LanguageChoiceDialog2_Demo(LlmQtLocaleLanguageManager *qtLanguageManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LanguageChoiceDialog2_Demo)
    , m_languageManager { qtLanguageManager}
{

    ui->setupUi(this);
    this->updateWidgets();

    if (m_languageManager->isSystemLanguageChosenByUser() == true)
        ui->cbLanguageChoice->setCurrentIndex(0); // System flag is at index 0
    else
        ui->cbLanguageChoice->setCurrentText(m_languageManager->currentLanguageNativeName());
}

LanguageChoiceDialog2_Demo::~LanguageChoiceDialog2_Demo()
{
    delete ui;
}




//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog2 :: Methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Widgets updating according m_languageManager
 * This method must contain all the tr(),
 * cause it will be called when LanguageChange event happens.
 *
 * The widget fields filled in QtDesigner don't need a tr(),
 * they will be translated anyway.
 */
void LanguageChoiceDialog2_Demo::updateWidgets()
{
    //Combo box fill:
    QString sLangBeforeUpdate = ui->cbLanguageChoice->currentText();
    QStringList languagesList = m_languageManager->languageNameNativeList(true);  // True -> <SYSTEM> flag is in the list
    ui->cbLanguageChoice->clear();
    for (int i = 0 ; i < languagesList.count() ; ++i)
    {
        ui->cbLanguageChoice->addItem(languagesList[i]);
    }
    ui->cbLanguageChoice->setCurrentText(sLangBeforeUpdate);
}



/**
 * Override of QWidget::changeEvent, to implement the dynamic translation of ui
 * @param [in] event UI's state change
 */
void LanguageChoiceDialog2_Demo::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form
        ui->retranslateUi(this);
         // retranslate other widgets which weren't added in designer, if needed
        this->updateWidgets();
    }
   // call of base class implementation
   QWidget::changeEvent(event);
}



//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog2 :: Slots
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Apply translation to the selected language.
 */
void LanguageChoiceDialog2_Demo::on_pbApply_clicked()
{
    QString sErrMsg;
    QString sLanguageChosen = ui->cbLanguageChoice->currentText();

    if ( m_languageManager->translateAppToLanguage(sLanguageChosen, &sErrMsg) == false)
    {
        QMessageBox::critical(this, "Error", sErrMsg);
    }
}

/**
 * Apply translation to the selected language and quit.
 */
void LanguageChoiceDialog2_Demo::on_pbOk_clicked()
{
    this->on_pbApply_clicked();
    this->close();
}



/**
 * Ask m_languageManager to reread the available qm files,
 * to re-write the languages list accordingly.
 */
void LanguageChoiceDialog2_Demo::on_pbUpdateLanguageList_clicked()
{
    QString sErrMsg;

    if ( m_languageManager->analyseAvailableLanguages(&sErrMsg) == false)
    {
        QMessageBox::critical(this, "Error", sErrMsg);
    }
    else
    {
        this->updateWidgets();
    }
}
