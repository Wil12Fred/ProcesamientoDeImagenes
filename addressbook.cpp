#include <QtWidgets>
#include <QPixmap>
#include <png.h>
#include "addressbook.h"
#include "functions_png.h"

AddressBook::AddressBook(QWidget *parent) : QWidget(parent) {
	contacts.insert(0, "Original");
	contacts.insert(1, "Grayscale");
	contacts.insert(2, "White and Black");
	contacts.insert(3, "Negative (CMY)");
	contacts.insert(4, "Logarithmic");//log(b,c)
	contacts.insert(5, "Exponential");//pow(e,c)
	contacts.insert(6, "Fourier transform");
	contacts.insert(7, "Inverse Fourier Transform");
	contacts.insert(8, "Filtro media - ponderada");//media(tam, pond)
	contacts.insert(9, "Filtro mediana");//mediana(tam)
	contacts.insert(10, "Filtro Sobel");
	contacts.insert(11, "Filtro Roberts");
	contacts.insert(12, "Filtro Laplace");
	contacts.insert(13, "Filtro Fourier");
	contacts.insert(14, "Dilatación");
	contacts.insert(15, "Erosión");
	contacts.insert(16, "Opening");
	contacts.insert(17, "Closing");
	contacts.insert(18, "Borde");
	contacts.insert(19, "Horizontal");
	contacts.insert(20, "Vertical");
	contacts.insert(21, "45 positivo");
	contacts.insert(22, "45 negativo");
	contacts.insert(23, "Filtro Canny");
	//
	inFourierTransform=false;
	currentOption=0;
	nameLabel = new QLabel(contacts[0]/*tr("Name:")*/);
	nameLine = new QLineEdit;
	nameLine->setReadOnly(true);
	addressText = new QLineEdit;
	addressText->setReadOnly(true);
	//
	//QLabel *addressLabel = new QLabel(tr("Address:"));
	QImage *image = new QImage("./lena.png");
	*image = image->scaled(500,500,Qt::KeepAspectRatio);
	//
	imageLabel = new QLabel();
	imageLabel->setPixmap(QPixmap::fromImage(*image));
	//
	addButton = new QPushButton(tr("&Add"));
	addButton->setEnabled(false);
	//editButton = new QPushButton(tr("&Edit"));
	//editButton->setEnabled(false);
	removeButton = new QPushButton(tr("&Save HSI"));
	removeButton->setEnabled(false);
	//findButton = new QPushButton(tr("&Find"));
	//findButton->setEnabled(false);
	submitButton = new QPushButton(tr("&Save Histogram..."));
        submitButton->setEnabled(false);
	//submitButton->hide();
	cancelButton = new QPushButton(tr("&Submit"));
	cancelButton->setEnabled(false);
	//
	nextButton = new QPushButton(tr("&Next"));
	nextButton->setEnabled(false);
	previousButton = new QPushButton(tr("&Previous"));
	previousButton->setEnabled(false);
	//
	loadButton = new QPushButton(tr("&Load..."));
	loadButton->setToolTip(tr("Load contacts from a file"));
	saveButton = new QPushButton(tr("&Save..."));
	saveButton->setToolTip(tr("Save contacts to a file"));
	saveButton->setEnabled(false);
	//
	//dialog = new FindDialog(this);
	//
	connect(addButton, SIGNAL(clicked()), this, SLOT(addContact()));
	connect(submitButton, SIGNAL(clicked()), this, SLOT(submitContact()));
	//connect(editButton, SIGNAL(clicked()), this, SLOT(editContact()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeContact()));
	//connect(findButton, SIGNAL(clicked()), this, SLOT(findContact()));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
	connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));

	QVBoxLayout *buttonLayout1 = new QVBoxLayout;
	buttonLayout1->addWidget(addButton);
	//buttonLayout1->addWidget(editButton);
	buttonLayout1->addWidget(removeButton);
	//buttonLayout1->addWidget(findButton);
	buttonLayout1->addWidget(submitButton);
	//buttonLayout1->addWidget(cancelButton);
	buttonLayout1->addWidget(loadButton);
	buttonLayout1->addWidget(saveButton);
	buttonLayout1->addStretch();

	QHBoxLayout *buttonLayout2 = new QHBoxLayout;
	buttonLayout2->addWidget(previousButton);
	buttonLayout2->addWidget(nextButton);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(imageLabel, 0, 0, Qt::AlignJustify);
	mainLayout->addWidget(nameLabel, 1, 0, Qt::AlignCenter);
	mainLayout->addLayout(buttonLayout2, 2, 0);
	//mainLayout->addWidget(addressLabel, 1, 0, Qt::AlignTop);
	mainLayout->addLayout(buttonLayout1, 0, 1);
	mainLayout->addWidget(nameLine, 1, 1);
	mainLayout->addWidget(addressText, 2, 1);
	mainLayout->addWidget(cancelButton, 3, 1);

	setLayout(mainLayout);
	setWindowTitle(tr("Simple Address Book"));
}

