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
#ifndef PD_CORE_WIDGET_H
#define PD_CORE_WIDGET_H

#include <plexy.h>
#include <style.h>
#include <plexydesk_ui_exports.h>

#include <QGraphicsLayoutItem>
#include <QGraphicsObject>

namespace UI
{

class ViewController;
/**
    * @brief The Base Class for All types of Desktop Widgets.
    */
/**
    * @brief
    *
    */
class DECL_UI_KIT_EXPORT Window : public QGraphicsObject,
  public QGraphicsLayoutItem
{
  Q_OBJECT

  Q_ENUMS(RenderLevel)
  Q_ENUMS(WidgetFlags)
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

  Q_INTERFACES(QGraphicsLayoutItem)
public:
  typedef enum {
    kRenderBackground = 1ul << 0,
    kRenderDropShadow = 1ul << 1,
    kDockWindowType = 1ul << 2,
    kTopLevelWindow = 1ul << 3,
    kConvertToWindowType = 1ul << 4,
    kRenderWindowTitle = 1ul << 5
  } WidgetFlags;

  /**
      * @brief
      *
      */
  enum RenderLevel { kRenderAtBackgroundLevel, kRenderAtForgroundLevel };
  /**
      * @brief Destructor
      */
  /**
      * @brief
      *
      */
  virtual ~Window();
  /**
      * @brief Returns the bounding Area of the Widget
      *
      * @return Bounding rectangle as a QRectF
      */
  virtual QRectF boundingRect() const;

  virtual void setWindowFlag(int flags, bool enable = true);
  /**
       * @brief
       *
       * @param rect
       */
  virtual void setMinimizedGeometry(const QRectF &rect);
  /**
      * @brief
      *
      * @return QRectF
      */
  virtual QRectF minimizedGeometry() const;
  /**
      * @brief
      *
      * @param view_controller
      */
  virtual void setController(ViewController *view_controller);
  /**
      * @brief
      *
      * @return ControllerInterface
      */
  virtual ViewController *controller() const;
  /**
      * @brief
      *
      * @param name
      */
  virtual void setLabelName(const QString &name);

  /**
      * @brief
      *
      * @return QString
      */
  virtual QString label() const;
  /**
      * @brief
      *
      * @return QString
      */
  virtual QString uuid() const;
  /**
      * @brief
      *
      * @param style
      */
  virtual StylePtr style() const = 0;
  /**
      * @brief
      *
      * @return Layer
      */
  virtual RenderLevel layerType() const;
  /**
      * @brief
      *
      * @param type
      */
  virtual void setLayerType(RenderLevel level) const;

  void setGeometry(const QRectF &rect);

Q_SIGNALS:
  /**
      * @brief
      *
      * @param widget
      */
  void closed(UI::Window *widget);
  /**
      * @brief
      *
      */
  void rectChanged();
  /**
      * @brief
      *
      */
  void stateChanged();
  /**
      * @brief
      *
      * @param pos
      * @param widgetId
      */
  void itemMoved(const QPointF &pos, const QString &widgetId);
  /**
      * @brief
      *
      */
  void clicked();

  void focusLost();

protected:
  /**
      * @brief
      *
      * @param rect
      * @param parent
      */
  Window(QGraphicsObject *parent = 0);
  /**
      * @brief
      *
      * @param painter
      * @param option
      * @param widget
      */
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget = 0);

  virtual QSizeF sizeHint(Qt::SizeHint which,
                          const QSizeF &constraint = QSizeF()) const;
  /**
      * @brief
      *
      * @param painter
      * @param rect
      */
  virtual void paintView(QPainter *painter, const QRectF &rect);
  /**
    * @brief
    *
    * @param event
    */
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  /**
      * @brief
      *
      * @param event
      */
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  /**
      * @brief
      *
      * @return float
      */
  virtual float scaleFactorForWidth() const;
  /**
      * @brief
      *
      * @return float
      */
  virtual float scaleFactorForHeight() const;
  /**
  * @brief Hide or show all the child Widgets
  *
  * method used to control the visibility of the child widgets. this is
  * useful when
  * heavy animation or scaling has to be done on the parent widget.
  * for instance the dock animation.
  *
  * @param show if true it will show the child widets, and hide if false.
  */
  virtual void setChildWidetVisibility(bool show);

  virtual void focusOutEvent(QFocusEvent *event);

private:
  class PrivateAbstractDesktopWidget;
  PrivateAbstractDesktopWidget *const d;
};
} //

#endif