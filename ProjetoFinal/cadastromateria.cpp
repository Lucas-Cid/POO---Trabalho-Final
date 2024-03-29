#include "cadastromateria.h"
#include "ui_cadastromateria.h"
#include "Sistema.h"
#include <QMessageBox>

cadastroMateria::cadastroMateria(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cadastroMateria)
{
    ui->setupUi(this);
}

cadastroMateria::~cadastroMateria()
{
    delete ui;
}

void cadastroMateria::on_pushButton_clicked()
{
    Sistema sist("aluno", "materias", "professores");
    QString nome = ui->lineEdit_5->text();
    char nomeC[50];
    strcpy(nomeC, nome.toStdString().c_str());

    //Chama a função .adicionaMateria(), a qual retorna o resultado da operação (sucesso ou falha),
    //o qual sera mostrado em uma message box.
    QMessageBox::about(this, "Status", QString::fromStdString(sist.adicionaMateria(nomeC)));
    this->close();
}
