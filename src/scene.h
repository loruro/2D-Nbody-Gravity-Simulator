/**
  ******************************************************************************
  * @file    scene.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Header file of Scene class.
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

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>

#include "body.h"

/**
  * @brief Object that manages graphical objects on screen (e.g. Bodies and
  *        their trails).
  */
class Scene : public QGraphicsScene {
  Q_OBJECT

 public:
  // Function under mouse cursor.
  enum ToolType {
    kCreate,
    kDelete,
    kNone
  };

  static constexpr float kGravConstant = 6673.85;

  /**
    * @brief Scene constructor.
    * @param parent Parent of object.
    * @param view_scale Current zoom of View.
    */
  Scene(QObject *parent = 0, qreal view_scale = 1);

  /**
    * @brief Scene destructor.
    */
  ~Scene();

  /**
    * @brief  Mass accessor.
    * @retval Mass used for creating new Body.
    */
  qreal GetMass() const;

  /**
    * @brief Mass mutator.
    * @param mass Mass used for creating new Body.
    */
  void SetMass(qreal mass);

  /**
    * @brief  Radius accessor.
    * @retval Radius used for creating new Body.
    */
  qreal GetRadius() const;

  /**
    * @brief Radius mutator.
    * @param radius Radius used for creating new Body.
    */
  void SetRadius(qreal radius);

  /**
    * @brief  Density accessor.
    * @retval Density used for creating new Body.
    */
  qreal GetDensity() const;

  /**
    * @brief Density mutator.
    * @param density Density used for creating new Body.
    */
  void SetDensity(qreal density);

  /**
    * @brief Tool mutator.
    * @param tool Function under mouse cursor.
    */
  void SetTool(ToolType tool);

  /**
    * @brief Time step mutator.
    * @param time_step New time step.
    */
  void SetTimeStep(qreal time_step);

  // Timer for advancing Scene in equal time intervals.
  QTimer *advancing_timer_;
  // Line used during creation of new Body. Visualizes its velocity.
  QGraphicsLineItem *creation_line_;
  QList<Body*> body_list_;
  // Are trails activated?
  bool trails_;
  // Euler or Runge-Kutta method?
  bool runge_kutta_;
  // Current zoom of View.
  qreal view_scale_;

 protected:
  /**
    * @brief Starts creating new Body or removes existing one.
    *        Depends on current tool.
    * @param event Event handler.
    */
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

  /**
    * @brief Ends creating new Body.
    * @param event Event handler.
    */
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  /**
    * @brief Updates creation line during Body creation.
    * @param event Event handler.
    */
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

 private:
  /**
    * @brief Updates velocity and position of Bodies using Euler method.
    */
  void AdvanceEuler();

  /**
    * @brief Updates velocity and position of Bodies using Runge-Kutta method.
    */
  void AdvanceRungeKutta();

  /**
    * @brief Find collision between Bodies.
    * @param body_1 Body 1.
    * @param body_2 Body 2.
    * @param distance Distance between Bodies.
    */
  void FindCollisions(Body* body_1, Body* body_2, qreal distance);

  /**
    * @brief Finds all Bodies colliding with each other,
    *        creating local collision group.
    * @param body Reference Body.
    */
  void CollidingGroupSearch(Body *body);

  /**
    * @brief Merges Bodies together if collision occurred.
    */
  void ResolveCollisions();

  /**
    * @brief Updates trails of Body.
    * @param body Body.
    */
  void AdvanceTrails(Body *body);

  // Parameters used for creating new Body.
  qreal new_mass_;
  qreal new_density_;
  qreal new_radius_;
  // Current function under mouse cursor.
  ToolType tool_;
  // Position of new Body. Used during its creation.
  QPointF last_cursor_pos_;
  // All Bodies which are currently colliding.
  QList<Body*> collision_list_;
  // Bodies which are currently colliding with each other in local group.
  QList<Body*> local_collision_list_;
  // Time step used in calculations of positon and velocity of Bodies.
  qreal time_step_;

 private slots:
  /**
    * @brief Updates velocity and position of Bodies.
    */
  void Advance();
};

#endif // SCENE_H
