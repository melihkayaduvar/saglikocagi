# 🏥 Sağlık Ocağı Yönetim Sistemi

Sağlık Ocağı Yönetim Sistemi; doktorların ve yöneticilerin klinik veya hastane süreçlerini kolayca yönetebilmesi için C++ ve Qt Framework kullanılarak geliştirilmiş, kullanıcı dostu bir masaüstü otomasyon uygulamasıdır. 

## 🚀 Özellikler

* **Rol Bazlı Giriş Sistemi:** Doktor ve Yönetici (Admin) olarak iki farklı yetki seviyesiyle güvenli oturum açma.
* **Yönetici Paneli:** 
  * Sistem istatistiklerinin anlık takibi (Toplam doktor, kayıtlı hasta, ziyaret, reçete sayısı).
  * İstatistiksel analiz: *En çok yazılan ilaç* tespiti.
  * Kurum içi duyuru oluşturma, listeleme ve yönetme.
  * İlaç, tetkik, doktor ve hasta kayıtlarına tam erişim ve yönetim (CRUD işlemleri).
* **Doktor Paneli:**
  * Hasta arama, kayıt oluşturma ve detaylı profil/geçmiş inceleme.
  * Muayene (Ziyaret) oluşturma; şikayet, tanı ve tedavi notları ekleme.
  * Hastaya özel reçete oluşturma ve ilaç yazma.
  * Laboratuvar için tetkik isteme ve tıbbi bulgu ekleme.
* **Dinamik Arama & Filtreleme:** Veri tablolarında (hasta, doktor, ziyaret vb.) anlık çalışan gelişmiş arama özelliği.

## 🛠️ Gereksinimler

Projeyi kendi ortamınızda derleyip çalıştırabilmek için aşağıdaki yazılımların kurulu olması gereklidir:

* **C++ Derleyicisi:** GCC (MinGW) veya MSVC (C++17 veya üzeri önerilir).
* **Qt Framework:** Qt 5 veya Qt 6 serisi (GUI ve QWidgets modülleri ile birlikte).
* **Geliştirme Ortamı:** Qt Creator (Tavsiye edilen IDE) veya Qt eklentisine sahip Visual Studio.

## ⚙️ Kurulum ve Çalıştırma

1. Proje kaynak kodlarını bilgisayarınıza indirin (ZIP olarak veya `git clone` komutu ile).
2. **Qt Creator** uygulamasını başlatın.
3. Menüden `File -> Open File or Project` (Dosya veya Proje Aç) yolunu izleyerek proje klasöründeki `.pro` (veya `CMakeLists.txt`) dosyasını seçin.
4. Projeyi yapılandırmak için bilgisayarınızdaki uygun derleyici kitini (Örn: Desktop Qt 6.x.x MinGW 64-bit) seçin.
5. Arayüzün sol alt köşesinde bulunan **Run (Yeşil Üçgen)** butonuna tıklayarak (veya `Ctrl+R` kısayoluyla) projeyi derleyip çalıştırın.
6. Giriş ekranında doktor veya yönetici ID'nizi ve şifrenizi girerek sisteme erişim sağlayın.

---

## 📝 Yapılacaklar (To-Do List)

Geliştirme süreci devam eden ve yakın zamanda sisteme entegre edilecek olan özellikler:

- [x] Doktor ve yönetici girişlerinde kullanıcı ID'si alınarak, ilgili işlemlerin bu ID üzerinden gerçekleştirilmesi.
- [x] Veritabanı sistemi SQL'e geçirildiği için, düzenleme (update/edit) fonksiyonlarının yeni SQL yapısına uyarlanması.
- [x] Doktor arayüzü bileşenlerinin tamamlanması ve eksik işlevlerin uygulanması.
- [ ] Yönetici arayüzüne “Yeni Yönetici Ekle” özelliğinin eklenmesi ve işlevsel hale getirilmesi.
- [ ] Yönetici arayüzündeki “Yedekle / Dışa Aktar” butonunun gerekli işlemleri yapacak şekilde aktif hale getirilmesi.

*(Not: Başındaki `+` işareti olan maddeler yapılmış sayılmıştır ve tik ile işaretlenmiştir.)*
