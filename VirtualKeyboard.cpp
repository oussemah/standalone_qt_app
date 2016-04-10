/*---------------------------------------------------------------------------------------------------------------------------------

Copyright (c) 2014 Arnaud Vazard
Copyright (c) 2016 Oussema Harbi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-----------------------------------------------------------------------------------------------------------------------------------*/

#include "VirtualKeyboard.h"

#include <QTextStream>

VirtualKeyboard::VirtualKeyboard(QWidget *w_parent) :
    QFrame(w_parent),
    ui(new Ui::VirtualKeyboard),
    mi_inputType(VIRTUALKEYBOARD_INPUT_UNKNOWINPUTTYPE)
{
}


VirtualKeyboard::~VirtualKeyboard()
{
    if (this->ui != NULL) delete this->ui;
    /*if (this->m_animation != NULL) delete this->m_animation;
    if (this->mw_lineEdit != NULL) delete this->mw_lineEdit;
    if (this->mw_textEdit != NULL) delete this->mw_textEdit;
    if (this->mw_plainTextEdit != NULL) delete this->mw_plainTextEdit;
    */
}


int VirtualKeyboard::initialisation(QWidget* w_inputWidget, QString s_language, bool b_displayBorder)
{
    QTextStream out(stdout);
    if (w_inputWidget != NULL)
       {

           // --- Check type of the input field to bind to the keyboard
          if (QString::compare(QString(w_inputWidget->metaObject()->className()),QString("QLineEdit")) == 0)
           {
               this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
           }
           else if (QString::compare(QString(w_inputWidget->metaObject()->className()),QString("QTextEdit")) == 0)
           {
               this->mi_inputType = VIRTUALKEYBOARD_INPUT_TEXTEDIT;
           }
           else if (QString::compare(QString(w_inputWidget->metaObject()->className()),QString("QPlainTextEdit")) == 0)
           {
               this->mi_inputType = VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT;
           }
           // If the control is a combobox and the combobox can be edited
           else if (QString::compare(QString(w_inputWidget->metaObject()->className()),QString("QComboBox")) == 0)
           {
               // Writing in a combobox is in fact writing in a lineEdit, so we use the lineEdit
               this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
           }
           else {
               out << "Object " << w_inputWidget->metaObject()->className() << "not rcgonized" << endl;
               return VIRTUALKEYBOARD_INIT_FAILED;
           }

       }
    // --- Keymaps Initialisation
    if (!this->initialisationKeymaps(s_language)) return VIRTUALKEYBOARD_UNKNOWLANGUAGE;

    // --- Setup widget's UI
    this->ui->setupUi(this);
    setStyleSheet("VirtualKeyboard {background: url(:/images/background.png) }");

    if (w_inputWidget != NULL)
       {

           // --- Check type of the input field to bind to the keyboard
          if (QString::compare(QString(w_inputWidget->metaObject()->className()),QString("QLineEdit")) == 0)
           {
               this->ui->lineEdit->setText(((QLineEdit*)w_inputWidget)->text());
               this->ui->textEdit->setHidden(true);
               this->ui->plainTextEdit->setHidden(true);
               this->ui->lineEdit->setFocus();
               connect (this->ui->lineEdit, SIGNAL(textChanged(QString)), (QLineEdit*)w_inputWidget, SLOT(setText(QString)));
           }
           else if (w_inputWidget->metaObject()->className() == QString("QTextEdit").toStdString().c_str())
           {
               this->ui->textEdit= new QTextEdit(((QTextEdit*)w_inputWidget));
               this->ui->lineEdit->setHidden(true);
               this->ui->plainTextEdit->setHidden(true);
               this->ui->textEdit->setFocus();
               connect (this->ui->textEdit, SIGNAL(textChanged(QString)), (QTextEdit*)w_inputWidget, SLOT(setText(QString)));
           }
           else if (w_inputWidget->metaObject()->className() == QString("QPlainTextEdit").toStdString().c_str())
           {
               this->ui->plainTextEdit = new QPlainTextEdit(((QPlainTextEdit*)w_inputWidget));
               this->ui->textEdit->setHidden(true);
               this->ui->lineEdit->setHidden(true);
               this->ui->plainTextEdit->setFocus();
               connect (this->ui->plainTextEdit, SIGNAL(textChanged(QString)), (QPlainTextEdit*)w_inputWidget, SLOT(setText(QString)));
           } /*
           // If the control is a combobox and the combobox can be edited
           else if (w_inputWidget->metaObject()->className() == QString("QComboBox").toStdString().c_str())
           {
               // Writing in a combobox is in fact writing in a lineEdit, so we use the lineEdit
               this->ui->lineEdit = this->mw_comboBox->lineEdit();
               this->ui->textEdit->setHidden(true);
               this->ui->plainTextEdit->setHidden(true);
               this->ui->lineEdit->setFocus();
               connect (this->ui->lineEdit, SIGNAL(textChanged(QString)), (QLineEdit*)w_inputWidget, SLOT(setText(QString)));
           }*/

       }

    // Display border around keyboard ?
    this->setFrameShape(QFrame::NoFrame);


    // --- Set CapsLock, numbers and punctuation off by default
    this->mb_isCapsOn = false;
    this->mb_isNumberOn = false;
    this->mb_isPunctuationOn = false;


    // Extraction of every QPushButton matching the regex "pushButton_principalKey_\\d\\d" into a list
    this->mlistw_principalKeys = this->findChildren<QPushButton *>(QRegExp("pushButton_principalKey_\\d\\d"));

    // --- Signals Mapping for non specific keys
    connect(&this->mo_mapperPrimaryKeys,    SIGNAL(mapped(int)),
            this,                           SLOT(keyPressed(int)));

    for (int i_i = 0; i_i < this->mlistw_principalKeys.size(); ++i_i)
    {
        // Connection between the button and the signal mapper
        connect(this->mlistw_principalKeys.at(i_i), SIGNAL(clicked()),
                &this->mo_mapperPrimaryKeys,        SLOT(map()));
        // Map the button with the index of the list to be able to link a key press to a specific key
        this->mo_mapperPrimaryKeys.setMapping(this->mlistw_principalKeys.at(i_i), i_i);
    }

    // --- Connection to change the input widget dynamically
    this->connectFocusChanged();

    // --- Set the initial keymap
    this->setKeymap(this->mlists_lowerKeymap);

    this->show();
    this->setSizeIncrement(5,5);
    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDirection(QPropertyAnimation::Forward);
    m_animation->setDuration(200);
    m_animation->setStartValue(QRect(0,-430,800,400));
    m_animation->setEndValue(QRect(0,0,800,400));
    m_animation->start();
    return VIRTUALKEYBOARD_SUCCESS;
}

