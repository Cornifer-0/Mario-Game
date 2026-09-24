/**
 * @file finder.hh
 * @brief
 * @version 0.1
 * @date 2025-05-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FINDER_HH
#define FINDER_HH

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include "geometry.hh"

/**
 * @brief La class Finder et permet guardar elements T associats a un regtangle en un espai 2d. T ha
 * de tenir el metode get_rect(). Finder et permet rapidament comprobar quins objectes
 * interseccionen amb un rectangle donat.
 * @tparam T
 */
template <typename T>
class Finder {
    struct QuadNode {
        std::vector<T *> items_;

        pro2::Rect rec_;

        QuadNode *parent_;

        QuadNode *top_left_child_;
        QuadNode *top_right_child_;
        QuadNode *bottom_left_child_;
        QuadNode *bottom_right_child_;

        QuadNode()
            : rec_({-1000, -1000, 20000, 20000}),
              parent_(nullptr),
              top_left_child_(nullptr),
              top_right_child_(nullptr),
              bottom_left_child_(nullptr),
              bottom_right_child_(nullptr) {}

        QuadNode(pro2::Rect rec)
            : rec_(rec),
              parent_(nullptr),
              top_left_child_(nullptr),
              top_right_child_(nullptr),
              bottom_left_child_(nullptr),
              bottom_right_child_(nullptr) {}

        QuadNode(pro2::Rect rec, QuadNode *parent)
            : rec_(rec),
              parent_(parent),
              top_left_child_(nullptr),
              top_right_child_(nullptr),
              bottom_left_child_(nullptr),
              bottom_right_child_(nullptr) {}
    };

    static const int MAX_OCCUPACY = 8;
    static const int MIN_NODE_SIZE = 20;  // 20x20 min node size

    QuadNode                           *root_;
    std::unordered_map<T *, pro2::Rect> item_rects_;

 public:
    Finder() : root_(new QuadNode()) {}

    ~Finder() {
        clearTree(root_);
    }

    /**
     * @brief Afegeix t al contenidor del Finder.
     *
     * @param t
     */
    void add(T *t) {
        item_rects_[t] = t->get_rect();
        add(root_, t);
    }

    /**
     * @brief Actualitza el rectangle associat a t dins del Finder.
     *
     * @param t
     */
    void update(T *t) {
        if (item_rects_.find(t) != item_rects_.end()) {
            pro2::Rect old_rect = item_rects_[t];
            remove_with_rect(t, old_rect);
        }

        item_rects_[t] = t->get_rect();
        add(t);
    }

    /**
     * @brief Afegeix t al Finder
     *
     * @param t
     */
    void remove(T *t) {
        if (item_rects_.find(t) != item_rects_.end()) {
            pro2::Rect old_rect = item_rects_[t];
            remove_with_rect(t, old_rect);
            item_rects_.erase(t);
        }
    }

    /**
     * @brief Retorna el conjunt d'objectes amb rectangles
     *        total o parcialment dins de `rect`.
     *
     * Si el nombre de rectangles del contenidor és `n`, el
     * cost de l'algorisme ha de ser O(log n).
     *
     * @param rect El rectangle de cerca
     *
     * @returns Un conjunt de punters a objectes que tenen un
     *          rectangle parcial o totalment dins de `rect`
     */
    std::set<T *> query(pro2::Rect rect) const {
        std::set<T *> S;
        query_(root_, rect, S);
        return S;
    }

 private:
    /**
     * @brief Funcio recursiva per buscar les fulles de l'arbre que interseccionen amb el rectangle
     * de t.
     *
     * @param qn
     * @param t
     */
    void add(QuadNode *qn, T *t) {
        if (qn->top_left_child_ != nullptr) {
            if (intersect(t->get_rect(), qn->top_left_child_->rec_)) {
                add(qn->top_left_child_, t);
            }
            if (intersect(t->get_rect(), qn->top_right_child_->rec_)) {
                add(qn->top_right_child_, t);
            }
            if (intersect(t->get_rect(), qn->bottom_left_child_->rec_)) {
                add(qn->bottom_left_child_, t);
            }
            if (intersect(t->get_rect(), qn->bottom_right_child_->rec_)) {
                add(qn->bottom_right_child_, t);
            }
        } else {
            qn->items_.push_back(t);
            updateNode(qn);
        }
    }

    /**
     * @brief Comproba si el node s'ha de dividir.
     *
     * @param qn
     */
    void updateNode(QuadNode *qn) {
        if (qn->items_.size() >= MAX_OCCUPACY) {
            if (qn->rec_.right - qn->rec_.left >= MIN_NODE_SIZE) {
                divide(qn);
            }
        }
    }

    /**
     * @brief Retorna true si i nomes si els rectangles a i b interseccionen en el pla.
     *
     * @param a
     * @param b
     * @return true
     * @return false
     */
    bool intersect(const pro2::Rect& a, const pro2::Rect& b) const {
        return !(a.right < b.left || a.left > b.right || a.bottom < b.top || a.top > b.bottom);
    }

