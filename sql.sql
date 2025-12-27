CREATE TABLE doktorlar ( 
    id INT IDENTITY(1,1) NOT NULL, 
    ad NVARCHAR(50) NOT NULL, 
    soyad NVARCHAR(50) NOT NULL, 
    telefon NVARCHAR(20) NULL, 
    diploma_no NVARCHAR(50) NULL, 
    uzmanlik NVARCHAR(100) NULL, 
    PRIMARY KEY CLUSTERED (id ASC));

CREATE TABLE hastalar ( 
    id INT IDENTITY(1,1) NOT NULL, 
    tc_no NVARCHAR(11) NOT NULL, 
    ad NVARCHAR(50) NOT NULL, 
    soyad NVARCHAR(50) NOT NULL, 
    telefon NVARCHAR(20) NULL, 
    dogum_tarihi DATE NULL, 
    cinsiyet INT NULL, 
    adres NVARCHAR(MAX) NULL, 
    kan_grubu NVARCHAR(10) NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    UNIQUE NONCLUSTERED (tc_no ASC));

CREATE TABLE hasta_alerjiler ( 
    id INT IDENTITY(1,1) NOT NULL, 
    hasta_id INT NOT NULL, 
    alerji_adi NVARCHAR(100) NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_alerjiler_hastalar 
        FOREIGN KEY (hasta_id) REFERENCES hastalar (id) ON DELETE CASCADE);

CREATE TABLE hasta_kronik_hastaliklar ( 
    id INT IDENTITY(1,1) NOT NULL, 
    hasta_id INT NOT NULL, 
    hastalik_adi NVARCHAR(100) NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_kronik_hastalar 
        FOREIGN KEY (hasta_id) REFERENCES hastalar (id) ON DELETE CASCADE);

CREATE TABLE ziyaretler ( 
    id INT IDENTITY(1,1) NOT NULL, 
    tarihsaat DATETIME NULL, 
    sikayet NVARCHAR(MAX) NULL, 
    tani NVARCHAR(MAX) NULL, 
    tedavinotlari NVARCHAR(MAX) NULL, 
    doktorid INT NULL, 
    hastaid INT NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_ziyaretler_doktorlar 
        FOREIGN KEY (doktorid) REFERENCES doktorlar (id) ON DELETE SET NULL, 
    CONSTRAINT fk_ziyaretler_hastalar 
        FOREIGN KEY (hastaid) REFERENCES hastalar (id) ON DELETE CASCADE);

CREATE TABLE bulgular ( 
    id INT IDENTITY(1,1) NOT NULL, 
    aciklama NVARCHAR(MAX) NULL, 
    tip INT NULL, 
    kaydedilmezamani DATETIME NULL, 
    ziyaretid INT NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_bulgular_ziyaretler 
        FOREIGN KEY (ziyaretid) REFERENCES ziyaretler (id) ON DELETE CASCADE);


CREATE TABLE ilaclar ( 
    id INT IDENTITY(1,1) NOT NULL, 
    barkod NVARCHAR(50) NULL, 
    ad NVARCHAR(100) NULL, 
    etkenmadde NVARCHAR(100) NULL, 
    form NVARCHAR(50) NULL, 
    dozajbilgisi NVARCHAR(100) NULL, 
    PRIMARY KEY CLUSTERED (id ASC));


CREATE TABLE receteler ( 
    id INT IDENTITY(1,1) NOT NULL, 
    tarih DATE NULL, 
    gecerliliksuresi INT NULL, 
    ziyaretid INT NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_receteler_ziyaretler 
        FOREIGN KEY (ziyaretid) REFERENCES ziyaretler (id) ON DELETE CASCADE);

CREATE TABLE tetkikler ( 
    id INT IDENTITY(1,1) NOT NULL, 
    ad NVARCHAR(100) NULL, 
    aciklama NVARCHAR(MAX) NULL, 
    nderkek NVARCHAR(100) NULL, 
    ndkadin NVARCHAR(100) NULL, 
    PRIMARY KEY CLUSTERED (id ASC));

CREATE TABLE istenentetkikler ( 
    id INT IDENTITY(1,1) NOT NULL, 
    istektarihi DATETIME NULL, 
    sonuctarihi DATETIME NULL, 
    sonuc NVARCHAR(MAX) NULL, 
    yorum NVARCHAR(MAX) NULL, 
    durum INT NULL, 
    ziyaretid INT NULL, 
    tetkikid INT NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_istenentetkikler_tetkikler 
        FOREIGN KEY (tetkikid) REFERENCES tetkikler (id), 
    CONSTRAINT fk_istenentetkikler_ziyaretler 
        FOREIGN KEY (ziyaretid) REFERENCES ziyaretler (id) ON DELETE CASCADE);


CREATE TABLE recetekalemleri ( 
    id INT IDENTITY(1,1) NOT NULL, 
    kullanimsekli NVARCHAR(100) NULL, 
    doz NVARCHAR(50) NULL, 
    periyot NVARCHAR(50) NULL, 
    adet INT NULL, 
    receteid INT NULL, 
    ilacid INT NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_recetekalemleri_ilaclar 
        FOREIGN KEY (ilacid) REFERENCES ilaclar (id), 
    CONSTRAINT fk_recetekalemleri_receteler 
        FOREIGN KEY (receteid) REFERENCES receteler (id) ON DELETE CASCADE);


CREATE TABLE duyurular ( 
    id INT IDENTITY(1,1) NOT NULL, 
    baslik NVARCHAR(255) NULL, 
    metin NVARCHAR(MAX) NULL, 
    tarih DATE NULL, 
    PRIMARY KEY CLUSTERED (id ASC));

CREATE TABLE doktorpass ( 
    id INT IDENTITY(1,1) NOT NULL, 
    kid INT NOT NULL, 
    sifre NVARCHAR(100) NOT NULL, 
    PRIMARY KEY CLUSTERED (id ASC), 
    CONSTRAINT fk_doktorpass_doktorlar 
        FOREIGN KEY (kid) REFERENCES doktorlar (id) ON DELETE CASCADE);

CREATE TABLE yoneticipass ( 
    id INT IDENTITY(1,1) NOT NULL, 
    kid INT NOT NULL, 
    sifre NVARCHAR(100) NOT NULL, 
    PRIMARY KEY CLUSTERED (id ASC));
