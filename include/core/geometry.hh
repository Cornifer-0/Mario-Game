/**
 * @file geometry.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef GEOMETRY_HH
#define GEOMETRY_HH

namespace pro2 {

struct Pt {
    int x = 0, y = 0;
};

/**
 * @brief Compara dos punts del pla
 *
 * La comparació és necessària per poder fer servir `Pt` com la clau d'un `map`.
 * La comparació utilitza primer la coordenada `x` (com si fos més "important"),
 * i, quan les `x`s són iguals, la coordenada `y`.
 */
inline bool operator<(const Pt& a, const Pt& b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
}

inline bool operator==(const Pt& a, const Pt& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Pt& a, const Pt& b) {
    return !(a == b);
}

inline Pt operator+(const Pt& a, const Pt& b) {
    return {a.x + b.x, a.y + b.y};
}

inline Pt operator-(const Pt& a, const Pt& b) {
    return {a.x - b.x, a.y - b.y};
}

inline Pt operator*(const Pt& a, const int& k) {
    return {a.x * k, a.y * k};
}

inline Pt operator/(const Pt& a, const int& k) {
    return {a.x / k, a.y / k};
}

struct Rect {
    int left, top, right, bottom;
};

inline Rect operator+(const Rect& a, const int& k) {
    return {a.left - k, a.top - k, a.right + k, a.bottom + k};
}

inline Rect operator-(const Rect& a, const int& k) {
    return {a.left + k, a.top + k, a.right - k, a.bottom - k};
}

}  // namespace pro2

#endif