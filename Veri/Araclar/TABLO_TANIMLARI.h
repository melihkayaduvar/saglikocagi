#pragma once

#include "TABLO.h"

#include "../Siniflar/doktor.h"
#include "../Siniflar/hasta.h"
#include "../Siniflar/ziyaret.h"
#include "../Siniflar/recete.h"
#include "../Siniflar/tetkik.h"
#include "../Siniflar/ilac.h"
#include "../Siniflar/istenentetkik.h"
#include "../Siniflar/recetekalemi.h"
#include "../Siniflar/bulgu.h"
#include "../Siniflar/loginpass.h"
#include "../Siniflar/duyuru.h"

using DoktorTablosu = TABLO<DOKTOR>;
using HastaTablosu = TABLO<HASTA>;
using ZiyaretTablosu = TABLO<ZIYARET>;
using BulguTablosu = TABLO<BULGU>;
using IstenenTetkikTablosu = TABLO<ISTENENTETKIK>;

using ReceteTablosu = TABLO<RECETE>;
using TetkikTablosu = TABLO<TETKIK>;
using IlacTablosu = TABLO<ILAC>;
using ReceteKalemiTablosu = TABLO<RECETEKALEMI>;

using LoginPassTablosu = TABLO<loginpass>;

using DuyuruTablosu = TABLO<Duyuru>;


