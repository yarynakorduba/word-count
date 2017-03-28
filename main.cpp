#include <QString>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QMap>
#include <cmath>
#include <iterator>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

using namespace std;


QStringList reading() {
    QStringList lst;
    QFile inputFile("/home/yaryna/AKS_main/one.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString l = in.readAll().toLower();
         // QString line = in.readLine();
          l = l.replace(',', ' ').replace('\'', ' ').replace('"', ' ').replace('.', ' ').replace(';', ' ').replace(':', ' ').replace('_', ' ').\
  replace('!',' ').replace('?',' ').replace('-',' ');
          lst = l.split(QRegExp("\\s+"), QString::SkipEmptyParts); //regexp


       //   qDebug(l.toUtf8());

       }
       inputFile.close();

    }
    return lst;
}

QList<QStringList> lst_division(QStringList data_lst, int threads) {
    QList<QStringList> general;
    int pointer = 0;
    int temp = 0;
    int division = std::ceil((float)data_lst.size()/threads);
    while (pointer < data_lst.size()) {
        QStringList tmp = {};
        for (int el=pointer; el < pointer+division; el++) {
            if (el < data_lst.size()) {
            tmp.append(data_lst[el]);

        }
        temp++;}

        general.append(tmp);
        pointer += division;
    }

    for (int ele = 0; ele < general.size(); ele++) {
           }
    return general;
}



QMap<QString, int> words;
QString output;
QMap<QString, int>::iterator it;
QMutex mutex;
QWaitCondition bufferNotEmpty;

class CountingThread : public QThread {

    public:
        CountingThread(QStringList data_lst);
        void run();

    protected:
        QStringList& data;


 };


CountingThread::CountingThread(QStringList data_lst): data (data_lst) {
    ;
}

void CountingThread::run() {
    for (int a=0; a<data.size(); a++) {
       // mutex.lock();
        if (!(words.contains(data[a]))) {
            words.insert(data[a], 1);
        }
        else {
            words.insert(data[a], words.value(data[a])+1);
        }
       // mutex.unlock();

      //  emit fin();
    }

}

int main(int argc, char *argv[])
{
   QStringList words_lst = reading();
   QList<QStringList> lst = lst_division(words_lst, 3);
   QList<CountingThread*> thread_lst;

   for (int el=0; el <lst.size(); el++) {

               CountingThread *thr;
               thr = new CountingThread(lst[el]);
               thread_lst.append(thr);
   }

   int i=0;
   for (auto thread: thread_lst) {

       thread->run();
       i++;
   }

  for (auto thread: thread_lst){

   thread->wait();}



   //---------------------------------------------------------------
   for(auto x: thread_lst)
       delete x;

   QMap<QString, int>::iterator it;
   for (it = words.begin(); it != words.end(); ++it) {
       // Format output here.
       output += QString("%1 : %2").arg(it.key()).arg(it.value()) + " |, ";
   }
   cout << output.toUtf8().constData() << " ";
}

