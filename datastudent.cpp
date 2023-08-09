#include "datastudent.h"
#include "ui_datastudent.h"

dataStudent::dataStudent(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dataStudent)
{
    ui->setupUi(this);
	if(openDB())readTable();
	else{
		ui->btnInsert->setEnabled(false);
		ui->btnDelete->setEnabled(false);
		ui->btnUpdate->setEnabled(false);
		ui->btnOrderBy->setEnabled(false);
	}
}

dataStudent::~dataStudent()
{
    delete ui;
}

bool dataStudent::openDB(){
	db = QSqlDatabase::addDatabase("QSQLITE");
	//use ../dataStudent/stuDB.db because there is another directory created under the parent directory when compiling using qtcreator. This is to prevent the compiler from searching the wrong directory 
    db.setDatabaseName("../dataStudent/studDB.db");
	if(db.open()){
		return true;
	}
	else{
		qDebug()<<"Sqlite library failed to open:"<<db.lastError().text();
		return false;
	}
}
//need to use model since data has to be shown on screen
void dataStudent::readTable(){
	//under public functions of QueryModel.Save the command inside model;
    //It is possible to change the headers displayed using the following command:
        //select id newIDname, name newName, score newScoreName FROM student;
    model.setQuery("select * from student;");
	//to show data: look for functions in QTableView (inherited virtual function from QAbstractItemView)o.e. setModel(QAbstractItemModel* model);
	ui->tableView->setModel(&model);
	if(model.lastError().isValid()){
		qDebug()<<model.lastError().text();
		return;
	}
}

//OrderBy
void dataStudent::on_btnOrderBy_clicked(){
    //obtain the current value inside
    QString field= ui->cmbField->currentText();
    QString method = (ui->cmbMethod->currentText()=="Ascending")?"ASC":"DESC";
    QString sqlStr = "select * from student order by "+ field +" "+ method;
     //A better method would be to use the QString attributes provided by the library.
     //This is because it is easier to check if you typed your commands correctly
     QString sqlStr1 = QString("select id, name, score from student order by %1 %2").arg(field).arg(method);

     //It is a good practise to always print out the string first because there are many variables
      qDebug()<<sqlStr1;

     //execute the new command
     model.setQuery(sqlStr1);
     //QSqlQuery::exec() is a function that returns true and sets query state to actibe if executed
     //else returns false. After query is executed, the query is positioned on an invalid record and must be nabvigated to a vlid record before data values can be retrieved for instance by using next.
     //above only used when using select
     //You can also use prepare() to prepare a QString command first before using command;
     }

void dataStudent::on_btnInsert_clicked(){
    //Insertions
    int id = ui->spinID->value();
    //remove the spaces before and after a string
    QString name = ui->editName->text().trimmed();
    //if a space is entered or no string is typed inside
    if(name.isEmpty() || name.isNull()){
        QMessageBox::warning(this,"Insert","Please enter a name!");
        return;
    }
    double score = ui->dSpinScore->value();
    QSqlQuery sqlStr;
    sqlStr.prepare("INSERT INTO student (id,name,score) "
                   "VALUES (:id,:name,:score)");
    sqlStr.bindValue(":id",id);
    sqlStr.bindValue(":name",name);
    sqlStr.bindValue(":score",score);
    qDebug()<<sqlStr.lastQuery();
    //it is also okay to use position sqlStr.bindValue(0,id); for instance


    //It is possible to use the following but not recommended because a command can get very complicated
    /*
     QString sqlStr = QString("INSERT INTO student(id,name,score) VALUES(%1,'%2',%3)").arg(id).arg(name).arg(score);
     Note how you need to add '' to %2 because if not the compiler does not recognise the string?
     */
    if(sqlStr.exec())readTable();
    //only needs to bind to db once because it is already saved as a parameter in QT that is used by Qquery. Only when there are multiple db you need to state which db to use
    else
        qDebug()<<sqlStr.lastError().text();
}

void dataStudent::on_btnDelete_clicked(){
    int id = ui->spinID->value();
    QSqlQuery query;
    query.prepare("delete from student where id = :id");
    query.addBindValue(id); //since only 1 argument is needed
    if(query.exec())
        model.setQuery("select id, name, score from student order by id");
    else
        qDebug()<<"Failed to delete: "<<query.lastError().text();

    //to ensure that the data user is trying to delete exists
    if(query.numRowsAffected()==0){
        QMessageBox::information(this,"delete","The data you are trying to delete does not exist!");
    }
}
//model index returns the cloumn and row number i.e. the unit itself
void dataStudent::on_btnUpdate_clicked(){
    int id = ui->spinID->value();
    QString name = ui->editName->text();
    double score = ui->dSpinScore->value();
    QSqlQuery query;
    query.prepare("update student set name= :name, score= :score where id = :id");
    query.addBindValue(name);
    query.addBindValue(score);
    query.addBindValue(id); //since only 1 argument is needed
    if(query.exec())
        model.setQuery("select id, name, score from student order by id");
    else
        qDebug()<<"Failed to delete: "<<query.lastError().text();

    //to ensure that the data user is trying to delete exists
    if(query.numRowsAffected()==0){
        QMessageBox::information(this,"update","The data you are trying to update does not exist!");
    }
}
//check function -> have to use next() functions etc

//doubleClicked() -> returns the column and row of cell clicked
//convert the information to appropriate data types using toInt functions

void dataStudent::on_tableView_doubleClicked(const QModelIndex &index)
{   //index is used to locate data inside a table
    int id = index.data().toInt();
    QSqlQuery query;
    query.prepare("select * from student where id=?");
    query.addBindValue(id);
    if(query.exec()){
        if(query.next()){
            //next() because it will be pointing at the line before the first line.
            int stuNo = query.value(0).toInt();
            QString stuName= query.value(1).toString();
            double score = query.value(2).toDouble();
            ui->spinID->setValue(stuNo);
            ui->dSpinScore->setValue(score);
        }
    }
}

//above code only considers the case when user clicks on ID.
//It is possible to check the cloumn number when clickling and then writing different codes for each situation.
//Can get model to and the row/column to modify the cell. But this may mean that editing on model != editing the db as writing function still has to be used

