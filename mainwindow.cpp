#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString out = QFileDialog::getExistingDirectory(this, "Set the output folder", "./");
    ui->lineEdit->setText(out);
    outputfolder = out;
}

void MainWindow::on_pushButton_clicked()
{
    QStringList outs = QFileDialog::getOpenFileNames(this, "set the input files", "./");
    for(int i = 0; i < outs.size(); ++i){
        ui->textEdit->insertPlainText(outs[i]+'\n');
        inputfiles.append(outs[i]);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
    inputfiles.clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    about ab;
    ab.exec();
    //check
    if(inputfiles.size() > 0 && outputfolder.size() > 0){
        /***************************/
        /*merge all the files first*/
        /***************************/

        string buf;
        bool header=false;
        FILE * fi, *fo;
        fo = fopen("~tmpfile", "w");
        if(!fo){
            QMessageBox::warning(this, "ERROR", "cannot create tmp file!");
            exit(0);
        }
        for( int i = 0 ; i < inputfiles.size() ; ++i )
        {
            fi = fopen(inputfiles[i].toStdString().c_str(), "r");
            if(!fi){
                QMessageBox::warning(this, "ERROR", "cannot open file "+ inputfiles[i]);
                exit(0);
            }
            fread_a_line(fi, buf);
            if(!header){
                fprintf(fo, "%s\n", buf.c_str());
                header = true;
            }
            while(fread_a_line(fi, buf)){
                fprintf(fo, "%s\n", buf.c_str());
            }
            fclose(fi);
        }
        fclose(fo);
        /*****************************/
        /**set output*****************/
        /*****************************/
        csvreader rr;
        rr.read(string("~tmpfile").c_str(), string("\t").c_str(), string("").c_str());
        if(outputfolder[outputfolder.size() - 1] != '/')
            outputfolder += "/";
        string out_directory = outputfolder.toStdString();

        string  author_tc_filename                = out_directory+"author_tc.txt";
        string  author_count_filename             = out_directory+"author_count.txt";
        string  year_count_filename               = out_directory+"year_count.txt";
        string  document_type_filename            = out_directory+"document_type.txt";
        string  year_pages_filename               = out_directory+"year_pages.txt";
        string  year_tc_filename                  = out_directory+"year_tc.txt";
        string  country_tc_filename               = out_directory+"country_tc.txt";
        string  year_nr_filename                  = out_directory+"year_nr.txt";
        string  year_subject_count_filename       = out_directory+"year_subject_count.txt";
        string  year_publication_count_filename   = out_directory+"year_publication_count.txt";
        string  publication_tc_count_filename     = out_directory+"publication_tc_count.txt";
        string  year_country_count_filename       = out_directory+"year_country_count.txt";
        string  year_cooperation_count_filename   = out_directory+"year_cooperation_count.txt";
        string  country_relationship_filename     = out_directory+"country_relationship.txt";
        string  author_relationship_filename      = out_directory+"author_relationship.txt";
        string  institute_tc_filename             = out_directory+"institute_tc.txt";
        string  institute_relationship_filename   = out_directory+"institute_relationship.txt";
        string  year_institute_count_filename     = out_directory+"year_institute_count.txt";
        string  year_keyword_DE_filename          = out_directory+"year_keyword_DE.txt";
        string  year_keyword_ID_filename          = out_directory+"year_keyword_ID.txt";
        string  year_keyword_DE_and_ID_filename   = out_directory+"year_keyword_DE_and_ID.txt";
        string  year_title_word_filename          = out_directory+"year_title_word.txt";

        author_cited(rr,"AU", ";","TC",fopen(author_tc_filename.c_str(),"w"));
        year_author_count(rr,"PY", "AU", ";","RP" ,fopen(author_count_filename.c_str(), "w"));

        /*总量分析*/
        year_count(rr, "PY", fopen(year_count_filename.c_str(),"w"));
        year_document_type(rr,"PY","DT",fopen(document_type_filename.c_str(),"w"));
        year_pages(rr,"PY","PG",fopen(year_pages_filename.c_str(),"w"));
        year_pages(rr,"PY","TC",fopen(year_tc_filename.c_str(),"w"));
        year_pages(rr,"PY","NR",fopen(year_nr_filename.c_str(),"w"));

        /*领域分析*///same as publications
        /*year_subject == year_keyword*/
        year_subject(rr,"PY","SC", ";",fopen(year_subject_count_filename.c_str(), "w"));


        /*期刊分析*/
        year_publication_count(rr,"PY","SO",fopen(year_publication_count_filename.c_str(), "w"));
        publication_tc_count(rr,"SO","TC",fopen(publication_tc_count_filename.c_str(),"w"));

        /*地域分布及国际合作*/
        year_country_count(rr,"PY","C1", ";", "[]",fopen(year_country_count_filename.c_str(),"w"));
        year_cooperation_count(rr,"PY","C1", ";", "[]", fopen(year_cooperation_count_filename.c_str(),"w"));
        country_cited(rr,"C1", ";", "[]","TC", fopen(country_tc_filename.c_str(),"w"));
        country_relationship(rr,"C1", ";", "[]", fopen(country_relationship_filename.c_str(),"w"));
        author_relationship(rr,"AU", ";", fopen(author_relationship_filename.c_str(),"w"));

        /* 机构与合作*/
        institute_cited(rr,"C1",";","[]","TC",fopen(institute_tc_filename.c_str(),"w"));
        institute_relationship(rr,"C1", ";", "[]", fopen(institute_relationship_filename.c_str(),"w"));
        year_institute_count(rr,"PY","C1",";","[]",fopen(year_institute_count_filename.c_str(), "w"));

        /*关键词分析*/
        year_keyword(rr,"PY","DE", ";",fopen(year_keyword_DE_filename.c_str(),"w"));
        year_keyword(rr,"PY","ID", ";",fopen(year_keyword_ID_filename.c_str(),"w"));
        year_dual_keyword(rr,"PY", "DE", ";", "ID", ";", fopen(year_keyword_DE_and_ID_filename.c_str(),"w"));

        year_keyword(rr,"PY","TI", "\'()\",. ;",fopen(year_title_word_filename.c_str(),"w"));

        this->close();
        about ab;
        ab.exec();
    }
    else{
        QMessageBox::warning(this, "ERROR", "please set the input files and output folder!", QMessageBox::Ok);
    }
}
