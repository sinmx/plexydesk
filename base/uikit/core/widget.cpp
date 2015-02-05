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

#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QGraphicsObject>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include <view_controller.h>
#include "widget.h"

/**
 * \class UI::AbstractDesktopWidget
 *
 * \ingroup PlexyDesk
 *
 * \brief Base class for Visual Desktop Extensions
 * UI::AbstractDesktopWidget defines all the common features of a visual
 *desktop extension.
 * When writing visual extensions, the user must inherit from this class. So
 *that it behaves the
 * same way.
 */

/**
 * \fn UI::AbstractDesktopWidget::boundingRect()
 * \breif Returns the Bounding rectangle of the Widget Content
 *
 * \paragraph This method returns the bounding Rectangle of the Widget Contents
 * it returns the user specificed content rectangle
 *(UI::AbstractDesktopWidget::setContentRect())
 * And does not automatically calculate or adjust with the content changes, The
 *size of the content must be
 * manually set if the content changes and this widget needs to adjust to that
 *size.
 *
 * @returns The bounding Rectangle for this widget
 * \sa UI::AbstractDesktopWidget::setContentRect()
 * \sa UI::AbstractDesktopWidget::setDockRect()
 */

/**
* \fn UI::AbstractDesktopWidget::setContentRect()
*\brief Set the content Rectangle of the Widget
*
* \paragraph Sets the content rectangle of the widget
* in situations where the widget can't
* find out by it self. For instance widets which changes
* it's content during runtime or chiild QML widgets.
* Use it only if you are changing the widget size.
*
* \param  rect The Content bounding box area
*/

/**
* \fn UI::AbstractDesktopWidget::setRect()
*\brief Set the bounding Rectangle of the Widget
*
* \paragraph Sets the bounding rectangle of the widget
* in situations where the widget rectangle property needs
* to be set.
*
* \param  rect The Content bounding box area
*/

/**
  *\fn UI::AbstractDesktopWidget::setChildWidetVisibility()
  *
  * \brief Hides the child items of this widget
  *
  * \param show Decides the visibility of the child widgets, set true to show
  *the child elements
  * false to hide them.
  *
  */

/** UI::AbstractDesktopWidget::setLabel
  * \fn
  * \brief Sets a label for the widget
  * \param name Name of the icon
  *
  * Label to display when the widget is in dock mode
  */

namespace UI
{

class Window::PrivateAbstractDesktopWidget
{
public:
  PrivateAbstractDesktopWidget() : m_widget_controller(0) {}
  ~PrivateAbstractDesktopWidget() {}

  RenderLevel m_current_layer_type;
  QString m_widget_name;
  QRectF m_minized_view_geometry;
  ViewController *m_widget_controller;
};

Window::Window(QGraphicsObject *parent)
  : QGraphicsObject(parent),
    QGraphicsLayoutItem(0),
    d(new PrivateAbstractDesktopWidget)
{
  d->m_widget_name = QLatin1String("Widget");
  d->m_current_layer_type = kRenderAtForgroundLevel;

  d->m_minized_view_geometry = QRectF(0.0, 0.0, 64, 64);

  setCacheMode(DeviceCoordinateCache);
  setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);
  setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
  setAcceptTouchEvents(true);
  setAcceptHoverEvents(true);
  setGraphicsItem(this);
}

Window::~Window() { delete d; }

QRectF Window::boundingRect() const
{
    return QRectF(QPointF(0, 0), geometry().size()); // d->m_content_geometry;
}

void Window::setWindowFlag(int flags, bool enable)
{
}

void Window::setMinimizedGeometry(const QRectF &rect)
{
  d->m_minized_view_geometry = rect;
}

QRectF Window::minimizedGeometry() const { return d->m_minized_view_geometry; }

void Window::setLabelName(const QString &name) { d->m_widget_name = name; }

QString Window::label() const { return d->m_widget_name; }

/*
void Widget::setContentsRect(const QRectF &rect)
{
    d->m_content_geometry = rect;

    prepareGeometryChange();
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    resetMatrix();
    update();
    updateGeometry();
}
*/

/*
QRectF Widget::geometry() const
{
    return d->m_content_geometry;
}
*/

QString Window::uuid() const
{
  QString rv;

  rv = QString(QCryptographicHash::hash((label().toLatin1()),
                                        QCryptographicHash::Md5).toHex());

  return rv;
}

Window::RenderLevel Window::layerType() const
{
  return d->m_current_layer_type;
}

void Window::setLayerType(RenderLevel level) const
{
  d->m_current_layer_type = level;
}

void Window::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget * /*widget*/)
{
  if (!painter->isActive()) {
    return;
  }
  if (isObscured()) {
    return;
  }

  painter->setClipRect(boundingRect());
  paintView(painter, boundingRect());
}

void Window::setGeometry(const QRectF &rect)
{
  // d->m_content_geometry = rect;
  prepareGeometryChange();
  QGraphicsLayoutItem::setGeometry(rect);
  setPos(rect.topLeft());
}

QSizeF Window::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  // todo: ignoreing which for now. we will return based on
  // return geometry().size();
  QSizeF sh;
  switch (which) {
  case Qt::MinimumSize:
    sh = QSizeF(0, 0);
    break;
  case Qt::PreferredSize:
    sh = QSizeF(50, 50); // rather arbitrary
    break;
  case Qt::MaximumSize:
    sh = QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    break;
  default:
    qWarning("QGraphicsWidget::sizeHint(): Don't know how to handle the "
             "value of 'which'");
    break;
  }
  return sh;
}

void Window::paintView(QPainter *painter, const QRectF &rect)
{
  StyleFeatures feature;
  feature.geometry = rect;

  if (style()) {
    style()->draw("window_frame", feature, painter);
  }

}

void Window::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsObject::mousePressEvent(event);
}

void Window::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  /*
  if (controller() && controller()->viewport()) {
      AbstractDesktopView *view =
  qobject_cast<AbstractDesktopView*>(controller()->viewport());
      if (view)
        view->saveItemLocationToSession(controller()->controllerName(), pos(),
  this->uuid());
  }
  */

  Q_EMIT clicked();
  QGraphicsObject::mouseReleaseEvent(event);
}

void Window::focusOutEvent(QFocusEvent *event)
{
  event->accept();
  Q_EMIT focusLost();
}

float Window::scaleFactorForWidth() const
{
  return geometry().width() / boundingRect().width();
}

float Window::scaleFactorForHeight() const
{
  return geometry().height() / boundingRect().height();
}

void Window::setChildWidetVisibility(bool show)
{
  Q_FOREACH(QGraphicsItem * item, this->childItems()) {
    (show) ? item->show() : item->hide();
  }
}

void Window::setController(ViewController *view_controller)
{
  d->m_widget_controller = view_controller;
}

ViewController *Window::controller() const
{
  return d->m_widget_controller;
}

} // namespace PlexyDesk