void AddressBook::addContact() {
	//oldName = nameLine->text();
	//oldAddress = addressText->toPlainText();
	//
	//nameLine->clear();
	//addressText->clear();
	load_file(".temp.png");
	write_png_file(".edited.png");
	if(inFourierTransform){
		if(currentOption==7){
			inFourierTransform=false;
		}
	} else {
		if(currentOption==6){
			inFourierTransform=true;
		}
	}
	updateInterface(NavigationMode);
}

void AddressBook::editContact() {
	//oldName = nameLine->text();
	//oldAddress = addressText->toPlainText();
	//
	updateInterface(EditingMode);
}

void AddressBook::submitContact() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Histogram"), "",
        tr("Comma-separated values (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
	load_file(".edited.png");
	print_histogram(fileName.toUtf8().constData());
    }
    /*QString name;// = nameLine->text();
    QString address;// = addressText->toPlainText();
	//
    if (name.isEmpty() || address.isEmpty()) {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a name and address."));
        return;
    }*/
	//
    /*if (currentMode == AddingMode) {
	//
        if (!contacts.contains(name)) {
            contacts.insert(name, address);
            QMessageBox::information(this, tr("Add Successful"),
                tr("\"%1\" has been added to your address book.").arg(name));
        } else {
            QMessageBox::information(this, tr("Add Unsuccessful"),
                tr("Sorry, \"%1\" is already in your address book.").arg(name));
        }
    } else if (currentMode == EditingMode) {
	//
        if (oldName != name) {
            if (!contacts.contains(name)) {
                QMessageBox::information(this, tr("Edit Successful"),
                    tr("\"%1\" has been edited in your address book.").arg(oldName));
                contacts.remove(oldName);
                contacts.insert(name, address);
            } else {
                QMessageBox::information(this, tr("Edit Unsuccessful"),
                    tr("Sorry, \"%1\" is already in your address book.").arg(name));
            }
        } else if (oldAddress != address) {
            QMessageBox::information(this, tr("Edit Successful"),
                tr("\"%1\" has been edited in your address book.").arg(name));
            //contacts[name] = address;
        }
    }*/
	//
    updateInterface(NavigationMode);
}

void AddressBook::cancel() {
    //nameLine->setText(oldName);
    //addressText->setText(oldAddress);
	applyOption(currentOption, ".edited.png");
	write_png_file(".temp.png");
	QImage *image = new QImage(".temp.png");
	*image = image->scaled(500,500,Qt::KeepAspectRatio);
	imageLabel->setPixmap(QPixmap::fromImage(*image));
    updateInterface(NavigationMode);
}

