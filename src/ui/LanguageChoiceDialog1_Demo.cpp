#include "LanguageChoiceDialog1_Demo.h"
#include "ui_LanguageChoiceDialog1_Demo.h"

#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>


//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog1 :: Constructor / Destructor
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 * @param [in] qtLanguageManager A QtLanguageManager ready to do translations.
 * @param [in] parent
 */
LanguageChoiceDialog1_Demo::LanguageChoiceDialog1_Demo(LlmQtLocaleLanguageManager *qtLanguageManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LanguageChoiceDialog1_Demo)
    , m_languageManager { qtLanguageManager}
{

    ui->setupUi(this);
    this->updateWidgets();
    ui->cbLanguageChoice->setCurrentText(m_languageManager->currentLanguageNativeName());
}

LanguageChoiceDialog1_Demo::~LanguageChoiceDialog1_Demo()
{
    delete ui;
}




//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog1 :: Methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Widgets updating according m_languageManager
 * This method must contain all the tr(),
 * cause it will be called when LanguageChange event happens.
 *
 * The widget fields filled in QtDesigner don't need a tr(),
 * they will be translated anyway.
 */
void LanguageChoiceDialog1_Demo::updateWidgets()
{
    if (m_languageManager->isSystemLanguageAvailable() == false)
    {
        ui->cbSystemLanguage->setChecked(false);
        ui->cbSystemLanguage->setEnabled(false);
        ui->cbSystemLanguage->setText(tr("System language not available"));
    }
    else
    {
        ui->cbSystemLanguage->setEnabled(true);
        QString sSystemLanguageName(" (" + m_languageManager->systemLanguageNativeName() + ")");
        ui->cbSystemLanguage->setText(tr("System language") + sSystemLanguageName);

        if (m_languageManager->isSystemLanguageChosenByUser() == true)
            ui->cbSystemLanguage->setChecked(true);
        else
            ui->cbSystemLanguage->setChecked(false);
    }

    //Combo box fill:
    QString sLangBeforeUpdate = ui->cbLanguageChoice->currentText();
    QStringList languagesList = m_languageManager->languageNameNativeList(false); // false -> <SYSTEM> flag is not in the list
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
void LanguageChoiceDialog1_Demo::changeEvent(QEvent* event)
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
// LanguageChoiceDialog1 :: Slots
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Apply translation to the selected language.
 */
void LanguageChoiceDialog1_Demo::on_pbApply_clicked()
{
    QString sErrMsg;

    if (ui->cbSystemLanguage->isChecked() == true)
    {
        if (m_languageManager->translateAppToSystemLanguage(&sErrMsg) == false)
        {
            QMessageBox::critical(this, "Error", sErrMsg);
        }
    }
    else
    {
        QString sLanguageChosen = ui->cbLanguageChoice->currentText();

        if ( m_languageManager->translateAppToLanguage(sLanguageChosen, &sErrMsg) == false)
        {
            QMessageBox::critical(this, "Error", sErrMsg);
        }
    }
}

/**
 * Apply translation to the selected language and quit.
 */
void LanguageChoiceDialog1_Demo::on_pbOk_clicked()
{
    this->on_pbApply_clicked();
    this->close();
}

/**
 * Lock of the combobox when system check box is selected
 * @param [in] bValue check box's value
 */
void LanguageChoiceDialog1_Demo::on_cbSystemLanguage_toggled(bool bValue)
{
    if (bValue == true)
    {
        ui->cbLanguageChoice->setCurrentText(m_languageManager->systemLanguageNativeName());
    }
    ui->cbLanguageChoice->setEnabled(not(bValue));
}

/**
 * Ask m_languageManager to reread the available qm files,
 * to re-write the languages list accordingly.
 */
void LanguageChoiceDialog1_Demo::on_pbUpdateLanguageList_clicked()
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
