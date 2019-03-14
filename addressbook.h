#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QMap>
//#include "finddialog.h"

class QPushButton;
class QLabel;
class QLineEdit;
class QTextEdit;

class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget *parent = 0);
    enum Mode { NavigationMode, AddingMode, EditingMode };

public slots:
    void addContact();
    void editContact();
    void submitContact();
    void cancel();
    void removeContact();
    //void findContact();
    void next();
    void previous();
    void saveToFile();
    void loadFromFile();

private:
	void updateInterface(Mode mode);
	void applyOption(int option, std::string file);

	QPushButton *addButton;
	//QPushButton *editButton;
	QPushButton *removeButton;
	//QPushButton *findButton;
	QPushButton *submitButton;
	QPushButton *cancelButton;
	QPushButton *nextButton;
	QPushButton *previousButton;
	QPushButton *loadButton;
	QPushButton *saveButton;
	QLabel *imageLabel;
	QLabel *nameLabel;

	QLineEdit *nameLine;
	QLineEdit *addressText;

	int currentOption;
	bool inFourierTransform;
	QMap<int, QString> contacts;
	//FindDialog *dialog;
	QString oldName;
	QString oldAddress;
	Mode currentMode;
};

#endif
