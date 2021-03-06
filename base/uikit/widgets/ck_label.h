#ifndef LABEL_H
#define LABEL_H

#include <ck_widget.h>

#include <QGraphicsObject>
#include <QGraphicsLayoutItem>

#include <ck_style.h>
#include <plexydesk_ui_exports.h>

namespace cherry_kit {

class DECL_UI_KIT_EXPORT label : public widget {
public:
  explicit label(widget *a_parent_ptr = 0);
  virtual ~label();

  virtual void set_text(const QString &a_txt);
  virtual QString text() const;

  virtual void set_size(const QSizeF &_asize);
  virtual void set_font_size(uint a_pixelSize);

  /*
  virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &a_constraint) const;
  virtual QRectF boundingRect() const;
  virtual void set_geometry(const QRectF &a_rect);
  virtual QRectF contents_bounding_rect() const;
  */

  virtual void set_label_style(const QColor &a_backgroundColor,
                               const QColor &a_textColor);
  virtual void set_highlight(bool a_mode = false);

  virtual int alignment();
  virtual void set_alignment(int a_alignment);

protected:
  virtual void paint_view(QPainter *a_painter_ptr, const QRectF &a_rect);

private:
  class PrivateLabel;
  PrivateLabel *const o_label;
};
}

#endif // LABEL_H
