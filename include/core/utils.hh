/**
 * @file utils.hh
 * @brief
 * @version 0.1
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef UTILS_HH
#define UTILS_HH

#include <vector>
#include "core/geometry.hh"
#include "graphics/window.hh"

namespace pro2 {

struct ControlsSetUp {
    int jump_, sprint_, left_, right_, crouch_, dance_;
};

const ControlsSetUp defaultPlayerOneControls = {Keys::Space, Keys::Up,   Keys::Left,
                                                Keys::Right, Keys::Down, Keys::B};

const ControlsSetUp defaultPlayerTwoControls = {Keys::W, Keys::V, Keys::A,
                                                Keys::D, Keys::S, Keys::Q};

/**
 * @brief Dibuja una línea horizontal en la ventana.
 *
 * @param window Ventana en la que se dibuja la línea.
 * @param xini Coordenada x inicial.
 * @param xfin Coordenada x final.
 * @param y Coordenada y.
 * @param color Color de la línea (opcional, si no se pone se asume `white`).
 */
void paint_hline(pro2::Window& window, int xini, int xfin, int y, pro2::Color color = pro2::white);

/**
 * @brief Dibuja una línea vertical en la ventana.
 *
 * @param window Ventana en la que se dibuja la línea.
 * @param x Coordenada x.
 * @param yini Coordenada y inicial.
 * @param yfin Coordenada y final.
 * @param color Color de la línea (opcional, si no se pone se asume `white`).
 */
void paint_vline(pro2::Window& window, int x, int yini, int yfin, pro2::Color color = pro2::white);

/**
 * @brief Dibuixa una imatge/textura a la finestra a partir d'una posició
 *
 * @param window Finestra a la que pintar
 * @param orig   Origen (cantonada de dalt a l'esquerra) del rectangle que forma el _sprite_
 * @param sprite Matriu de colors que representa la imatge (_sprite_).
 * @param mirror Si cal pintar girar la textura horitzontalment
 */
void paint_sprite(pro2::Window&                        window,
                  pro2::Pt                             orig,
                  const std::vector<std::vector<int>>& sprite,
                  bool                                 mirror,
                  float                                scale = 1.0f);

/**
 * @brief Paints the background sprite to the window, with a parallax effect given by the
 * parralaxMuliplier. From a certain height, it starts drawing the sky (all black by default).
 *
 * @param window
 * @param sprite
 * @param parallaxMultiplier
 * @param sky
 */
void paint_background_sprite(pro2::Window&                        window,
                             const std::vector<std::vector<int>>& sprite,
                             double                               parallaxMultiplier,
                             const std::vector<std::vector<int>>& sky = {{0}});

/**
 * @brief Dibuixa una imatge/textura a la finextra a partir d'una posicio (aquesta es el centre)
 *
 * @param window   Finestra a la que pintar
 * @param orig     Origen (centre) del rectangle que forma el _sprite_
 * @param sprite   Matriu de colors que representa la imatge (_sprite_).
 * @param mirrorSi cal pintar girar la textura horitzontalment
 * @param scae     Escala
 */
void paint_sprite_center(pro2::Window&                        window,
                         pro2::Pt                             orig,
                         const std::vector<std::vector<int>>& sprite,
                         bool                                 mirror,
                         float                                scae = 1.0f);

/**
 * @brief Dibuixa una xifra a la pantalla.
 *
 * @param window Finestra a la que pintar
 * @param orig   Origen (cantonada de dalt a l'esquerra) del rectangle que forma el _sprite_
 * @param number Xifra que pintar entre 0 i 9
 */
void paint_number(pro2::Window& window, pro2::Pt orig, int number);

/**
 * @brief Paints every pixel outside the circle (formed by center and radius parameters) black.
 *
 * @param window
 * @param radius
 * @param center
 */
void paint_end_circle(Window& window, int radius, Pt center);

}  // namespace pro2

#endif