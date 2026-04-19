#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QEventLoop>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // czy uzytkownik wpisal dane
    QString imie = ui->lineEdit->text();
    QString faktura = ui->lineEdit_2->text();

    if (imie.isEmpty() || faktura.isEmpty()) {
        ui->plainTextEdit->setPlainText("BŁĄD: Wprowadź imię klienta i numer faktury przed wygenerowaniem maila.");
        return;
    }

    ui->plainTextEdit->setPlainText("Generowanie wiadomości. Proszę czekać...");

    // jaki ton
    QString ton = ui->comboBox->currentText();

    // zapytanie do modelu
    QString prompt = "Napisz krótkiego maila przypominającego o płatności za fakturę. "
                     "Ton wiadomości ma być: " + ton + ". "
                             "Imię klienta: " + imie + ". Numer faktury: " + faktura + ".";

    // polaczenie sieciowe
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // paczka JSON dla Ollamy
    QJsonObject json;
    json["model"] = "SpeakLeash/bielik-11b-v3.0-instruct:Q4_K_M";
    json["prompt"] = prompt;
    json["system"] = "Jesteś profesjonalnym księgowym piszącym maile do klientów.";
    json["stream"] = false;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // wyslanie zapytania i odp
    QNetworkReply* reply = manager->post(request, data);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // obsluga bledow
    if (reply->error() != QNetworkReply::NoError) {
        ui->plainTextEdit->setPlainText("BŁĄD POŁĄCZENIA: Nie można połączyć się z modelem językowym.\n"
                                        "Sprawdź, czy program Ollama jest uruchomiony.");
    } else {
        // odp
        QJsonDocument reply_json = QJsonDocument::fromJson(reply->readAll());
        QString odpowiedz = reply_json.object()["response"].toString();
        ui->plainTextEdit->setPlainText(odpowiedz);
    }

    reply->deleteLater();
    manager->deleteLater();
}