bool VirtualKeyboard::initialisationKeymaps(QString s_language)
{
    this->mlists_numbersKeymap << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0"
                               << "!" << "@" << "#" << "$" << "%" << "&&" << "*" << "(" << ")" << ""
                               << "," << "-" << "_" << "[" << "]" << "?" << ".";

    this->mlists_punctuationKeymap << "!" << "@" << "#" << "$" << "%" << "&&" << "*" << "(" << ")" << ""
                                   << ";" << "-" << "_" << "[" << "]" << "?" << "."  << "/" << "\\";

    // If the language is not EN or FR then return false
    if (s_language == "EN")
    {
        this->mlists_lowerKeymap << "q" << "w" << "e" << "r" << "t" << "y" << "u" << "i" << "o" << "p"
                                 << "a" << "s" << "d" << "f" << "g" << "h" << "j" << "k" << "l" << ""
                                 << "z" << "x" << "c" << "v" << "b" << "n" << "m";

        this->mlists_upperKeymap << "Q" << "W" << "E" << "R" << "T" << "Y" << "U" << "I" << "O" << "P"
                                 << "A" << "S" << "D" << "F" << "G" << "H" << "J" << "K" << "L" << ""
                                 << "Z" << "X" << "C" << "V" << "B" << "N" << "M";
    }
    else if (s_language == "FR")
    {
        this->mlists_lowerKeymap << "a" << "z" << "e" << "r" << "t" << "y" << "u" << "i" << "o" << "p"
                                 << "q" << "s" << "d" << "f" << "g" << "h" << "j" << "k" << "l" << "m"
                                 << "w" << "x" << "c" << "v" << "b" << "n" ;

        this->mlists_upperKeymap << "A" << "Z" << "E" << "R" << "T" << "Y" << "U" << "I" << "O" << "P"
                                 << "Q" << "S" << "D" << "F" << "G" << "H" << "J" << "K" << "L" << "M"
                                 << "W" << "X" << "C" << "V" << "B" << "N" ;
    }
    else
        return false;

    return true;
}