void AddressBook::removeContact() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Histogram"), "",
        tr("Comma-separated values (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
	load_file(".edited.png");
	print_hsi(fileName.toUtf8().constData());
    }
    //QString name;// = nameLine->text();
    //QString address;// = addressText->toPlainText();
	//
    /*if (contacts.contains(name)) {
	//
        int button = QMessageBox::question(this,
            tr("Confirm Remove"),
            tr("Are you sure you want to remove \"%1\"?").arg(name),
            QMessageBox::Yes | QMessageBox::No);
	//
        if (button == QMessageBox::Yes) {
	//
            previous();
            contacts.remove(name);
	//
            QMessageBox::information(this, tr("Remove Successful"),
                tr("\"%1\" has been removed from your address book.").arg(name));
        }
    }*/
	//
    updateInterface(NavigationMode);
}

void AddressBook::applyOption(int option, std::string file){
	/*contacts.insert(0, "Original");
	contacts.insert(1, "Grayscale");
	contacts.insert(2, "White and Black");
	contacts.insert(3, "Negative");
	contacts.insert(4, "Logarithmic");//log
	contacts.insert(5, "Exponential");//pow
	contacts.insert(6, "Fourier transform");
	contacts.insert(7, "Inverse Fourier Transform");
	contacts.insert(8, "Filtro media - ponderada");
	contacts.insert(9, "Filtro mediana");
	contacts.insert(10, "Filtro Sobel");
	contacts.insert(11, "Filtro Roberts");
	contacts.insert(12, "Filtro Laplace");*/
	load_file(file);
	if(!inFourierTransform) {
		if(option==0){
		} else if (option==1){
			to_grayscale();
		} else if (option==2){
			to_grayscale();
			to_white_and_black();
		} else if (option==3){
			to_negative();
		} else if (option==4){//log
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			to_log(b,c);
		} else if (option==5){//pow
			to_grayscale();
			double e=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			to_pow(e,c);
		} else if (option==6){
			to_grayscale();
			to_fourier();
			complex_to_image(tfourier);
		} else if (option==8){//media
			to_grayscale();
			int tam=stoi(nameLine->text().toUtf8().constData());
			double pon=stod(addressText->text().toUtf8().constData());
			filter_media(tam, pon);
		} else if (option==9){//mediana
			to_grayscale();
			int tam=stoi(nameLine->text().toUtf8().constData());
			filter_mediana(tam);
		} else if (option==10){
			to_grayscale();
			filter_roberts();
		} else if (option==11){
			to_grayscale();
			filter_sobel();
		} else if (option==12){
			to_grayscale();
			filter_laplace();
		} else if (option==14){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			int Mask[3];
			to_white_and_black(b);
			dilate(Mask, c, c);
		} else if (option==15){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			int Mask[3];
			to_white_and_black(b);
			erosion(Mask, c, c);
		} else if (option==16){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			int Mask[3];
			to_white_and_black(b);
			opening(Mask, c, c);
		} else if (option==17){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			int Mask[3];
			to_white_and_black(b);
			closing(Mask, c, c);
		} else if (option==18){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			double c=stod(addressText->text().toUtf8().constData());
			int Mask[3];
			to_white_and_black(b);
			dilateMinusErosion(Mask, c, c);
		} else if (option==19){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			filter_horizontal();
			to_white_and_black(b);
		} else if (option==20){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			filter_vertical();
			to_white_and_black(b);
		} else if (option==21){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			filter_45p();
			to_white_and_black(b);
		} else if (option==22){
			to_grayscale();
			double b=stod(nameLine->text().toUtf8().constData());
			filter_45n();
			to_white_and_black(b);
		} else if (option==23){
			to_grayscale();
			filter_canny();
		}
	} else {
		if (option==7){
			fourier_to_image(/*tfourier*/);
			complex_to_image(write_cpx);
		} else if(option==13){
			filter_fourier();
			complex_to_image(tfourier);
		}
	}
}

