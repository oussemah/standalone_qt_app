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

#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QFrame>
#include <QSignalMapper>
#include <QRegExp>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QPointer>
#include <QPropertyAnimation>

#include "ui_VirtualKeyboard.h"


// Exit codes for initialisation
#define VIRTUALKEYBOARD_SUCCESS 0
#define VIRTUALKEYBOARD_UNKNOWLANGUAGE  1
#define VIRTUALKEYBOARD_INIT_FAILED     2

// Types of input widget
#define VIRTUALKEYBOARD_INPUT_LINEEDIT      0
#define VIRTUALKEYBOARD_INPUT_TEXTEDIT      1
#define VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT 2
#define VIRTUALKEYBOARD_INPUT_UNKNOWINPUTTYPE -1

// String used on some special keys
#define VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_ON       "A/a"
#define VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF      "123"
#define VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF  ",:!/"
#define VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_ON   "A/a"


namespace Ui {
class VirtualKeyboard;
}

/**
 * \brief Virtual Keyboard widget
 */
class VirtualKeyboard : public QFrame
{
    Q_OBJECT


    // Private Members
private:

    QPropertyAnimation *m_animation;

    QString m_textValue;
    /**
     * User interface for the virtual keyboard
     */
    Ui::VirtualKeyboard *ui;

    /**
     * Pointer used to interact with a lineEdit
     */
    QLineEdit *mw_lineEdit;

    /**
     * Pointer used to interact with a TextEdit
     */
    QTextEdit *mw_textEdit;

    /**
     * Pointer used to interact with a PlainTextEdit
     */
    QPlainTextEdit *mw_plainTextEdit;

    /**
     * Pointer used ONLY to test if the widget is a comboBox. If it is, we use the lineEdit pointer to save the comboBox's lineEdit
     */
    QComboBox *mw_comboBox;

    /**
     * Map the non specific "primary" keys to the keyPressed slot
     */
    QSignalMapper mo_mapperPrimaryKeys;

    /**
     * List of non specific buttons ([A - Z], [0 - 9], ...)
     */
    QList<QPushButton *> mlistw_principalKeys;

    /**
     * Lower letters Keys list
     */
    QList<QString> mlists_lowerKeymap;

    /**
     * Upper letters Keys list
     */
    QList<QString> mlists_upperKeymap;

    /**
     * Numbers Keys list
     */
    QList<QString> mlists_numbersKeymap;

    /**
     * Punctuation Keys list
     */
    QList<QString> mlists_punctuationKeymap;

    /**
     * Caps lock state
     */
    bool mb_isCapsOn;

    /**
     * Numbers state
     */
    bool mb_isNumberOn;

    /**
     * Punctuation state
     */
    bool mb_isPunctuationOn;

    /**
     * Type of input widget
     *
     * Possible values :
     *  \li VIRTUALKEYBOARD_INPUT_LINEEDIT
     *  \li VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT
     *  \li VIRTUALKEYBOARD_INPUT_TEXTEDIT
     */
    int mi_inputType;


    // Public Functions
public:

    /**
     * \brief Constructor
     * \param w_parent : parent Widget (default 0)
     */
    explicit VirtualKeyboard(QWidget *w_parent = 0);

    /**
     * \brief Destructor
     *
     * Delete member pointers
     */
    ~VirtualKeyboard();

    /**
     * \brief initialisation
     *
     * \param[in] w_inputWidget : Widget to interact with. Can be of the following types :
     *      \li QLineEdit
     *      \li QTextEdit
     *      \li QPlainTextEdit
     *      \li QComboBox (editable)
     *
     * \param[in] s_language : Language used to set the keymaps. Possible choices are :
     *      \li "EN" (=> qwerty, default value)
     *      \li "FR" (=> azerty)
     *
     * \param[in] b_displayBorder : if true, a border will be displayed around the keyboard (default false)
     *
     * \return
     *      \li VIRTUALKEYBOARD_SUCCESS if no error occured
     *      \li VIRTUALKEYBOARD_UNKNOWLANGUAGE if the language passed is unknown
     */
    int initialisation(QWidget* input, QString s_language = "EN", bool b_displayBorder = false);

