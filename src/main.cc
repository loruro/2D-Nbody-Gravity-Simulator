/**
  ******************************************************************************
  * @file    main.cc
  * @author  Karol Leszczyński
  * @version V1.0.0
  * @date    28-December-2013
  * @brief   Main.
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

#include <QApplication>
#include <QDesktopWidget>

#include "mainwindow.h"

/**
  * @brief  Main function.
  * @retval Value thas was set to exit().
  */
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  int width = QApplication::desktop()->width();
  int height = QApplication::desktop()->height();
  MainWindow w;

  // Placing window on center of a screen (if resolution higher than 1280x720).
  if (width > 1280 && height > 720)
    w.move((width - 1280) / 2, (height - 720) / 2);
  else
    w.move(0, 0);

  w.show();
  return a.exec();
}
