/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@plexydesk.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include <plexy.h>
#include "clockwidget.h"
#include <qmatrix.h>
// Added by qt3to4:
#include <QPixmap>
#include <QPaintEvent>
#include <math.h>
#include <QBitmap>
#include <QTimer>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDateTime>
#include <QDir>
#include <plexyconfig.h>

namespace UIKit {
ClockWidget::ClockWidget(QGraphicsObject *parent)
    : UIKit::Widget(parent), m_timezone(0) {
  m_timer_ptr = new QTimer(this);
  m_timer_ptr->setTimerType(Qt::VeryCoarseTimer);

  connect(m_timer_ptr, SIGNAL(timeout()), this, SLOT(on_timout_slot_func()));
  m_timer_ptr->start(1000);

  set_widget_flag(Widget::kRenderDropShadow, false);
  setFlag(QGraphicsItem::ItemIsMovable, false);
  set_widget_name("Clock");
  on_timout_slot_func();
}

void ClockWidget::update_time(const QVariantMap &a_data) {
  QDateTime _date_time = QDateTime::currentDateTime();

  if (m_timezone)
    _date_time = _date_time.toTimeZone(*m_timezone);

  m_second_value = 6.0 * _date_time.time().second();
  m_minutes_value = 6.0 * _date_time.time().minute();
  m_hour_value = 30.0 * _date_time.time().hour();

  update();
}

void ClockWidget::set_timezone_id(const QByteArray &a_timezone_id) {
  m_timezone_id = a_timezone_id;

  m_timer_ptr->stop();

  if (m_timezone) {
    delete m_timezone;
  }

  m_timezone = new QTimeZone(a_timezone_id);
  qDebug() << Q_FUNC_INFO << a_timezone_id;

  m_timer_ptr->start();
}

ClockWidget::~ClockWidget() {
  if (m_timer_ptr) {
    m_timer_ptr->stop();
    delete m_timer_ptr;
  }

  if (m_timezone)
    delete m_timezone;
  qDebug() << Q_FUNC_INFO;
}

void ClockWidget::draw_clock_hands(QPainter *p, QRectF rect, int factor,
                                   float angle, QColor hand_color,
                                   int thikness) {
  p->save();
  float _adjustment = rect.width() / factor;

  QRectF _clock_hour_rect(rect.x() + _adjustment, rect.y() + _adjustment,
                          rect.width() - (_adjustment * 2),
                          rect.height() - (_adjustment * 2));

  QTransform _xform_hour;
  QPointF _transPos = _clock_hour_rect.center();
  _xform_hour.translate(_transPos.x(), _transPos.y());
  _xform_hour.rotate(angle);
  _xform_hour.translate(-_transPos.x(), -_transPos.y());
  p->setTransform(_xform_hour);

  QPen _clock_hour_pen(hand_color, thikness, Qt::SolidLine, Qt::RoundCap,
                       Qt::RoundJoin);
  p->setPen(_clock_hour_pen);

  // p->drawRect(_clock_hour_rect);
  p->drawLine(_clock_hour_rect.topLeft(), _clock_hour_rect.center());
  p->restore();
}

void ClockWidget::on_timout_slot_func() {
  update_time(QVariantMap());
  update();
}

void ClockWidget::paint_view(QPainter *p, const QRectF &r) {
  QRectF rect(r.x() + 8, r.y() + 8, r.width() - 16, r.height() - 16);

  p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing |
                    QPainter::HighQualityAntialiasing);

  QPen _clock_frame_pen(QColor("#646464"), 8, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin);
  p->setPen(_clock_frame_pen);

  QPainterPath _clock_background;
  _clock_background.addEllipse(rect);

  p->fillPath(_clock_background, QColor("#646464"));
  p->drawEllipse(rect);

  /* Draw Hour Hand */
  draw_clock_hands(p, rect, 3, 45.0 + m_hour_value, QColor("#f0f0f0"), 8);
  draw_clock_hands(p, rect, 4, 45.0 + m_minutes_value, QColor("#FFFFFF"), 6);
  draw_clock_hands(p, rect, 5, 45.0 + m_second_value, QColor("#d6d6d6"), 2);

  QRectF _clock_wheel_rect(rect.center().x() - 8, rect.center().y() - 8, 16,
                           16);
  QPainterPath _clock_wheel_path;

  _clock_wheel_path.addEllipse(_clock_wheel_rect);

  p->fillPath(_clock_wheel_path, QColor("#d6d6d6"));
}
}