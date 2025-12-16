#pragma once

#include <functional>
#include <QVector>
#include <memory>
#include <QDebug>

template <class T>
class TABLO{
public:
    using VeriSinifi=T;
    using VeriPointer=std::shared_ptr<VeriSinifi>;
    using VeriDizisi=QVector<VeriPointer>;
    using SorguFonksiyonu=std::function<bool(VeriPointer)>;

    using SqlEklemeFonksiyonu = std::function<void(VeriPointer)>;
    using SqlSilmeFonksiyonu = std::function<void(quint32)>;
    using SqlGuncellemeFonksiyonu = std::function<void(VeriPointer)>;
    VeriPointer olustur(){
        return VeriPointer(new VeriSinifi);
    }


    void ekle(VeriPointer yeni_nesne){

        if(_sqlEklemeIslemi){
            _sqlEklemeIslemi(yeni_nesne);
        } else {
            yeni_nesne->setId(_sonid++);
        }
        this->_elemanlar.append(yeni_nesne);
    }

    bool IdyeGoreSil(quint32 id){
        for (quint32 i=0;i<_elemanlar.size();i++) {
            if(_elemanlar[i]->id()==id){
                if(_sqlSilmeIslemi){
                    _sqlSilmeIslemi(id);
                }
                _elemanlar.removeAt(i);
                return true;
            }
        }
        return false;
    }

    void setSqlEklemeIslemi(SqlEklemeFonksiyonu func) { _sqlEklemeIslemi = func; }
    void setSqlSilmeIslemi(SqlSilmeFonksiyonu func) { _sqlSilmeIslemi = func; }

    void ramEkle(VeriPointer nesne) {
        this->_elemanlar.append(nesne);
        if(nesne->id() >= _sonid) _sonid = nesne->id() + 1;
    }

    VeriPointer IdyeGoreAra(quint32 id){
        if(_elemanlar.size()<100){
            for(auto eleman:_elemanlar){
                if(id==eleman->id()){
                    return eleman;
                }
            }
        }else{
            quint32 baslangic=0;
            quint32 bitis=_elemanlar.size();
            quint32 orta;
            while (baslangic<bitis) {
                orta=(baslangic+bitis)/2;
                if(id==_elemanlar[orta]->id()){
                    return _elemanlar[orta];
                }
                if(id>_elemanlar[orta]->id()){
                    baslangic=orta;
                    continue;
                }
                if(id<_elemanlar[orta]->id()){
                    bitis=orta;
                    continue;
                }
            }
        }
        throw std::runtime_error("hata");
    }

    VeriDizisi bul(SorguFonksiyonu sorgu){
        VeriDizisi aramaSonucu;
        for(auto &eleman:_elemanlar){
            if(sorgu(eleman)){
                aramaSonucu.append(eleman);
            }
        }
        return aramaSonucu;
    }
    VeriDizisi tumu(){
        return bul([](VeriPointer d){
            return true;
        });
    }
    bool duzenle(quint32 id, std::function<void(VeriPointer&)> duzenlemeIslemi) {
        try {
            VeriPointer nesne = IdyeGoreAra(id);
            duzenlemeIslemi(nesne);
            if(_sqlGuncellemeIslemi){
                _sqlGuncellemeIslemi(nesne);
            }

            return true;
        } catch (const std::exception& e) {
            return false;
        }
    }


    void setSqlGuncellemeIslemi(SqlGuncellemeFonksiyonu func) { _sqlGuncellemeIslemi = func; }
    quint32 boyut(){
        return _elemanlar.size();
    }
private:
    TABLO():_sonid{1} {};
    VeriDizisi _elemanlar;
    quint32 _sonid;

    SqlEklemeFonksiyonu _sqlEklemeIslemi = nullptr;
    SqlSilmeFonksiyonu _sqlSilmeIslemi = nullptr;
    SqlGuncellemeFonksiyonu _sqlGuncellemeIslemi = nullptr;

    friend class VERITABANI;
};
