/**
  ******************************************************************************
  * @file    body.cc
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Body class.
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

#include "body.h"

#include <QGraphicsScene>

Body::Body(qreal mass, qreal radius, QPointF vel, QPointF pos)
    : QGraphicsEllipseItem(),
      trail_iterator_(0),
      last_position_(pos),
      k1dx_(QPointF(0.0, 0.0)),
      k1dv_(QPointF(0.0, 0.0)),
      k2dx_(QPointF(0.0, 0.0)),
      k2dv_(QPointF(0.0, 0.0)),
      k3dx_(QPointF(0.0, 0.0)),
      k3dv_(QPointF(0.0, 0.0)),
      k4dx_(QPointF(0.0, 0.0)),
      k4dv_(QPointF(0.0, 0.0)),
      velocity_(vel) {
  SetMass(mass);
  SetRadius(radius);
  setPos(pos);
  setZValue(1);
  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
  setPen(Qt::NoPen);
}

Body::~Body() {
  foreach (QGraphicsLineItem *trail, trails_)
    delete trail;
}

qreal Body::GetRadius() const {
  return radius_;
}

void Body::SetRadius(qreal radius) {
  radius_ = radius;
  setRect(-radius_, -radius_, 2.0 * radius_, 2.0 * radius_);
}

qreal Body::GetMass() const {
  return mass_;
}

void Body::SetMass(qreal mass) {
  mass_ = mass;

  // Setting colour of Body depending on its mass.
  QColor color;
  if (mass_ < 500000) {
    qreal value = std::min(255 * mass_ / 500000, 255.0);
    color = QColor(value, 255, 0);
  } else {
    qreal value = std::max(255 - 255 * (mass_ - 500000) / 500000, 0.0);
    color = QColor(255, value, 0);
  }
  setBrush(QBrush(color));
}

QPointF Body::GetVelocity() const {
  return velocity_;
}

void Body::SetVelocity(QPointF velocity) {
  velocity_ = velocity;
}

void Body::SetVelocity(qreal vel_x, qreal vel_y) {
  velocity_ = QPointF(vel_x, vel_y);
}

void Body::CreateTrails(qreal view_scale) {
  last_position_ = pos();
  QGraphicsLineItem *trail;
  for (int i = 0; i < kTrailCount; ++i) {
    trail = new QGraphicsLineItem();
    trails_.append(trail);
    scene()->addItem(trail);
    trail->setPen(QPen(QColor(255, 255, 255)));
    trail->setPos(0, 0);
    trail->setLine(pos().x(), pos().y(), pos().x(), pos().y());
    // Trail's size in Scene depends of current zoom of View, so the visible
    // size stays the same.
    trail->setPen(QPen(Qt::white, 0.25 / view_scale));
  }
}

void Body::DeleteTrails() {
  foreach (QGraphicsLineItem *trail, trails_) {
    scene()->removeItem(trail);
    delete trail;
  }
  trails_.clear();
}