    /**
     * @brief Divideix el quadNode qn, es a dir, crea 4 fills a aquest node i reperteix als fills
     * els objectes que hi havia en aquest node. Els rects dels fills corresponen als quadrants del
     * rect del pare (top_left, top_right, bottom_left, bottom_right).
     *
     * @param qn
     */
    void divide(QuadNode *qn) {
        const int width = qn->rec_.right - qn->rec_.left;
        const int height = qn->rec_.bottom - qn->rec_.top;

        pro2::Rect tl = {qn->rec_.left, qn->rec_.top, qn->rec_.right - width / 2,
                         qn->rec_.bottom - height / 2};
        pro2::Rect tr = {qn->rec_.left + width / 2, qn->rec_.top, qn->rec_.right,
                         qn->rec_.bottom - height / 2};
        pro2::Rect bl = {qn->rec_.left, qn->rec_.top + height / 2, qn->rec_.right - width / 2,
                         qn->rec_.bottom};
        pro2::Rect br = {qn->rec_.left + width / 2, qn->rec_.top + height / 2, qn->rec_.right,
                         qn->rec_.bottom};

        qn->top_left_child_ = new QuadNode(tl, qn);
        qn->top_right_child_ = new QuadNode(tr, qn);
        qn->bottom_left_child_ = new QuadNode(bl, qn);
        qn->bottom_right_child_ = new QuadNode(br, qn);

        for (T *item : qn->items_) {
            if (intersect(item->get_rect(), tl)) {
                add(qn->top_left_child_, item);
            }
            if (intersect(item->get_rect(), tr)) {
                add(qn->top_right_child_, item);
            }
            if (intersect(item->get_rect(), bl)) {
                add(qn->bottom_left_child_, item);
            }
            if (intersect(item->get_rect(), br)) {
                add(qn->bottom_right_child_, item);
            }
        }
        qn->items_.clear();
    }

    /**
     * @brief Eliminar un objecte que ha estat guardat amb old_rect.
     *
     * @param t
     * @param old_rect
     */
    void remove_with_rect(T *t, pro2::Rect old_rect) {
        remove_(root_, t, old_rect);
    }

    /**
     * @brief Funcio recursiva per buscar on esta guardat t, i eliminar-lo.
     *
     * @param qn
     * @param t
     * @param old_rect
     */
    void remove_(QuadNode *qn, T *t, const pro2::Rect old_rect) {
        if (qn == nullptr) {
            return;
        }

        if (!qn->items_.empty()) {
            qn->items_.erase(std::remove(qn->items_.begin(), qn->items_.end(), t),
                             qn->items_.end());
            // merge(qn->parent_);
            return;
        }

        if (qn->top_left_child_ != nullptr && intersect(old_rect, qn->top_left_child_->rec_)) {
            remove_(qn->top_left_child_, t, old_rect);
        }
        if (qn->top_right_child_ != nullptr && intersect(old_rect, qn->top_right_child_->rec_)) {
            remove_(qn->top_right_child_, t, old_rect);
        }
        if (qn->bottom_left_child_ != nullptr &&
            intersect(old_rect, qn->bottom_left_child_->rec_)) {
            remove_(qn->bottom_left_child_, t, old_rect);
        }
        if (qn->bottom_right_child_ != nullptr &&
            intersect(old_rect, qn->bottom_right_child_->rec_)) {
            remove_(qn->bottom_right_child_, t, old_rect);
        }
    }

    /**
     * @brief La funcio afageix els objecte de items a S, si i nomes si interseccionen amb rect.
     *
     * @param items
     * @param rect
     * @param S
     */
    void add_to_set_if_intersects(const std::vector<T *> items,
                                  pro2::Rect             rect,
                                  std::set<T *>&         S) const {
        // std::cout << "look" << std::endl;
        for (auto item : items) {
            if (intersect(item->get_rect(), rect)) {
                S.insert(item);
            }
        }
        // std::cout << "done this one " << std::endl;
    }

    /**
     * @brief Query recursiva per buscar un objecte dins de l'arbre de QuadNode's.
     *
     * @param qn
     * @param rect
     * @param S
     */
    void query_(QuadNode *qn, pro2::Rect rect, std::set<T *>& S) const {
        // std::cout << "how many times" << std::endl;
        if (qn == nullptr) {
            return;
        }

        if (qn->items_.size() > 0) {
            // cout << qn->items_.size() << endl;
            add_to_set_if_intersects(qn->items_, rect, S);
        }
        // std::cout << "1" << std::endl;

        if (qn->top_left_child_ != nullptr && intersect(rect, qn->top_left_child_->rec_)) {
            // std::cout << "2" << std::endl;
            query_(qn->top_left_child_, rect, S);
        }
        if (qn->top_right_child_ != nullptr && intersect(rect, qn->top_right_child_->rec_)) {
            // std::cout << "3 " << std::endl;
            query_(qn->top_right_child_, rect, S);
        }
        if (qn->bottom_left_child_ != nullptr && intersect(rect, qn->bottom_left_child_->rec_)) {
            // std::cout << "4 " << std::endl;
            query_(qn->bottom_left_child_, rect, S);
        }
        if (qn->bottom_right_child_ != nullptr && intersect(rect, qn->bottom_right_child_->rec_)) {
            // std::cout << "5 ? " << std::endl;
            query_(qn->bottom_right_child_, rect, S);
        }
        // std::cout << "6 " << std::endl;
    }

    /**
     * @brief Funcio recursiva per borrar l'arbre sencer.
     *
     * @param node
     */
    void clearTree(QuadNode *node) {
        if (!node) {
            return;
        }
        clearTree(node->top_left_child_);
        clearTree(node->top_right_child_);
        clearTree(node->bottom_left_child_);
        clearTree(node->bottom_right_child_);
        delete node;
    }
};

#endif