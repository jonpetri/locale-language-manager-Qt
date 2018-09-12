#include "MainWindow_Demo.h"
#include "ui_MainWindow_Demo.h"
#include "LanguageChoiceDialog1_Demo.h"
#include "LanguageChoiceDialog2_Demo.h"


//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog1 :: Constructor / Destructor
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 * @param [in] qtLanguageManager A QtLanguageManager ready to do translations.
 * @param [in] parent
 */
MainWindow_Demo::MainWindow_Demo(LlmQtLocaleLanguageManager *qtLanguageManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow_Demo)
    , m_languageManager {qtLanguageManager}
{
    ui->setupUi(this);
    this->updateWidgets();
}

MainWindow_Demo::~MainWindow_Demo()
{
    delete ui;
}



//-----------------------------------------------------------------------------------------------------------------------
// LanguageChoiceDialog1 :: Methods
//-----------------------------------------------------------------------------------------------------------------------

/**
 * Widgets updating according different parameters
 * This method must contain all the tr(),
 * cause it will be called when LanguageChange event happens.
 *
 * The widget fields filled in QtDesigner don't need a tr(),
 * they will be translated anyway.
 */
void MainWindow_Demo::updateWidgets()
{
    // Enable access to language menu if m_languageManager is usable.
    // Hence the language setting panel is accessible only if we have qm files !!
    ui->actionLanguage_settings1->setEnabled(m_languageManager->isUsable());
    ui->actionLanguage_settings2->setEnabled(m_languageManager->isUsable());
}

/**
 * Override of QWidget::changeEvent, to implement the dynamic translation of the ui
 * @param [in] event UI's state change
 */
void MainWindow_Demo::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate qtDesigner form
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
 * open of a first panel of language settings
 */
void MainWindow_Demo::on_actionLanguage_settings1_triggered()
{
    LanguageChoiceDialog1_Demo d(m_languageManager, this);
    d.show();
    d.exec();
}
/**
 * open of a second (different) panel of language settings
 */
void MainWindow_Demo::on_actionLanguage_settings2_triggered()
{

    LanguageChoiceDialog2_Demo d(m_languageManager, this);
    d.show();
    d.exec();
}