#include "ck_button.h"

#include <ck_widget.h>

#include <QGraphicsSceneEvent>
#include <QStyleOptionGraphicsItem>

#include <ck_extension_manager.h>
#include <ck_resource_manager.h>

namespace cherry_kit {

class button::PrivateButton {
public:
  typedef enum {
    NORMAL,
    PRESS,
    HOVER
  } ButtonState;

  PrivateButton() {}
  ~PrivateButton() {}

  QSizeF m_button_size;
  ButtonState mState;
  QString mLabel;
  QVariant mData;
};

button::button(widget *a_parent_ptr)
    : widget(a_parent_ptr), o_button(new PrivateButton) {
  o_button->mState = PrivateButton::NORMAL;

  if (resource_manager::style()) {
    set_size(QSize(
        resource_manager::style()->attribute("button", "width").toFloat(),
        resource_manager::style()->attribute("button", "height").toFloat()));
  }

  setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
  setFlag(QGraphicsItem::ItemIsMovable, false);
  setFlag(QGraphicsItem::ItemIsFocusable, true);
  setFiltersChildEvents(true);
  setAcceptHoverEvents(true);
}

button::~button() { delete o_button; }

void button::set_label(const QString &a_txt) {
  o_button->mLabel = a_txt;
  update();
}

void button::mousePressEvent(QGraphicsSceneMouseEvent *a_event_ptr) {
  o_button->mState = PrivateButton::PRESS;
  update();
  widget::mousePressEvent(a_event_ptr);
}

void button::mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event_ptr) {
  o_button->mState = PrivateButton::NORMAL;
  update();

  widget::mouseReleaseEvent(a_event_ptr);
}

void button::hoverEnterEvent(QGraphicsSceneHoverEvent *a_event_ptr) {
  o_button->mState = PrivateButton::HOVER;
  update();

  widget::hoverEnterEvent(a_event_ptr);
}

void button::hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event_ptr) {
  o_button->mState = PrivateButton::NORMAL;
  update();

  widget::hoverLeaveEvent(a_event_ptr);
}

void button::paint_normal_button(QPainter *a_painter_ptr,
                                 const QRectF &a_rect) {
  style_data feature;

  feature.text_data = o_button->mLabel;
  feature.geometry = a_rect;
  feature.render_state = style_data::kRenderElement;

  if (cherry_kit::resource_manager::style()) {
    cherry_kit::resource_manager::style()->draw("button", feature,
                                                a_painter_ptr);
  }
}

void button::paint_sunken_button(QPainter *painter, const QRectF &a_rect) {
  style_data feature;

  feature.text_data = o_button->mLabel;
  feature.geometry = a_rect;
  feature.render_state = style_data::kRenderPressed;

  if (cherry_kit::resource_manager::style()) {
    cherry_kit::resource_manager::style()->draw("button", feature, painter);
  }
}

void button::paint_hover_button(QPainter *a_painter, const QRectF &a_rect) {
  style_data feature;

  feature.text_data = o_button->mLabel;
  feature.geometry = a_rect;
  feature.render_state = style_data::kRenderRaised;

  if (cherry_kit::resource_manager::style()) {
    cherry_kit::resource_manager::style()->draw("button", feature, a_painter);
  }
}

StylePtr button::style() const { return cherry_kit::resource_manager::style(); }

void button::set_size(const QSizeF &a_size) {
  prepareGeometryChange();
  o_button->m_button_size = a_size;
  setMinimumSize(a_size);
  update();
}

QSizeF button::sizeHint(Qt::SizeHint which, const QSizeF &a_constraint) const {
  return boundingRect().size();
}

QRectF button::boundingRect() const {
  return QRectF(0, 0, o_button->m_button_size.width(),
                o_button->m_button_size.height());
}

void button::setGeometry(const QRectF &a_rect) { setPos(a_rect.topLeft()); }

void button::set_action_data(const QVariant &a_data) {
  o_button->mData = a_data;
}

QVariant button::action_data() const { return o_button->mData; }

void button::on_button_pressed(std::function<void()> a_handler) {}

void button::paint_view(QPainter *a_painter_ptr, const QRectF &a_rect) {
  switch (o_button->mState) {
  case PrivateButton::NORMAL:
    paint_normal_button(a_painter_ptr, a_rect);
    break;
  case PrivateButton::PRESS:
    paint_sunken_button(a_painter_ptr, a_rect);
    break;
  case PrivateButton::HOVER:
    paint_hover_button(a_painter_ptr, a_rect);
  default:
    qDebug() << Q_FUNC_INFO << "Unknown Button State";
  }
}

void button::setIcon(const QImage & /*img*/) {}

QString button::text() const { return o_button->mLabel; }
}