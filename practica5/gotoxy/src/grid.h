//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid
{
    public:
        Grid()
        {
            array.resize((int)(width/tile));
            for (auto &row : array)
                row.resize((int)(width/tile));
            int k=0;
            for (int i = hmin; i < width/2; i += tile, k++)
            {
                int l=0;
                for (int j = hmin; j < width/2; j += tile, l++)
                {
                    array[k][l] = Value{false, nullptr, i, j};
                }
            }
        };

        struct Value
        {
            bool occupied = false;
            QGraphicsItem * paint_cell = nullptr;
            int cx, cy;
            int dist = 0; //dist vecinos
        };

        std::vector<std::vector<Value>> array;

        void create_graphic_items(QGraphicsScene &scene)
        {
            for (auto &row : array)
                for (auto &elem : row)
                {
                    elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("Green")),
                                                    QBrush(QColor("Green")));
                    elem.paint_cell->setPos(elem.cx, elem.cy);
                }
        }
    /*
 * Inicializamos el array a false, osea, no ocupades.
 */
    void inicializate()
    {
        for (int i = 0; i < this->tam; ++i) {
            for (int j = 0; j < this->tam; ++j) {
                this->array[i][j] = false;
            }
        }
    }

public:
    /**
     * modificamos en funcion de v la coordenada x,z
     * @param x
     * @param z
     * @param v
     */
    //void set_value(int x, int z, bool v){

    //}

    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    bool get_value(int x, int z) {
        //auto [i, j] = transformar(x,z);
        //return  this->array[x][z];
        return true;
    }

    void set_occupied(int x, int z) {
        auto[i, j] = transformWtG(x, z);
        array[i][j].occupied = true;
        array[i][j].paint_cell->setBrush(QColor("Red"));
    }

    bool get_occupied(int x, int z) {
        auto[i, j] = transformWtG(x, z);
        return array[i][j].occupied;
    }
    /**
     * Transforma las coordenadas del mundo al grid
     * @param i -> coordenada x
     * @param j -> coordenada z
     * @return (X,Z) en el grid
     */
    std::optional <std::tuple<int, int>> transformWtG(int i, int j) {
        int k = i/tile + (width / tile) / 2;
        int l = j/tile + (width / tile) / 2;
            return std::make_tuple(k, l);
    }

    std::tuple<int, int> transformGtW(int k, int l){
        int i = k * tile - width / 2;
        int j = l * tile - width / 2;
        return std::make_tuple(i, j);
    }

};


#endif //GOTOXY_GRID_H
