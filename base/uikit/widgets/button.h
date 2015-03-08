#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <plexy.h>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsLayoutItem>

#include <functional>

#include <widget.h>
#include <widget.h>
#include <style.h>
#include <plexydesk_ui_exports.h>

namespace UIKit
{
class DECL_UI_KIT_EXPORT Button : public Widget
{
  Q_OBJECT

public:
  explicit Button(QGraphicsObject *a_parent_ptr = 0);
  virtual ~Button();

  virtual void setLabel(const QString &txt);
  virtual QString label() const;

  virtual void setIcon(const QImage &img);

  virtual StylePtr style() const;

  virtual void setSize(const QSize &size);
  virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

  void setActionData(const QVariant &data);
  QVariant actionData() const;

  virtual void onButtonPressed(std::function<void ()> aHandler);
protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  virtual void paint_view(QPainter *painter, const QRectF &rect);
  virtual void paintNormalButton(QPainter *painter, const QRectF &rect);
  virtual void paintSunkenButton(QPainter *painter, const QRectF &rect);
private:
  class PrivateButton;
  PrivateButton *const d;
};
}
#endif // UI_BUTTON_H