    /**
     * \brief Connect QApplication::focusChanged to VirtualKeyboard::setInputWidget to change the input widget dynamically
     */
    void connectFocusChanged();

    /**
     * \brief Disconnect QApplication::focusChanged from VirtualKeyboard::setInputWidget to stop changing the input widget dynamically
     */
    void disconnectFocusChanged();

    // Private Functions
private:

    /**
     * \brief initialise the keymaps
     * \param[in] s_language : Language used to set the keymaps. Possible choices are :
     *      \li "EN" (=> qwerty, default value)
     *      \li "FR" (=> azerty)
     * \return True if the language is supported and the keymaps have been set, else False
     */
    bool initialisationKeymaps(QString s_language);

    /**
     * \brief Set the keymap from a list of QString
     * \param[in] lists_keys : list of keys
     */
    void setKeymap(QList<QString> &lists_keys);

    /**
     * \brief Toggle the Caps lock state
     */
    void toggleCapsLock();

    /**
     * \brief Toggle the numbers Keys
     */
    void toggleNumbers();

    /**
     * \brief Toggle the punctuation Keys
     */
    void togglePunctuation();


    // Signals
signals:

    /**
     * \brief Signal emitted when Enter key is pressed
     */
    void enterKeyPressed();


    /**
     * Signal emitted when we are done entering text
     *
     */
    void hideMe(void);

    // Public Slots
public slots:

    // Private Slots
private slots:

    /**
     * \brief Slot connected to the QApplication::focusChanged(QWidget old, QWidget new) signal
     *
     * It allows to dynamically change the widget to interact with, depending on wich widget has received the focus
     *
     * \param[in] w_old : last focused widget (Unused here)
     * \param[in] w_new : Newly focused widget
     * \return true if \a w_new is of supported type, false if not
     */
    void setInputWidget(QWidget *w_old, QWidget *w_new);

    /**
     * \brief Slot called on each non specific key press
     * \param[in] i_indexKey : Index mapped to the key via the QSignalMapper mo_mapperPrimaryKeys
     */
    void keyPressed(int i_indexKey);

    /**
     * \brief Slot called when pushButton_principalKey_caps is clicked
     *
     * Toggle the caps lock
     */
    void on_pushButton_principalKey_caps_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_numbers is clicked
     *
     * Toggle the numbers keys
     */
    void on_pushButton_principalKey_numbers_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_punctuation is clicked
     *
     * Toggle the punctuation keys
     */
    void on_pushButton_principalKey_punctuation_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_space is clicked
     *
     * Send a space
     */
    void on_pushButton_principalKey_space_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_backspace is clicked
     *
     * Simulate a backspace press (erase selected text / text to the right of the cursor)
     */
    void on_pushButton_principalKey_backspace_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_enter is clicked
     *
     * Doesn't do anything for now
     */
    void on_pushButton_principalKey_enter_clicked();

    /**
     * \brief Slot called when on_pushButton_principalKey_Hide is clicked
     *
     * Hides the keyboard
     */
    void on_pushButton_principalKey_Hide_clicked();

    /**
     * \brief Slot called when pushButton_secondaryKey_copy is clicked
     *
     * Copy selected text to clipboard
     *
     */
    //void on_pushButton_secondaryKey_copy_clicked();

    /**
     * \brief Slot called when pushButton_secondaryKey_cut is clicked
     *
     * Cut selected text to clipboard
     */
    //void on_pushButton_secondaryKey_cut_clicked();

    /**
     * \brief Slot called when pushButton_secondaryKey_paste is clicked
     *
     * Paste text currently in clipboard to the selected text input zone
     */
    //void on_pushButton_secondaryKey_paste_clicked();

    void canBeHidden(void);

};

#endif // VIRTUALKEYBOARD_H
