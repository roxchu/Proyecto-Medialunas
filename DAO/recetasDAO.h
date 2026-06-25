#ifndef recetasDAO_H
#define recetasDAO_H
#include "DAO.h"
#include "../models/receta.h"

class RecetaDAO : public DAO <Receta>{
    public:
    RecetaDAO();
};

#endif