void AddressBook::next() {
	currentOption=(currentOption+1)%contacts.size();
	int option=currentOption;
        nameLine->setReadOnly(true);
        addressText->setReadOnly(true);
	if(!inFourierTransform) {
		write_cpx = NULL;
		tfourier = NULL;
		if (option==4){//log
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("2"));
			addressText->setText(tr("10"));
		} else if (option==5){//pow
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("2"));
			addressText->setText(tr("0.01"));
		} else if (option==8){//media
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("3"));
			addressText->setText(tr("1"));
		} else if (option==9){//mediana
			nameLine->setReadOnly(false);
			nameLine->setText(tr("3"));
		} else if (option==14){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==15){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==16){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==17){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==18){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==19){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		} else if (option==20){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		} else if (option==21){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		} else if (option==22){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		}
	}
	nameLabel->setText(contacts[currentOption]);
	applyOption(currentOption, ".edited.png");
	write_png_file(".temp.png");
	QImage *image = new QImage(".temp.png");
	*image = image->scaled(500,500,Qt::KeepAspectRatio);
	imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void AddressBook::previous() {
	currentOption=(currentOption+contacts.size()-1)%contacts.size();
	int option=currentOption;
        nameLine->setReadOnly(true);
        addressText->setReadOnly(true);
	if(!inFourierTransform) {
		write_cpx = NULL;
		tfourier = NULL;
		if (option==4){//log
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("2"));
			addressText->setText(tr("10"));
		} else if (option==5){//pow
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("2"));
			addressText->setText(tr("0.01"));
		} else if (option==8){//media
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("3"));
			addressText->setText(tr("1"));
		} else if (option==9){//mediana
			nameLine->setReadOnly(false);
			nameLine->setText(tr("3"));
		} else if (option==14){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==15){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==16){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==17){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==18){
			nameLine->setReadOnly(false);
			addressText->setReadOnly(false);
			nameLine->setText(tr("128"));
			addressText->setText(tr("3"));
		} else if (option==19){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		} else if (option==20){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		} else if (option==21){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		} else if (option==22){
			nameLine->setReadOnly(false);
			nameLine->setText(tr("128"));
		}
	}
	nameLabel->setText(contacts[currentOption]);
	applyOption(currentOption, ".edited.png");
	write_png_file(".temp.png");
	QImage *image = new QImage(".temp.png");
	*image = image->scaled(500,500,Qt::KeepAspectRatio);
	imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void AddressBook::updateInterface(Mode mode) {
    currentMode = mode;
    switch (currentMode) {
    case AddingMode:
    case EditingMode:
        //nameLine->setReadOnly(false);
        //nameLine->setFocus(Qt::OtherFocusReason);
        //addressText->setReadOnly(false);
        addButton->setEnabled(false);
        //editButton->setEnabled(false);
        removeButton->setEnabled(false);
        nextButton->setEnabled(false);
        previousButton->setEnabled(false);
        submitButton->show();
        cancelButton->show();
        loadButton->setEnabled(false);
        saveButton->setEnabled(false);
        break;
    case NavigationMode:
        if (contacts.isEmpty()) {
            //nameLine->clear();
            //addressText->clear();
        }
        addButton->setEnabled(true);
        int number = contacts.size();
        //editButton->setEnabled(number >= 1);
        removeButton->setEnabled(true);//number >= 1);
        //findButton->setEnabled(number > 2);
        nextButton->setEnabled(true);//number > 1);
        previousButton->setEnabled(true);//number > 1);
        submitButton->setEnabled(true);
        //submitButton->hide();
        cancelButton->setEnabled(true);
        loadButton->setEnabled(true);
        saveButton->setEnabled(true);//number >= 1);
        break;
    }
}

void AddressBook::saveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Address Book"), "",
        tr("Address Book (*.png);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
	load_file(".edited.png");
	write_png_file(fileName.toUtf8().constData());
    }
}

void AddressBook::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Address Book"), "",
        tr("Address Book (*.png);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
	file.close();
	QImage *image = new QImage(fileName);
	load_file(fileName.toUtf8().constData());
	write_png_file(".edited.png");
	*image = image->scaled(500,500,Qt::KeepAspectRatio);
	imageLabel->setPixmap(QPixmap::fromImage(*image));
    }
    updateInterface(NavigationMode);
}