void VirtualKeyboard::setKeymap(QList<QString> &lists_keys)
{
    for (int i_i = 0; i_i < this->mlistw_principalKeys.size(); ++i_i)
    {
        // if the index is superior to the size of "lists_keys" OR if the string at i_i in "lists_keys" is empty, we hide the button
        if (i_i >= lists_keys.size() || lists_keys.at(i_i).isEmpty())
        {
            this->mlistw_principalKeys.at(i_i)->hide();
        }
        else // We set the text of the key to the value of lists_keys[i_i]
        {
            this->mlistw_principalKeys.at(i_i)->setText(lists_keys.at(i_i));
            if (this->mlistw_principalKeys.at(i_i)->isHidden()) this->mlistw_principalKeys.at(i_i)->show();
        }
    }
}


void VirtualKeyboard::toggleCapsLock()
{
    // We change the state of the caps lock and reset to false the others states and buttons
    this->mb_isCapsOn = !this->mb_isCapsOn;
    this->mb_isNumberOn = false;
    this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF);
    this->mb_isPunctuationOn = false;
    this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF);

    if (this->mb_isCapsOn)
    {
        this->setKeymap(this->mlists_upperKeymap);
        this->ui->pushButton_principalKey_caps->setStyleSheet("QPushButton { background-color: cyan; border-radius: 3px; }");
    }
    else
    {
        this->setKeymap(this->mlists_lowerKeymap);
        this->ui->pushButton_principalKey_caps->setStyleSheet("");
    }
}


void VirtualKeyboard::toggleNumbers()
{
    // We change the state of the "numbers" boolean and reset to false the others states and buttons
    this->mb_isNumberOn = !this->mb_isNumberOn;
    this->mb_isCapsOn = false;
    this->ui->pushButton_principalKey_caps->setStyleSheet("");
    this->mb_isPunctuationOn = false;
    this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF);

    if (this->mb_isNumberOn)
    {
        this->setKeymap(this->mlists_numbersKeymap);
        this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_ON);
    }
    else
    {
        this->setKeymap(this->mlists_lowerKeymap);
        this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF);
    }
    this->ui->pushButton_principalKey_caps->setEnabled(!this->mb_isNumberOn);
}


void VirtualKeyboard::togglePunctuation()
{
    // We change the state of the "Punctuation" boolean and reset to false the others states and buttons
    this->mb_isPunctuationOn = !this->mb_isPunctuationOn;
    this->mb_isCapsOn = false;
    this->ui->pushButton_principalKey_caps->setStyleSheet("");
    this->mb_isNumberOn = false;
    this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF);

    if (this->mb_isPunctuationOn)
    {
        this->setKeymap(this->mlists_punctuationKeymap);
        this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_ON);
    }
    else
    {
        this->setKeymap(this->mlists_lowerKeymap);
        this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF);
    }
    this->ui->pushButton_principalKey_caps->setEnabled(!this->mb_isPunctuationOn);
}


