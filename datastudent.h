#ifndef DATASTUDENT_H
#define DATASTUDENT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class dataStudent; }
QT_END_NAMESPACE

class dataStudent : public QMainWindow
{
	Q_OBJECT

	public:
		dataStudent(QWidget *parent = nullptr);
		~dataStudent();
	private:
		//to open and access database 
		bool openDB();
		void readTable();
    public slots:
        void on_btnOrderBy_clicked();
        void on_btnInsert_clicked();
        void on_btnDelete_clicked();
        void on_btnUpdate_clicked();
private slots:
        void on_tableView_doubleClicked(const QModelIndex &index);

private:
		Ui::dataStudent *ui;
		QSqlDatabase db;//add Database returns an object so there is no need to use a pointer here 
		QSqlQueryModel model; //database model. 
};
#endif // DATASTUDENT_H
