/**
  ******************************************************************************
  * @file    scene.cc
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Scene class.
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

#include "scene.h"

#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent, qreal view_scale)
    : QGraphicsScene(parent),
      trails_(false),
      runge_kutta_(false),
      view_scale_(view_scale),
      new_mass_(1.0),
      new_density_(1.0),
      new_radius_(1.0),
      tool_(kNone),
      time_step_(1.0) {
  setBackgroundBrush(Qt::black);
  setItemIndexMethod(QGraphicsScene::NoIndex);
  // Temporary object used to stretch Scene.
  addItem(new QGraphicsEllipseItem(-1000000, -1000000, 2000000, 2000000));

  creation_line_ = new QGraphicsLineItem();
  addItem(creation_line_);
  creation_line_->setPen(QPen(QColor(255, 255, 255)));
  // Line is visible only during creation of Body.
  creation_line_->setVisible(false);
  creation_line_->setZValue(2);

  advancing_timer_ = new QTimer(this);
  connect(advancing_timer_, SIGNAL(timeout()), this, SLOT(Advance()));
  advancing_timer_->start(10);
}

Scene::~Scene() {
  delete advancing_timer_;
  delete creation_line_;
  foreach (Body *body, body_list_) {
    body->DeleteTrails();
    removeItem(body);
    delete body;
  }
}

qreal Scene::GetMass() const {
  return new_mass_;
}

void Scene::SetMass(qreal mass) {
  new_mass_ = mass;
}

qreal Scene::GetRadius() const {
  return new_radius_;
}

void Scene::SetRadius(qreal radius) {
  new_radius_ = radius;
}

qreal Scene::GetDensity() const {
  return new_density_;
}

void Scene::SetDensity(qreal density) {
  new_density_ = density;
}

void Scene::SetTool(ToolType tool) {
  tool_ = tool;
}

void Scene::SetTimeStep(qreal time_step) {
  time_step_ = time_step;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (tool_ == kCreate) {
    // Starts drawing line from position of mouse press event.
    last_cursor_pos_ = event->scenePos();
    creation_line_->setVisible(true);
    creation_line_->setLine(last_cursor_pos_.x(), last_cursor_pos_.y(),
                            last_cursor_pos_.x(), last_cursor_pos_.y());
  } else if (tool_ == kDelete) {
    QGraphicsItem *item_under_cursor = itemAt(event->scenePos(), QTransform());
    if (item_under_cursor != 0) {
      Body *body = dynamic_cast<Body*>(item_under_cursor);
      if (body != NULL) {
        body->DeleteTrails();
        removeItem(body);
        body_list_.removeOne(body);
        delete body;
      }
    }
  }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (tool_ == kCreate) {
    creation_line_->setVisible(false);
    QPointF new_velocity = event->scenePos() - last_cursor_pos_;
    Body *body = new Body(new_mass_, new_radius_,
                          new_velocity, last_cursor_pos_);
    this->addItem(body);
    body_list_.append(body);
    if (trails_)
      body->CreateTrails(view_scale_);
  }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  // Updates other end of creation line to current position of cursor.
  if (tool_ == kCreate) {
    creation_line_->setLine(last_cursor_pos_.x(), last_cursor_pos_.y(),
                            event->scenePos().x(), event->scenePos().y());
  }
}

void Scene::AdvanceEuler() {
  int i = 0;
  foreach (Body* body_1, body_list_.mid(0, body_list_.count() - 1)) {
    foreach (Body* body_2, body_list_.mid(1 + i, body_list_.count() - 1 - i)) {
      qreal delta_x = body_2->pos().x() - body_1->pos().x();
      qreal delta_y = body_2->pos().y() - body_1->pos().y();
      qreal distance = sqrt(delta_x * delta_x + delta_y * delta_y);
      // Eliminates crazy velocities when body was spawned inside another one.
      if (distance > 0.03) {
        // Distance is to the power of -3 instead -2, because acceleration
        // would have to be divided by distance in next lines anyway.
        qreal acceleration = kGravConstant * time_step_ * pow(distance, -3.0);
        qreal acceleration_x = acceleration * delta_x;
        qreal acceleration_y = acceleration * delta_y;
        qreal force_1_x = acceleration_x * body_2->GetMass();
        qreal force_1_y = acceleration_y * body_2->GetMass();
        qreal force_2_x = acceleration_x * body_1->GetMass();
        qreal force_2_y = acceleration_y * body_1->GetMass();
        body_1->SetVelocity(body_1->GetVelocity().x() + force_1_x,
                            body_1->GetVelocity().y() + force_1_y);
        body_2->SetVelocity(body_2->GetVelocity().x() - force_2_x,
                            body_2->GetVelocity().y() - force_2_y);
      }
      FindCollisions(body_1, body_2, distance);
    }
    ++i;
  }

  foreach (Body* body, body_list_) {
    body->last_position_ = body->pos();
    body->setPos(body->pos() + body->GetVelocity() * time_step_);
    if (trails_)
      AdvanceTrails(body);
  }
}

void Scene::AdvanceRungeKutta() {
  Body *body = body_list_.last();
  int i = 0;
  foreach (Body* body_1, body_list_.mid(0, body_list_.count() - 1)) {
    body_1->k1dx_ = body_1->GetVelocity() * time_step_;
    foreach (Body* body_2, body_list_.mid(1 + i, body_list_.count() - 1 - i)) {
      qreal delta_x = body_2->pos().x() - body_1->pos().x();
      qreal delta_y = body_2->pos().y() - body_1->pos().y();
      qreal distance = sqrt(delta_x * delta_x + delta_y * delta_y);
      // Eliminates crazy velocities when body was spawned inside another one.
      if (distance > 0.03) {
        // Distance is to the power of -3 instead -2, because acceleration
        // would have to be divided by distance in next lines anyway.
        qreal acceleration = kGravConstant * time_step_ * pow(distance, -3.0);
        qreal acceleration_x = acceleration * delta_x;
        qreal acceleration_y = acceleration * delta_y;
        body_1->k1dv_ += QPointF(acceleration_x * body_2->GetMass(),
                                 acceleration_y * body_2->GetMass());
        body_2->k1dv_ += QPointF(-acceleration_x * body_1->GetMass(),
                                 -acceleration_y * body_1->GetMass());
      }
    }
    ++i;
  }
  body->k1dx_ = body->GetVelocity() * time_step_;

  i = 0;
  foreach (Body* body_1, body_list_.mid(0, body_list_.count() - 1)) {
    body_1->k2dx_ = time_step_ * (body_1->GetVelocity() + body_1->k1dv_ * 0.5);
    foreach (Body* body_2, body_list_.mid(1 + i, body_list_.count() - 1 - i)) {
      qreal pos_1_x = body_1->pos().x() + body_1->k1dx_.x() * 0.5;
      qreal pos_1_y = body_1->pos().y() + body_1->k1dx_.y() * 0.5;
      qreal pos_2_x = body_2->pos().x() + body_2->k1dx_.x() * 0.5;
      qreal pos_2_y = body_2->pos().y() + body_2->k1dx_.y() * 0.5;
      qreal delta_x = pos_2_x - pos_1_x;
      qreal delta_y = pos_2_y - pos_1_y;
      qreal distance = sqrt(delta_x * delta_x + delta_y * delta_y);
      // Eliminates crazy velocities when body was spawned inside another one.
      if (distance > 0.03) {
        // Distance is to the power of -3 instead -2, because acceleration
        // would have to be divided by distance in next lines anyway.
        qreal acceleration = kGravConstant * time_step_ * pow(distance, -3.0);
        qreal acceleration_x = acceleration * delta_x;
        qreal acceleration_y = acceleration * delta_y;
        body_1->k2dv_ += QPointF(acceleration_x * body_2->GetMass(),
                                 acceleration_y * body_2->GetMass());
        body_2->k2dv_ += QPointF(-acceleration_x * body_1->GetMass(),
                                 -acceleration_y * body_1->GetMass());
      }
    }
    ++i;
  }
  body->k2dx_ = time_step_ * (body->GetVelocity() + body->k1dv_ * 0.5);

  i = 0;
  foreach (Body* body_1, body_list_.mid(0, body_list_.count() - 1)) {
    body_1->k3dx_ = time_step_ * (body_1->GetVelocity() + body_1->k2dv_ * 0.5);
    foreach (Body* body_2, body_list_.mid(1 + i, body_list_.count() - 1 - i)) {
      qreal pos_1_x = body_1->pos().x() + body_1->k2dx_.x() * 0.5;
      qreal pos_1_y = body_1->pos().y() + body_1->k2dx_.y() * 0.5;
      qreal pos_2_x = body_2->pos().x() + body_2->k2dx_.x() * 0.5;
      qreal pos_2_y = body_2->pos().y() + body_2->k2dx_.y() * 0.5;
      qreal delta_x = pos_2_x - pos_1_x;
      qreal delta_y = pos_2_y - pos_1_y;
      qreal distance = sqrt(delta_x * delta_x + delta_y * delta_y);
      // Eliminates crazy velocities when body was spawned inside another one.
      if (distance > 0.03) {
        // Distance is to the power of -3 instead -2, because acceleration
        // would have to be divided by distance in next lines anyway.
        qreal acceleration = kGravConstant * time_step_ * pow(distance, -3.0);
        qreal acceleration_x = acceleration * delta_x;
        qreal acceleration_y = acceleration * delta_y;
        body_1->k3dv_ += QPointF(acceleration_x * body_2->GetMass(),
                                 acceleration_y * body_2->GetMass());
        body_2->k3dv_ += QPointF(-acceleration_x * body_1->GetMass(),
                                 -acceleration_y * body_1->GetMass());
      }
    }
    ++i;
  }
  body->k3dx_ = time_step_ * (body->GetVelocity() + body->k2dv_ * 0.5);

  i = 0;
  foreach (Body* body_1, body_list_.mid(0,body_list_.count() - 1)) {
    body_1->k4dx_ = time_step_ * (body_1->GetVelocity() + body_1->k3dv_);
    foreach (Body* body_2, body_list_.mid(1 + i, body_list_.count() - 1 - i)) {
      qreal pos_1_x = body_1->pos().x() + body_1->k3dx_.x();
      qreal pos_1_y = body_1->pos().y() + body_1->k3dx_.y();
      qreal pos_2_x = body_2->pos().x() + body_2->k3dx_.x();
      qreal pos_2_y = body_2->pos().y() + body_2->k3dx_.y();
      qreal delta_x = pos_2_x - pos_1_x;
      qreal delta_y = pos_2_y - pos_1_y;
      qreal distance = sqrt(delta_x * delta_x + delta_y * delta_y);
      // Eliminates crazy velocities when body was spawned inside another one.
      if (distance > 0.03) {
        // Distance is to the power of -3 instead -2, because acceleration
        // would have to be divided by distance in next lines anyway.
        qreal acceleration = kGravConstant * time_step_ * pow(distance, -3.0);
        qreal acceleration_x = acceleration * delta_x;
        qreal acceleration_y = acceleration * delta_y;
        body_1->k4dv_ += QPointF(acceleration_x * body_2->GetMass(),
                                 acceleration_y * body_2->GetMass());
        body_2->k4dv_ += QPointF(-acceleration_x * body_1->GetMass(),
                                 -acceleration_y * body_1->GetMass());
      }
      FindCollisions(body_1, body_2, distance);
    }
    ++i;
  }
  body->k4dx_ = time_step_ * (body->GetVelocity() + body->k3dv_);

  foreach (Body* body, body_list_) {
    body->last_position_ = body->pos();
    QPointF delta_velocity = body->k1dv_ + 2.0 * body->k2dv_;
    delta_velocity += 2.0 * body->k3dv_ + body->k4dv_;
    delta_velocity /= 6.0;
    body->SetVelocity(body->GetVelocity() + delta_velocity);
    QPointF delta_pos = body->k1dx_ + 2.0 * body->k2dx_;
    delta_pos += 2.0 * body->k3dx_ + body->k4dx_;
    delta_pos /= 6.0;
    body->setPos(body->pos() + delta_pos);
    body->k1dv_ = QPointF(0,0);
    body->k2dv_ = QPointF(0,0);
    body->k3dv_ = QPointF(0,0);
    body->k4dv_ = QPointF(0,0);
    if (trails_)
      AdvanceTrails(body);
  }
}

void Scene::FindCollisions(Body* body_1, Body* body_2, qreal distance) {
  if (distance <= body_1->GetRadius() + body_2->GetRadius()) {
    if (!collision_list_.contains(body_1))
      collision_list_.append(body_1);
    if (!collision_list_.contains(body_2))
      collision_list_.append(body_2);
    body_1->colliding_with_.append(body_2);
    body_2->colliding_with_.append(body_1);
  }
}

void Scene::CollidingGroupSearch(Body *body) {
  foreach (Body *g, body->colliding_with_) {
    if (!local_collision_list_.contains(g)) {
      local_collision_list_.append(g);
      CollidingGroupSearch(g);
    }
  }
  body->colliding_with_.clear();
}

void Scene::ResolveCollisions() {
  foreach (Body *body, collision_list_) {
    qreal group_mass_ = 0;
    qreal group_volume_ = 0;
    QPointF group_momentum_ = QPointF(0,0);
    QPointF group_mass_center_ = QPointF(0,0);
    local_collision_list_.append(body);
    CollidingGroupSearch(body);
    foreach (Body *colliding_body, local_collision_list_) {
      qreal colliding_mass = colliding_body->GetMass();
      // If some Bodies collide together, resulting Body should preserve mass,
      // volume and momentum.
      group_mass_ += colliding_mass;
      group_volume_ += pow(colliding_body->GetRadius(), 3);
      group_momentum_ += colliding_body->GetVelocity() * colliding_mass;
      // Position of new Body should be in center of mass of colliding Bodies.
      group_mass_center_ += colliding_body->pos() * colliding_mass;
      collision_list_.removeOne(colliding_body);
      if (colliding_body != body) {
        colliding_body->DeleteTrails();
        this->removeItem(colliding_body);
        body_list_.removeOne(colliding_body);
      }
    }
    body->SetMass(group_mass_);
    body->SetRadius(cbrt(group_volume_));
    body->SetVelocity(group_momentum_ / group_mass_);
    body->setPos(group_mass_center_ / group_mass_);
    local_collision_list_.clear();
  }
}

void Scene::AdvanceTrails(Body *body) {
  QGraphicsLineItem *trail = body->trails_.at(body->trail_iterator_);
  trail->setLine(body->last_position_.x(), body->last_position_.y(),
                 body->pos().x(), body->pos().y());
  if (body->trail_iterator_ < body->kTrailCount - 1)
    ++(body->trail_iterator_);
  else
    body->trail_iterator_ = 0;
}

void Scene::Advance() {
  if (runge_kutta_ && !body_list_.isEmpty())
    AdvanceRungeKutta();
  else if (!body_list_.isEmpty())
    AdvanceEuler();

  ResolveCollisions();
}
