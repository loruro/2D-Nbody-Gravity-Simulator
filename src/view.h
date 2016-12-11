/**
  ******************************************************************************
  * @file    view.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Header file of View class.
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

#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QSlider>
#include <QWheelEvent>

/**
  * @brief Object inherited from QGraphicsView just for capturing
  *        mouse wheel event.
  */
class View : public QGraphicsView {
  Q_OBJECT

 public:
  /**
    * @brief View constructor.
    * @param parent Parent of object.
    */
  View(QWidget *parent);

  /**
    * @brief Zoom slider mutator.
    * @param zoom_slider Slider used for zooming.
    */
  void SetZoomSlider(QSlider *zoom_slider);

 protected:
  /**
    * @brief Updates position of zoom slider after mouse wheel event.
    * @param event Event handler.
    */
  void wheelEvent(QWheelEvent *event);

 private:
  QSlider *zoom_slider_;
};

#endif // VIEW_H
