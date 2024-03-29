#include "adicionarfalta.h"
#include "ui_adicionarfalta.h"
#include "Materia.h"
#include <fstream>
#include <iostream>
#include <QMessageBox>

adicionarFalta::adicionarFalta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adicionarFalta)
{
    contadorImpressao = 0;
    pos = 0;
    ui->setupUi(this);
}

adicionarFalta::~adicionarFalta()
{
    delete ui;
}

//No primeiro botão pressionado, sera capturado o nome da materia
//para a qual se deseja fazer a chamada
void adicionarFalta::on_pushButton_clicked()
{
    QString materiaQS = ui->lineEdit_5->text();
    ui->lineEdit_5->setText("");
    char materia[50];
    strcpy(materia, materiaQS.toStdString().c_str());
    string file = "materias";
    fstream arq(file.c_str(), ios::binary | ios::in | ios::out);
    if (!arq.is_open())
    {
        QMessageBox::about(this, "Status", "Falha ao abrir o arquivo");
        this->close();
    }
    Materia* auxMateria = new Materia;

    arq.read(reinterpret_cast<char*>(auxMateria), sizeof (Materia));
    while(!arq.eof() && strcmp(auxMateria->getNome(), materia))
    {
        arq.read( reinterpret_cast<char *>(auxMateria), sizeof (Materia));
        pos++;
    }

    if (strcmp(auxMateria->getNome(), materia))
        {
        QMessageBox::about(this, "Status", "Materia nao encontrada");
        this->close();
        }
    this->materia = auxMateria;

    //Após a leitura do arquivo, é feita a mudança da caixa de texto para o nome
    //do primeiro aluno do vetor de alunos da materia (caso haja algum)
    if (this->materia->getQtd() <= 0)
    {
        QMessageBox::about(this, "Status", "Nenhum aluno cadastrado na materia");
        this->close();
    }
    ui->label_6->setText(QString::fromStdString((this->materia->imprimePosicao(contadorImpressao))));
    arq.close();
}

//Em seguida, o botão anterior é excluido e substituido por um novo.
//Este fica responsável por adicionar a falta à um determinado aluno
void adicionarFalta::on_pushButton_3_clicked()
{
    QString falta = ui->lineEdit_5->text();
    ui->lineEdit_5->setText("");
    materia->acrescentarFalta(contadorImpressao, falta.toInt());
    contadorImpressao++;

    //Cada vez que o botão é pressionado, o texto da interface muda para o nome
    //de outro aluno
    if (contadorImpressao < materia->getQtd())
        ui->label_6->setText(QString::fromStdString((materia->imprimePosicao(contadorImpressao))));

    //Quando todo o vetor é percorrido, é feita a escrita do arquivo, na mesma pocição
    //em que a materia havia sido lida
    if (contadorImpressao >= materia->getQtd())
    {
        char file[] = {"materias"};
        fstream arq(file, ios::binary | ios::in | ios::out);
        if (!arq.is_open())
        {
            QMessageBox::about(this, "Status", "Falha ao abrir o arquivo");
            this->close();
        }
        arq.seekg(pos * sizeof(Materia));
        arq.write(reinterpret_cast<char *>(materia), sizeof (Materia));
        arq.close();
        QMessageBox::about(this, "Status", "Chamada realizada com sucesso");
        this->close();
    }
}