void VirtualKeyboard::setInputWidget(QWidget *w_old, QWidget *w_new)
{
    Q_UNUSED(w_old)

    // Line Edit
    if ((this->ui->lineEdit = qobject_cast<QLineEdit *>(w_new)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
        return;
    }
    // Text Edit
    else if ((this->ui->textEdit= qobject_cast<QTextEdit *>(w_new)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_TEXTEDIT;
        return;
    }
    // Plain Text Edit
    else if ((this->ui->plainTextEdit = qobject_cast<QPlainTextEdit *>(w_new)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT;
        return;
    }
    // Editable ComboBox
    else if ((this->mw_comboBox = qobject_cast<QComboBox *>(w_new)))
    {
        // If the combobox can be edited
        if (this->mw_comboBox->isEditable())
        {
            // Writing in a combobox is in fact writing in a lineEdit, so we use the lineEdit
            this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
            this->ui->lineEdit = this->mw_comboBox->lineEdit();
            return;
        }
    }
}


void VirtualKeyboard::keyPressed(int i_indexKey)
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT && this->ui->lineEdit)
    {
        this->ui->lineEdit->insert(this->mlistw_principalKeys.at(i_indexKey)->text());
    }
    // Plain Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT && this->ui->plainTextEdit)
    {
        this->ui->plainTextEdit->insertPlainText(this->mlistw_principalKeys.at(i_indexKey)->text());
    }
    // Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT && this->ui->textEdit)
    {
        this->ui->textEdit->insertPlainText(this->mlistw_principalKeys.at(i_indexKey)->text());
    }
}


void VirtualKeyboard::on_pushButton_principalKey_space_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT && this->ui->lineEdit)
    {
        this->ui->lineEdit->insert(" ");
    }
    // Plain Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT && this->ui->plainTextEdit)
    {
        this->ui->plainTextEdit->insertPlainText(" ");
    }
    // Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT && this->ui->textEdit)
    {
        this->ui->textEdit->insertPlainText(" ");
    }
}


void VirtualKeyboard::on_pushButton_principalKey_backspace_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT && this->ui->lineEdit)
    {
        this->ui->lineEdit->backspace();
    }
    // Plain Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT && this->ui->plainTextEdit)
    {
        this->ui->plainTextEdit->textCursor().deletePreviousChar();
    }
    // Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT && this->ui->textEdit)
    {
        this->ui->textEdit->textCursor().deletePreviousChar();
    }
}


void VirtualKeyboard::on_pushButton_principalKey_caps_clicked()
{
    this->toggleCapsLock();
}


void VirtualKeyboard::on_pushButton_principalKey_numbers_clicked()
{
    this->toggleNumbers();
}


void VirtualKeyboard::on_pushButton_principalKey_punctuation_clicked()
{
    this->togglePunctuation();
}


void VirtualKeyboard::on_pushButton_principalKey_enter_clicked()
{
    emit this->enterKeyPressed();
}

/*
void VirtualKeyboard::on_pushButton_secondaryKey_copy_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT && this->ui->lineEdit)
    {
        this->ui->lineEdit->copy();
    }
    // Plain Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT && this->ui->plainTextEdit)
    {
        this->ui->plainTextEdit->copy();
    }
    // Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT && this->ui->textEdit)
    {
        this->ui->textEdit->copy();
    }
}


void VirtualKeyboard::on_pushButton_secondaryKey_cut_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT && this->ui->lineEdit)
    {
        this->ui->lineEdit->cut();
    }
    // Plain Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT && this->ui->plainTextEdit)
    {
        this->ui->plainTextEdit->cut();
    }
    // Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT && this->ui->textEdit)
    {
        this->ui->textEdit->cut();
    }
}


void VirtualKeyboard::on_pushButton_secondaryKey_paste_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT && this->ui->lineEdit)
    {
        this->ui->lineEdit->paste();
    }
    // Plain Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT && this->ui->plainTextEdit)
    {
        this->ui->plainTextEdit->paste();
    }
    // Text Edit
    else if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT && this->ui->textEdit)
    {
        this->ui->textEdit->paste();
    }
}

*/

void VirtualKeyboard::connectFocusChanged()
{
    // --- Connection to change the input widget dynamically
    this->connect(qApp, &QApplication::focusChanged,
                  this, &VirtualKeyboard::setInputWidget);
}


void VirtualKeyboard::disconnectFocusChanged()
{
    // --- Disconnection to stop changing the input widget dynamically
    this->disconnect(qApp, &QApplication::focusChanged,
                     this, &VirtualKeyboard::setInputWidget);
}

void VirtualKeyboard::on_pushButton_principalKey_Hide_clicked()
{
    m_animation->setDirection(QPropertyAnimation::Backward);
    connect (m_animation, SIGNAL(finished()), this, SLOT(canBeHidden()));
    m_animation->start();
}

void VirtualKeyboard::canBeHidden(void)
{
    disconnect (m_animation, SIGNAL(finished()), this, SLOT(canBeHidden()));
    emit hideMe();
}
