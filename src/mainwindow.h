/**
  ******************************************************************************
  * @file    mainwindow.h
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Header file of MainWindow class.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QShortcut>

#include "scene.h"
#include "view.h"

/**
  * @brief Main application window.
  */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
    * @brief MainWindow constructor.
    * @param parent Parent of object.
    */
  MainWindow(QWidget *parent = 0);

  /**
    * @brief MainWindow destructor.
    */
  ~MainWindow();

 private:
  /**
    * @brief Initializes Scene and View.
    */
  void SceneAndViewInit();

  /**
    * @brief Initializes window menus.
    */
  void MenuInit();

  /**
    * @brief Initializes file menu.
    */
  void FileMenuInit();

  /**
    * @brief Initializes options menu.
    */
  void OptionsMenuInit();

  /**
    * @brief Initializes sliders.
    */
  void SlidersInit();

  /**
    * @brief Initializes buttons.
    */
  void ButtonsInit();

  /**
    * @brief Initializes widgets layout.
    */
  void LayoutInit();

  /**
    * @brief  Finds random integer number in specified range.
    * @param  low Minimum value.
    * @param  high Maximum value.
    * @retval Random integer number.
    */
  int RandInt(int low, int high) const;

  /**
    * @brief  Adds new planet to Scene.
    * @param  mass Mass of new planet [10^24 kg].
    * @param  density Density of new planet [kg/m^3].
    * @param  semi_major_axis Semi-major axis of orbit of new planet [10^6 km].
    * @param  eccentricity Eccentricity of orbit of new planet.
    * @param  angle Initial angle of orbit of new planet [°].
    * @retval New planet.
    */
  Body *AddPlanet(qreal mass, qreal density, qreal semi_major_axis,
                  qreal eccentricity, qreal angle);

  /**
    * @brief  Adds new moon to Scene.
    * @param  planet Satellite of.
    * @param  mass Mass of new moon [10^24 kg].
    * @param  density Density of new moon [kg/m^3].
    * @param  semi_major_axis Semi-major axis of orbit of new moon [10^6 km].
    * @param  eccentricity Eccentricity of orbit of new moon.
    * @param  angle Initial angle of orbit of new moon [°].
    */
  void AddMoon(const Body *planet, qreal mass, qreal density,
               qreal semi_major_axis, qreal eccentricity,
               qreal angle);

  View *view_;
  Scene *scene_;
  QSlider *zoom_slider_;
  QSlider *mass_slider_;
  QSlider *density_slider_;
  QSlider *time_slider_;
  QLabel *zoom_label_;
  QLabel *mass_label_;
  QLabel *density_label_;
  QLabel *radius_label_;
  QLabel *time_label_;
  QGridLayout *main_layout_;
  QPushButton *create_button_;
  QPushButton *delete_button_;
  QPushButton *pause_button_;
  QPushButton *drag_button_;
  QShortcut *pause_shortcut_;
  QHBoxLayout *button_layout_;
  QMenu *file_menu_;
  QMenu *options_menu_;
  QAction *new_action_;
  QAction *quit_action_;
  QAction *load_sol_action_;
  QAction *load_proto_action_;
  QAction *set_trails_action_;
  QAction *set_aa_action_;
  QAction *set_euler_action_;
  QAction *set_rk4_action_;
  QActionGroup *options_action_group_;
  // Current zoom of View.
  qreal current_scale_;

 private slots:
  /**
    * @brief Resets Scene and View.
    */
  void DeleteAll();

  /**
    * @brief Loads The Solar System preset.
    */
  void LoadSolarSystem();

  /**
    * @brief Loads protostar with protodisk preset.
    */
  void LoadProtodisk();

  /**
    * @brief Toggles trails.
    */
  void SetTrails();

  /**
    * @brief Toggles antialiasing.
    */
  void SetAntialiasing();

  /**
    * @brief Zooms View.
    * @param value Scaling value.
    */
  void Zoom(int value);

  /**
    * @brief Changes mass of new Bodies.
    * @param value Mass of new Bodies.
    */
  void ChangeMass(int value);

  /**
    * @brief Changes density of new Bodies.
    * @param value Density of new Bodies.
    */
  void ChangeDensity(int value);

  /**
    * @brief Changes radius of new Bodies.
    */
  void ChangeRadius();

  /**
    * @brief Changes time step of simulation.
    * @param value New time step value.
    */
  void ChangeTime(int value);

  /**
    * @brief Handles click of button on interface.
    * @param check Value of button.
    */
  void ButtonClicked(bool check);

  /**
    * @brief Set Runge-Kutta method mode.
    */
  void SetEuler();

  /**
    * @brief Set Euler method mode.
    */
  void SetRK4();
};

#endif // MAINWINDOW_H
