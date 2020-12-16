//
// Created by salabeta on 24/11/20.
//

#ifndef GOTOXY_GRID_H
#define GOTOXY_GRID_H

#include <QGraphicsItem>

template<typename HMIN, HMIN hmin, typename WIDTH, WIDTH width, typename TILE, TILE tile>
class Grid {
public:
    Grid() {
        array.resize((int) (width / tile));
        for (auto &row : array)
            row.resize((int) (width / tile));
        int k = 0;
        for (int i = hmin; i < width / 2; i += tile, k++) {
            int l = 0;
            for (int j = hmin; j < width / 2; j += tile, l++) {
                array[k][l] = Value{false, nullptr, i, j};
            }
        }
    };

    struct Value {
        bool occupied = false;
        QGraphicsRectItem *paint_cell = nullptr;
        QGraphicsTextItem *text_cell = nullptr;
        int cx, cy;
        int dist = 0; //dist vecinos
    };

    std::vector<std::vector<Value>> array;

    void create_graphic_items(QGraphicsScene &scene, QGraphicsView *view) {
        auto fondo = QColor("LightGreen");
        fondo.setAlpha(40);
        QFont font("Bavaria");
        font.setPointSize(40);
        font.setWeight(QFont::TypeWriter);
        for (auto &row : array)
            for (auto &elem : row) {
                elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")),
                                                QBrush(fondo));
                elem.paint_cell->setPos(elem.cx, elem.cy);
                elem.text_cell = scene.addText("-1", font);
                elem.text_cell->setPos(elem.cx - tile / 2, elem.cy - tile / 2);
                // Get the current transform
                QTransform transform(elem.text_cell->transform());
                qreal m11 = transform.m11();    // Horizontal scaling
                qreal m12 = transform.m12();    // Vertical shearing
                qreal m13 = transform.m13();    // Horizontal Projection
                qreal m21 = transform.m21();    // Horizontal shearing
                qreal m22 = transform.m22();    // vertical scaling
                qreal m23 = transform.m23();    // Vertical Projection
                qreal m31 = transform.m31();    // Horizontal Position (DX)
                qreal m32 = transform.m32();    // Vertical Position (DY)
                qreal m33 = transform.m33();    // Addtional Projection Factor
                // Vertical flip
                m22 = -m22;
                // Write back to the matrix
                transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                // Set the items transformation
                elem.text_cell->setTransform(transform);
            }
    }


    /*
 * Inicializamos el array a false, osea, no ocupades.
 */
    void inicializate() {
        for (int i = 0; i < this->tam; ++i) {
            for (int j = 0; j < this->tam; ++j) {
                this->array[i][j] = false;
            }
        }
    }

    void compute_navigation_function(int x, int z) {
        auto target = transformGtW(x, z);
        reset_cell_distances();
        int dist = 0;
        Eigen::Vector2f tw = target.value();
        std::vector<Value> L1 = neighboors(get_value(tw.x(), tw.z()), dist);
        std::vector<Value> L2 = {};
        bool end = false;
        while (!end) {
            for (auto current_cell : L1) {
                auto selected = neighboors(current_cell, dist);
                L2.insert(selected);
            }
            dist++;
            end = L2.empty();
            L1.swap(L2);
            L2.clear();
        }
    }


    std::vector<Value> neighboors(Value v, int dist) {
        std::vector<std::tuple<int, int>> lista_coor_de_vecinos{{-1, -1},
                                                                {0,  -1},
                                                                {1,  -1},
                                                                {-1, 0},
                                                                {1,  0},
                                                                {-1, 1},
                                                                {0,  1},
                                                                {-1, 1}};

        std::vector<Value> lista_de_valores;
        for (auto[dk, dl] : lista_coor_de_vecinos) {
            int k = v.cx + dk;        // OJO hay que añadir al struct Value las coordenadas de array
            int l = v.cy + dl;
            if (array[k][l].dist != -1) {
                set_value(k, l, dist);
                lista_de_valores.insert(array[k][l]);
            }
        }
        return lista_de_valores;
    }

    void reset_cell_distances() {
        for (auto &row : this->array) {
            for (auto &elem : row)
                elem.dist = -1;
        }
    }

public:

    /**
     * devolvemos el valor de la coordenada x,z
     * @param x
     * @param z
     * @return
     */
    Value get_value(int x, int z) {
        auto[i, j] = transformWtG(x, z);
        return this->array[x][z];
    }

    /**
     * modificamos en funcion de v la coordenada x,z
     * @param x
     * @param y
     * @param val
     */
    void set_value(int x, int y, int val) {
        array[x][y].dist = val;
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
    std::optional<std::tuple<int, int>> transformWtG(int i, int j) {
        int k = i / tile + (width / tile) / 2;
        int l = j / tile + (width / tile) / 2;
        return std::make_tuple(k, l);
    }

    std::tuple<int, int> transformGtW(int k, int l) {
        int i = k * tile - width / 2;
        int j = l * tile - width / 2;
        return std::make_tuple(i, j);
    }

    void create_graphic_items(QGraphicsScene &scene, QGraphicsView *view) {
        auto fondo = QColor("LightGreen");
        fondo.setAlpha(40);
        QFont font("Bavaria");
        font.setPointSize(40);
        font.setWeight(QFont::TypeWriter);
        for (auto &row : array)
            for (auto &elem : row) {
                elem.paint_cell = scene.addRect(-tile / 2, -tile / 2, tile, tile, QPen(QColor("DarkGreen")),
                                                QBrush(fondo));
                elem.paint_cell->setPos(elem.cx, elem.cy);
                elem.text_cell = scene.addText("-1", font);
                elem.text_cell->setPos(elem.cx - tile / 2, elem.cy - tile / 2);
                // Get the current transform
                QTransform transform(elem.text_cell->transform());
                qreal m11 = transform.m11();    // Horizontal scaling
                qreal m12 = transform.m12();    // Vertical shearing
                qreal m13 = transform.m13();    // Horizontal Projection
                qreal m21 = transform.m21();    // Horizontal shearing
                qreal m22 = transform.m22();    // vertical scaling
                qreal m23 = transform.m23();    // Vertical Projection
                qreal m31 = transform.m31();    // Horizontal Position (DX)
                qreal m32 = transform.m32();    // Vertical Position (DY)
                qreal m33 = transform.m33();    // Addtional Projection Factor
                // Vertical flip
                m22 = -m22;
                // Write back to the matrix
                transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                // Set the items transformation
                elem.text_cell->setTransform(transform);
            }
    }


};


#endif //GOTOXY_GRID_H
