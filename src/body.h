/**
  ******************************************************************************
  * @file    body.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Header file of Body class.
  ******************************************************************************
  * @attention
  *
  * This file is part of 2D N-Body Gravity Simulator.
  *
  * 2D N-Body Gravity Simulator is free software: you can redistribute it and/or
  * modify it under terms of GNU General Public License as published by
  * Free Software Foundation, either version 3 of License, or
  * (at your option) any later version.
  *
  * 2D N-Body Gravity Simulator is distributed in hope that it will be
  * useful, but WITHOUT ANY WARRANTY; without even implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
  * GNU General Public License for more details.
  *
  * You should have received a copy of GNU General Public License
  * along with 2D N-Body Gravity Simulator.
  * If not, see <http://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#ifndef BODY_H
#define BODY_H

#include <QGraphicsEllipseItem>

/**
  * @brief Planet-like object floating in space.
  */
class Body : public QGraphicsEllipseItem {
 public:
  // Number of lines which together create trail behind Body.
  static constexpr int kTrailCount = 25;

  /**
    * @brief Body constructor.
    * @param mass Mass of new Body.
    * @param radius Radius of new Body.
    * @param vel Velocity of new Body.
    * @param pos Position of new Body.
    */
  Body(qreal mass, qreal radius, QPointF vel, QPointF pos);

  /**
    * @brief Body constructor.
    * @param mass Mass of new Body.
    * @param radius Radius of new Body.
    * @param vel_x X component of velocity of new Body.
    * @param vel_y Y component of velocity of new Body.
    * @param pos_x X component of position of new Body.
    * @param pos_y Y component of position of new Body.
    */
  Body(qreal mass, qreal radius, qreal vel_x, qreal vel_y,
       qreal pos_x, qreal pos_y)
      : Body(mass, radius, QPointF(vel_x, vel_y), QPointF(pos_x, pos_y)) {}

  /**
    * @brief Body destructor.
    */
  ~Body();

  /**
    * @brief  Radius accessor.
    * @retval Radius of Body.
    */
  qreal GetRadius() const;

  /**
    * @brief Radius mutator.
    * @param radius New radius of Body.
    */
  void SetRadius(qreal radius);

  /**
    * @brief  Mass accessor.
    * @retval Mass of Body.
    */
  qreal GetMass() const;

  /**
    * @brief Mass mutator.
    * @param mass New mass of Body.
    */
  void SetMass(qreal mass);

  /**
    * @brief  Velocity accessor.
    * @retval Velocity of Body.
    */
  QPointF GetVelocity() const;

  /**
    * @brief Velocity mutator.
    * @param velocity New velocity of Body.
    */
  void SetVelocity(QPointF velocity);

  /**
    * @brief Velocity mutator.
    * @param vel_x X component of new velocity of Body.
    * @param vel_y Y component of new velocity of Body.
    */
  void SetVelocity(qreal vel_x, qreal vel_y);

  /**
    * @brief Creates trails behind Body.
    * @param view_scale Current zoom of View.
    */
  void CreateTrails(qreal view_scale);

  /**
    * @brief Removes trails behind Body.
    */
  void DeleteTrails();

  // List of Bodies which are colliding with this Body.
  QList<Body*> colliding_with_;
  QList<QGraphicsLineItem*> trails_;
  int trail_iterator_;
  QPointF last_position_;
  // Increments used in Runge-Kutta method.
  QPointF k1dx_;
  QPointF k1dv_;
  QPointF k2dx_;
  QPointF k2dv_;
  QPointF k3dx_;
  QPointF k3dv_;
  QPointF k4dx_;
  QPointF k4dv_;

 private:
  qreal radius_;
  qreal mass_;
  QPointF velocity_;
};

#endif // BODY_H
