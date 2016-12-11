/**
  ******************************************************************************
  * @file    mainwindow.cc
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   MainWindow class.
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

#include "mainwindow.h"

#include <QMenuBar>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      current_scale_(1) {
  QTime time = QTime::currentTime();
  qsrand(time.msec());

  this->setWindowTitle("Gravity Simulator");
  this->resize(1280, 720);
  SceneAndViewInit();
  this->setCentralWidget(view_);

  MenuInit();
  SlidersInit();
  ButtonsInit();
  LayoutInit();

  scene_->SetTool(Scene::kCreate);
  view_->SetZoomSlider(zoom_slider_);
  ChangeMass(1.0);
  ChangeDensity(1000.0);
  ChangeTime(10);
  Zoom(0);
}

MainWindow::~MainWindow() {
  delete scene_;
  delete drag_button_;
  delete pause_button_;
  delete delete_button_;
  delete create_button_;
  delete zoom_slider_;
  delete mass_slider_;
  delete density_slider_;
  delete time_slider_;
  delete zoom_label_;
  delete mass_label_;
  delete density_label_;
  delete radius_label_;
  delete time_label_;
  delete button_layout_;
  delete main_layout_;
  delete view_;
  delete new_action_;
  delete quit_action_;
  delete load_sol_action_;
  delete load_proto_action_;
  delete options_action_group_;
  delete set_trails_action_;
  delete set_aa_action_;
  delete set_euler_action_;
  delete set_rk4_action_;
  delete pause_shortcut_;
  delete file_menu_;
  delete options_menu_;
}

void MainWindow::SceneAndViewInit() {
  scene_ = new Scene(this, current_scale_);

  view_ = new View(this);
  view_->setScene(scene_);
  view_->setRenderHints(QPainter::Antialiasing);
  view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  // Remove unnecessary object which was created earlier.
  scene_->removeItem(scene_->items().at(0));
}

void MainWindow::MenuInit() {
  FileMenuInit();
  OptionsMenuInit();
}

void MainWindow::FileMenuInit() {
  file_menu_ = menuBar()->addMenu("&File");

  new_action_ = new QAction("&New", this);
  file_menu_->addAction(new_action_);
  connect(new_action_, SIGNAL(triggered()), this, SLOT(DeleteAll()));

  load_sol_action_ = new QAction("&Load Solar System", this);
  file_menu_->addAction(load_sol_action_);
  connect(load_sol_action_, SIGNAL(triggered()), this, SLOT(LoadSolarSystem()));

  load_proto_action_ = new QAction("&Load Protoplanetary Disk", this);
  file_menu_->addAction(load_proto_action_);
  connect(load_proto_action_, SIGNAL(triggered()), this, SLOT(LoadProtodisk()));

  file_menu_->addSeparator();

  quit_action_ = new QAction("&Quit", this);
  file_menu_->addAction(quit_action_);
  connect(quit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::OptionsMenuInit() {
  options_menu_ = menuBar()->addMenu("&Options");
  options_action_group_ = new QActionGroup(this);

  set_trails_action_ = new QAction("Trails", this);
  options_menu_->addAction(set_trails_action_);
  set_trails_action_->setCheckable(true);
  connect(set_trails_action_, SIGNAL(triggered()), this, SLOT(SetTrails()));

  set_aa_action_ = new QAction("&Antialiasing", this);
  options_menu_->addAction(set_aa_action_);
  set_aa_action_->setCheckable(true);
  set_aa_action_->setChecked(true);
  connect(set_aa_action_, SIGNAL(triggered()), this, SLOT(SetAntialiasing()));

  options_menu_->addSeparator();

  set_euler_action_ = new QAction("&Euler", this);
  options_menu_->addAction(set_euler_action_);
  set_euler_action_->setCheckable(true);
  set_euler_action_->setChecked(true);
  options_action_group_->addAction(set_euler_action_);
  connect(set_euler_action_, SIGNAL(triggered()), this, SLOT(SetEuler()));

  set_rk4_action_ = new QAction("&Runge-Kutta 4", this);
  options_menu_->addAction(set_rk4_action_);
  set_rk4_action_->setCheckable(true);
  options_action_group_->addAction(set_rk4_action_);
  connect(set_rk4_action_, SIGNAL(triggered()), this, SLOT(SetRK4()));
}

void MainWindow::SlidersInit() {
  zoom_slider_ = new QSlider(Qt::Vertical, view_);
  zoom_slider_->setRange(-300, 100);
  zoom_slider_->setValue(0);
  connect(zoom_slider_, SIGNAL(valueChanged(int)), this, SLOT(Zoom(int)));
  zoom_label_ = new QLabel("", this);

  mass_slider_ = new QSlider(Qt::Horizontal, view_);
  mass_slider_->setRange(1, 1000000);
  mass_slider_->setValue(1);
  connect(mass_slider_, SIGNAL(valueChanged(int)), this, SLOT(ChangeMass(int)));
  mass_label_ = new QLabel("", view_);

  density_slider_ = new QSlider(Qt::Horizontal, view_);
  density_slider_->setRange(500, 6000);
  density_slider_->setValue(1000);
  connect(density_slider_, SIGNAL(valueChanged(int)),
          this, SLOT(ChangeDensity(int)));
  density_label_ = new QLabel("", view_);

  radius_label_ = new QLabel("", view_);

  time_slider_ = new QSlider(Qt::Horizontal, view_);
  time_slider_->setRange(1, 50);
  time_slider_->setValue(10);
  connect(time_slider_, SIGNAL(valueChanged(int)), this, SLOT(ChangeTime(int)));
  time_label_ = new QLabel("", view_);
}

void MainWindow::ButtonsInit() {
  pause_button_ = new QPushButton("Pause", view_);
  pause_button_->setCheckable(1);
  connect(pause_button_, SIGNAL(toggled(bool)), this,
          SLOT(ButtonClicked(bool)));
  pause_shortcut_ = new QShortcut(Qt::Key_Space, this);
  connect(pause_shortcut_, SIGNAL(activated()), pause_button_, SLOT(click()));

  drag_button_ = new QPushButton("Drag view", view_);
  drag_button_->setCheckable(1);
  drag_button_->setAutoExclusive(1);
  connect(drag_button_, SIGNAL(toggled(bool)), this, SLOT(ButtonClicked(bool)));

  create_button_ = new QPushButton("Create", view_);
  create_button_->setCheckable(1);
  create_button_->setAutoExclusive(1);
  create_button_->setChecked(1);
  connect(create_button_, SIGNAL(toggled(bool)),
          this, SLOT(ButtonClicked(bool)));

  delete_button_ = new QPushButton("Delete", view_);
  delete_button_->setCheckable(1);
  delete_button_->setAutoExclusive(1);
  connect(delete_button_, SIGNAL(toggled(bool)),
          this, SLOT(ButtonClicked(bool)));

  // Group buttons together.
  button_layout_ = new QHBoxLayout();
  button_layout_->addWidget(pause_button_);
  button_layout_->addWidget(drag_button_);
  button_layout_->addWidget(create_button_);
  button_layout_->addWidget(delete_button_);
}

void MainWindow::LayoutInit() {
  main_layout_ = new QGridLayout(view_);
  main_layout_->setColumnMinimumWidth(0, 150);
  main_layout_->setColumnMinimumWidth(2, 70);
  main_layout_->setColumnStretch(0, 1);
  main_layout_->setColumnStretch(1, 10);
  main_layout_->setColumnStretch(2, 1);
  main_layout_->setRowStretch(0, 1);
  main_layout_->setRowStretch(1, 1);
  main_layout_->setRowStretch(2, 1);
  main_layout_->setRowStretch(3, 1);
  main_layout_->setRowStretch(4, 1);
  main_layout_->setRowStretch(5, 1);
  main_layout_->setRowStretch(6, 100);
  main_layout_->setRowStretch(8, 1);
  main_layout_->setRowStretch(9, 2);
  main_layout_->addWidget(zoom_slider_, 1, 2, 8, 1, Qt::AlignHCenter);
  main_layout_->addWidget(zoom_label_, 0, 2, 1, 1, Qt::AlignHCenter);
  main_layout_->addLayout(button_layout_, 0, 0, 1, 1);
  main_layout_->addWidget(density_label_, 1, 0, 1, 1, Qt::AlignHCenter);
  main_layout_->addWidget(density_slider_, 2, 0, 1, 1);
  main_layout_->addWidget(mass_label_, 3, 0, 1, 1, Qt::AlignHCenter);
  main_layout_->addWidget(mass_slider_, 4, 0, 1, 1);
  main_layout_->addWidget(radius_label_, 5, 0, 1, 1, Qt::AlignHCenter);
  main_layout_->addWidget(time_label_, 7, 0, 1, 1);
  main_layout_->addWidget(time_slider_, 8, 0, 1, 1, Qt::AlignLeft);
}

int MainWindow::RandInt(int low, int high) const {
  return qrand() % ((high + 1) - low) + low;
}

Body *MainWindow::AddPlanet(qreal mass, qreal density, qreal semi_major_axis,
                            qreal eccentricity, qreal angle) {
  // Calculations of proper velocity and position of planet which will create
  // desired orbit around Sun.
  qreal velocity = scene_->kGravConstant * 1989100 * (1 + eccentricity);
  velocity /= semi_major_axis * 100 * (1 - eccentricity);
  velocity = sqrt(velocity);
  qreal position = semi_major_axis * 100 * (1 - eccentricity);
  qreal angle_rad = angle * M_PI / 180;
  Body *body = new Body(mass, 10 * cbrt(mass / (density * 4.189)),
                        -velocity * sin(angle_rad), velocity * cos(angle_rad),
                        position * cos(angle_rad), position * sin(angle_rad));
  scene_->addItem(body);
  scene_->body_list_.append(body);
  if (scene_->trails_)
    body->CreateTrails(current_scale_);
  return body;
}

void MainWindow::AddMoon(const Body *planet, qreal mass, qreal density,
                         qreal semi_major_axis, qreal eccentricity,
                         qreal angle) {
  // Calculations of proper velocity and position of moon which will create
  // desired orbit around planet.
  qreal velocity = scene_->kGravConstant;
  velocity *= planet->GetMass() * (1 + eccentricity);
  velocity /= semi_major_axis * 100 * (1  -eccentricity);
  velocity = sqrt(velocity);
  qreal position = semi_major_axis * 100 * (1 - eccentricity);
  qreal angle_rad = angle * M_PI / 180;
  Body *body = new Body(mass, 10 * cbrt(mass / (density * 4.189)),
                        -velocity * sin(angle_rad) + planet->GetVelocity().x(),
                        velocity * cos(angle_rad) + planet->GetVelocity().y(),
                        position * cos(angle_rad) + planet->pos().x(),
                        position * sin(angle_rad) + planet->pos().y());
  scene_->addItem(body);
  scene_->body_list_.append(body);
  if (scene_->trails_)
    body->CreateTrails(current_scale_);
}

void MainWindow::DeleteAll() {
  foreach (Body* body, scene_->body_list_) {
    body->DeleteTrails();
    scene_->removeItem(body);
    scene_->body_list_.clear();
    delete body;
  }
  zoom_slider_->setValue(0);
  view_->centerOn(0, 0);
}

void MainWindow::LoadSolarSystem() {
  // Initialize scene.
  DeleteAll();
  zoom_slider_->setValue(-100);
  set_trails_action_->setChecked(true);
  SetTrails();

  // Add Sun.
  Body *body = new Body(1989100.0, 10 * cbrt(1989100 / (1409 * 4.189)),
                        0.0, 0.0, 0.0, 0.0);
  scene_->addItem(body);
  scene_->body_list_.append(body);
  if (scene_->trails_)
    body->CreateTrails(current_scale_);

  // Add planets and moons.
  body = AddPlanet(0.3301, 5427, 57.909227, 0.20563593, 48.331);     // Mercury
  body = AddPlanet(4.8673, 5243, 108.20948, 0.00677672, 76.678);     // Venus
  body = AddPlanet(5.9722, 5513, 149.59826, 0.01671123, 348.73936);  // Earth
  AddMoon(body, 0.073477, 3346, 0.384399, 0.0549, 125.08);  // Moon
  body = AddPlanet(0.64169, 3934, 227.94382, 0.0933941, 49.562);     // Mars
  body = AddPlanet(1898.1, 1326, 778.34082, 0.04838624, 100.492);    // Jupiter
  AddMoon(body, 0.0894, 3528, 0.4216, 0.0041, 0);           // Io
  AddMoon(body, 0.048, 3010, 0.6709, 0.009, 0);             // Europa
  AddMoon(body, 0.14819, 1936, 1.0704, 0.0013, 0);          // Ganymede
  AddMoon(body, 0.10758, 1830, 1.8827, 0.0074, 0);          // Callisto
  body = AddPlanet(568.32, 687, 1426.6664, 0.05386179, 113.643);     // Saturn
  AddMoon(body, 0.0000375, 1150, 0.18552, 0.0202, 0);       // Mimas
  AddMoon(body, 0.000108, 1610, 0.237948, 0.0047, 0);       // Enceladus
  AddMoon(body, 0.0006174, 980, 0.294619, 0.02, 0);         // Tethys
  AddMoon(body, 0.001095, 1480, 0.377396, 0.002, 0);        // Dione
  AddMoon(body, 0.002306, 1230, 0.527108, 0.001, 0);        // Rhea
  AddMoon(body, 0.13452, 1880, 1.22187, 0.0288, 0);         // Titan
  AddMoon(body, 0.0018053, 1080, 3.56082, 0.0286, 0);       // Iapetus
  body = AddPlanet(86.81, 1270, 2870.6582, 0.04725744, 73.99);       // Uranus
  AddMoon(body, 0.0000659, 1200, 0.12939, 0.0013, 0);       // Miranda
  AddMoon(body, 0.00135, 1670, 0.1909, 0.0012, 0);          // Ariel
  AddMoon(body, 0.0012, 1400, 0.2662, 0.005, 0);            // Umbriel
  AddMoon(body, 0.0035, 1720, 0.4363, 0.0011, 0);           // Titania
  AddMoon(body, 0.003014, 1630, 0.583519, 0.0014, 0);       // Oberon
  body = AddPlanet(102.41, 1638, 4498.3964, 0.00859048, 131.794);    // Neptune
  AddMoon(body, 0.0214, 2061, 0.354759, 0.00002, 0);        // Triton
}

void MainWindow::LoadProtodisk() {
  // Initialize scene.
  DeleteAll();
  zoom_slider_->setValue(-70);
  set_trails_action_->setChecked(false);
  SetTrails();
  qreal mass;
  qreal radius;
  qreal density;
  Body *body;

  // Add protostar.
  mass = 1000000;
  density = 6000;
  radius = 100.0 * cbrt(mass / (density * 4.189));
  body = new Body(mass, radius, 0.0, 0.0, 0.0, 0.0);
  scene_->addItem(body);
  scene_->body_list_.append(body);
  if (scene_->trails_)
    body->CreateTrails(current_scale_);

  // Add protodisk objects.
  mass = 1;
  density = 500;
  radius = 100.0 * cbrt(mass / (density * 4.189));
  for (int i = 0; i < 1000; ++i) {
    qreal min_disk_radius_squared = 250000.0;
    qreal max_disk_radius_squared = 2250000.0;
    qreal disk_radius = RandInt(0, 1000) * 0.001;
    disk_radius *= max_disk_radius_squared - min_disk_radius_squared;
    disk_radius += min_disk_radius_squared;
    disk_radius = sqrt(disk_radius);
    qreal disk_angle = RandInt(0, 3600) * 0.1 * M_PI / 180.0;
    qreal body_vel_x = -sqrt(6673850000.0 / disk_radius) * sin(disk_angle);
    qreal body_vel_y = sqrt(6673850000.0 / disk_radius) * cos(disk_angle);
    qreal body_pos_x = disk_radius * cos(disk_angle);
    qreal body_pos_y = disk_radius * sin(disk_angle);
    body = new Body(mass, radius,
                    body_vel_x, body_vel_y, body_pos_x, body_pos_y);
    scene_->addItem(body);
    scene_->body_list_.append(body);
    if (scene_->trails_)
      body->CreateTrails(current_scale_);
  }
}

void MainWindow::SetTrails() {
  if (set_trails_action_->isChecked()) {
    scene_->trails_ = true;
    foreach (Body *body, scene_->body_list_)
      body->CreateTrails(current_scale_);
  } else {
    scene_->trails_ = false;
    foreach (Body *body, scene_->body_list_)
      body->DeleteTrails();
  }
}

void MainWindow::SetAntialiasing() {
  view_->setRenderHint(QPainter::Antialiasing, set_aa_action_->isChecked());
  foreach (Body *body, scene_->body_list_) {
    // Cache has to be reset.
    body->setCacheMode(QGraphicsItem::NoCache);
    body->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
  }
}

void MainWindow::Zoom(int value) {
  // Zoom is in logarithmic scale.
  qreal scale = pow(10.0, value / 100.0);
  view_->scale(scale / current_scale_, scale / current_scale_);
  current_scale_ = scale;
  scene_->view_scale_ = current_scale_;
  QString label_text = "<font color='white'>Zoom: ";
  label_text += QString::number(current_scale_);
  label_text += "</font>";
  zoom_label_->setText(label_text);
  // Change size of creation line and trails in Scene,
  // so their visible size stays the same.
  scene_->creation_line_->setPen(QPen(Qt::white, 1 / current_scale_));
  foreach (Body *body, scene_->body_list_)
    foreach (QGraphicsLineItem *line, body->trails_)
      line->setPen(QPen(Qt::white, 0.25 / current_scale_));
}

void MainWindow::ChangeMass(int value) {
  scene_->SetMass(value);
  QString label_text = "<font color='white'>Mass: ";
  label_text += QString::number(value);
  label_text += "</font>";
  mass_label_->setText(label_text);

  ChangeRadius();
}

void MainWindow::ChangeDensity(int value) {
  scene_->SetDensity(value);
  QString label_text = "<font color='white'>Density: ";
  label_text += QString::number((value / 1000.0));
  label_text += "</font>";
  density_label_->setText(label_text);

  ChangeRadius();
}

void MainWindow::ChangeRadius() {
  qreal radius = scene_->GetMass() / (4.189 * scene_->GetDensity());
  radius = 100.0 * pow(radius, 1.0 / 3.0);
  scene_->SetRadius(radius);
  QString label_text = "<font color='white'>Radius: ";
  label_text += QString::number(scene_->GetRadius());
  label_text += "</font>";
  radius_label_->setText(label_text);
}

void MainWindow::ChangeTime(int value) {
  scene_->SetTimeStep(value / 1000.0);
  QString label_text = "<font color='white'>Time step: ";
  label_text += QString::number(value / 1000.0);
  label_text += "</font>";
  time_label_->setText(label_text);
}

void MainWindow::ButtonClicked(bool check) {
  QObject* obj = sender();
  if (obj == pause_button_) {
    if (check)
      scene_->advancing_timer_->stop();
    else
      scene_->advancing_timer_->start(10);

  } else if (obj == drag_button_) {
    if (check) {
      view_->setDragMode(QGraphicsView::ScrollHandDrag);
      scene_->SetTool(Scene::kNone);
    }

  } else if (obj == create_button_) {
    if (check) {
      scene_->SetTool(Scene::kCreate);
      view_->setDragMode(QGraphicsView::NoDrag);
    }

  } else if (obj == delete_button_) {
    if (check) {
      scene_->SetTool(Scene::kDelete);
      view_->setDragMode(QGraphicsView::NoDrag);
    }
  }
}

void MainWindow::SetEuler() {
  scene_->runge_kutta_ = false;
}

void MainWindow::SetRK4() {
  scene_->runge_kutta_ = true;
}
