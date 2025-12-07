#include "idlisinif.h"

IdliSinif::IdliSinif():_id{0}{}
IdliSinif::IdliSinif(const IdliSinif& kaynak):_id{kaynak._id}{}

quint32 IdliSinif::id() const{return _id;}

void IdliSinif::setId(quint32 newid){_id=newid;}
