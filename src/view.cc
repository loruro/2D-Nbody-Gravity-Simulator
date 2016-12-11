/**
  ******************************************************************************
  * @file    view.cc
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   View class.
  ******************************************************************************
  * @attention
  *
  * This file is part of 2D N-Body Gravity Simulator.
  *
  * 2D N-Body Gravity Simulator is free software: you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * 2D N-Body Gravity Simulator is distributed in the hope that it will be
  * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with 2D N-Body Gravity Simulator.
  * If not, see <http://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#include "view.h"

View::View(QWidget *parent)
  : QGraphicsView(parent),
    zoom_slider_(NULL) {}

void View::SetZoomSlider(QSlider *zoom_slider) {
  zoom_slider_ = zoom_slider;
}

void View::wheelEvent(QWheelEvent *event) {
  zoom_slider_->setValue(zoom_slider_->value() + event->angleDelta().y() / 12);
}
