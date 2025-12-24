#include "receteekle.h"
#include "ui_receteekle.h"
#include "../../Veri/veritabani.h"

receteekle::receteekle(quint32 kid,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::receteekle),
    m_kid(kid)
{
    ui->setupUi(this);
    cbZiyaret_guncelle();
    cbIlac_guncelle();
    ui->deReceteTarihi->setDate(QDate::currentDate());
    connect(ui->lwReceteKalemleri,&QListWidget::itemSelectionChanged, this, &receteekle::lwReceteKalemleri_silmesecimi);
    ui->lwReceteKalemleri->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

receteekle::~receteekle()
{
    delete ui;
}

ReceteTablosu::VeriPointer receteekle::getVeri() const
{
    //recete
    veri->setGecerlilikSuresi(ui->leGecerlilikSuresi->text().toUInt());
    veri->setTarih(ui->deReceteTarihi->date());
    quint32 ziyaretid =m_kid;
    veri->setZiyaretId(ziyaretid);
    //recete kalemi
    for(auto i=0;i<ui->lwReceteKalemleri->count();i++){
        auto kalem=VERITABANI::vt().recetekalemleri().olustur();
        QString satir = ui->lwReceteKalemleri->item(i)->text();
        QStringList parcalar = satir.split(" , ");
        if (parcalar.size() == 5) {
            quint32 ilacid=parcalar[0].section("ID: ", 1).section(" ",0,0).toUInt();
            kalem->setIlacID(ilacid);
            kalem->setAdet(parcalar[1].toUInt());
            kalem->setDoz(parcalar[2]);
            kalem->setPeriyot(parcalar[3]);
            kalem->setKullanimSekli(parcalar[4]);
            veri->kalemEkle(kalem);
        }
    }
    return veri;
}

void receteekle::setVeri(const ReceteTablosu::VeriPointer &newVeri)
{
    veri = newVeri;
    ui->leGecerlilikSuresi->setText(QString::number(veri->gecerlilikSuresi()));
}

void receteekle::on_btnEkle_clicked()
{
    QString girilenrecetekalemi;

    girilenrecetekalemi.append(ui->cbIlaclar->currentText()+" , "+
                               ui->leAdet->text()+" , "+ui->leDoz->text()+" , "
                               +ui->lePeriyot->text()+" , "+ui->teKullanimSekli->toPlainText());
    if(!girilenrecetekalemi.isEmpty()&&
        !ui->leAdet->text().isEmpty()&&
        !ui->leDoz->text().isEmpty()&&
        !ui->lePeriyot->text().isEmpty()&&
        !ui->teKullanimSekli->toPlainText().isEmpty()
        )
    {
        ui->lwReceteKalemleri->addItem(girilenrecetekalemi);
        ui->leAdet->clear();
        ui->leDoz->clear();
        ui->lePeriyot->clear();
        ui->teKullanimSekli->clear();
    }
}


void receteekle::on_btnSil_clicked()
{
    QListWidgetItem *item = ui->lwReceteKalemleri->currentItem();
    if(item){
        delete item;
    }
}
void receteekle::lwReceteKalemleri_silmesecimi(){
    bool secimVarMi= !ui->lwReceteKalemleri->selectedItems().isEmpty();
    ui->btnSil->setEnabled(secimVarMi);
}

void receteekle::cbZiyaret_guncelle()
{
    auto ziyaret=VERITABANI::vt().ziyaretler().IdyeGoreAra(m_kid);
    ui->cbZiyaretler->clear();

    QString a;
    a.append("ID: ");
    a.append(QString::number(ziyaret->id())+" , ");
    auto hasta=VERITABANI::vt().hastalar().IdyeGoreAra(ziyaret->hastaid());
    auto doktorlar = VERITABANI::vt().doktorlar().bul([&](DoktorTablosu::VeriPointer d){
        return d->id()==ziyaret->doktorid();
    });
    a.append(hasta->adi()+" ");
    a.append(hasta->soyadi()+", ");
    if (!doktorlar.isEmpty()) {
        auto doktor = doktorlar[0];
        a.append("Doktor:"+doktor->adi()+" "+doktor->soyadi()+", ");
    } else {
        a.append("Doktor: Bilinmeyen Doktor, ");
    }

    a.append("Tanı:"+ziyaret->tani()+", ");
    a.append(ziyaret->tarihsaat().toString());

    ui->cbZiyaretler->addItem(a);
}

void receteekle::cbIlac_guncelle()
{
    auto ilaclar = VERITABANI::vt().ilaclar().tumu();
    for(auto &i:ilaclar){
        ui->cbIlaclar->addItem("ID: "+QString::number(i->id())+" "+i->ad());
    }
}
