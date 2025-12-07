#pragma once

enum class CinsiyetEnum {
    ERKEK,
    KADIN,
    BELIRTILMEMIS
};

enum class BulguTipiEnum {
    SEMPTOM,
    FIZIKSEL_MUAYENE,
    LABARATUVAR,
    GORUNTULEME
};

enum class TetkikDurumuEnum {
    ISTENDI,
    ORNEK_ALINDI,
    CALISILIYOR,
    SONUCLANDI,
    IPTAL_EDILDI
};
