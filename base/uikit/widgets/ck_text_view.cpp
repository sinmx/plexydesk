#include "ck_text_view.h"

#include <QTextLayout>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace cherry_kit {

class font_metrics {
   public:
    font_metrics() {
    }

    ~font_metrics() {}

    static int font_width() {
      QFont current_font;
      QFontMetrics metric(current_font);
      return metric.width('_');
    }

    static int font_width(char a_letter) {
      QFont current_font;
      QFontMetrics metric(current_font);
      return metric.width(a_letter);
    }


    static int font_height() {
      QFont current_font;
      QFontMetrics metric(current_font);
      return metric.height();
    }
};

class document {
public:
    document() {}

    ~document() {}

    void set_text(const std::string &a_data) {
        m_data = a_data;
    }

    std::string text() const{
       return m_data;
    }

    void parse() {
        std::string    line;
        std::istringstream stream(m_data);
        m_line_vector.clear();
        while(std::getline(stream, line)) {
            m_line_vector.push_back(line);
        }
    }

    void parse_async() {
    }

    std::vector<std::string> lines() {
       return m_line_vector;
    }

    void insert(int x, int y, const std::string &a_char);

    int char_at(int pos) {
        return m_data.at(pos);
    }

private:
    std::string m_data;
    std::vector<std::string> m_line_vector;
};

class text_view::text_view_context {
public:
    text_view_context() {
        m_font_height = font_metrics::font_height();
        m_font_width = font_metrics::font_width();
        m_caret_pos = 0;
    }
    ~text_view_context() {}

    int layout(QPainter *p, const std::__cxx11::string &data, int a_height);
    void insert_char(const std::string &a_char);

    std::string m_text;
    font_metrics *m_engine;

    /* the visual caret pos */
    QPoint m_text_cursor_pos;
    int m_caret_pos;
    QTextLayout *m_layout_mgr;

    int m_font_height;
    int m_font_width;

    document m_doc;
};

text_view::text_view(widget *a_parent) : widget(a_parent),
    ctx(new text_view_context) {
    ctx->m_engine = new font_metrics();
    ctx->m_layout_mgr = new QTextLayout();
}

text_view::~text_view(){
    delete ctx;
}

void text_view::set_text(const std::__cxx11::string &a_text)
{
   ctx->m_text = a_text;
   ctx->m_doc.set_text(a_text);
   ctx->m_doc.parse();

   std::vector<std::string> line = ctx->m_doc.lines();

   ctx->m_text_cursor_pos.setX(
               ctx->m_font_width * line.at(line.size() - 1).length() + 1);

   ctx->m_text_cursor_pos.setY(line.size() - 1);

   update();
}

void text_view::paint_view(QPainter *a_painter, const QRectF &a_rect) {
    a_painter->fillRect(a_rect, Qt::white);
    /* use Qt */

    a_painter->save();
    a_painter->setClipRect(a_rect);
    a_painter->setRenderHint(QPainter::TextAntialiasing, true);
    a_painter->setRenderHint(QPainter::Antialiasing, true);
    a_painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    std::vector<std::string> line_data = ctx->m_doc.lines();
    int line_num = 0;

    std::for_each(std::begin(line_data),
                  std::end(line_data),
                  [&](std::string &data) {
        line_num = ctx->layout(a_painter, data, line_num);
    });

    a_painter->drawRect(QRect(ctx->m_text_cursor_pos.x(),
                              ctx->m_text_cursor_pos.y() +2,
                              2,
                              font_metrics::font_height() - 4));
    /*
    ctx->m_layout_mgr->drawCursor(a_painter,
                                  ctx->m_text_cursor_pos,
                                  0,
                                  1);
                                  */
    a_painter->restore();

}

void text_view::keyPressEvent(QKeyEvent *a_event_ptr)
{
  if (a_event_ptr->key() == Qt::Key_Left) {
     ctx->m_text_cursor_pos.setX(
                 ctx->m_text_cursor_pos.x() - ctx->m_font_width);
     ctx->m_caret_pos--;
     update();
     return;
  }

  if (a_event_ptr->key() == Qt::Key_Right) {
     ctx->m_text_cursor_pos.setX(
                 ctx->m_text_cursor_pos.x() + ctx->m_font_width);
     ctx->m_caret_pos++;
     update();
     return;
  }

  if (a_event_ptr->key() == Qt::Key_Up) {
     ctx->m_text_cursor_pos.setY(
                 ctx->m_text_cursor_pos.y() - ctx->m_font_height);
     update();
     return;
  }

  if (a_event_ptr->key() == Qt::Key_Down) {
     ctx->m_text_cursor_pos.setY(
                 ctx->m_text_cursor_pos.y() + ctx->m_font_height);
     update();
     return;
  }

  ctx->insert_char(a_event_ptr->text().toStdString());

  int move_by = font_metrics::font_width(a_event_ptr->text().at(0).toLatin1());
  ctx->m_text_cursor_pos.setX(
                 ctx->m_text_cursor_pos.x() + move_by);

  ctx->m_caret_pos++;
  update();
}

int text_view::text_view_context::layout(QPainter *p,
                                         const std::string &data,
                                         int a_height)
{
   QFont current_font;
   QFontMetrics fontMetrics(current_font);
   int leading = fontMetrics.leading();
   qreal height = a_height;

   m_layout_mgr->clearLayout();
   m_layout_mgr->clearAdditionalFormats();
   m_layout_mgr->clearFormats();
   m_layout_mgr->setText(data.c_str());
   m_layout_mgr->beginLayout();

   while (1) {
        QTextLine line = m_layout_mgr->createLine();

        if (!line.isValid()) {
            break;
        }

        line.setNumColumns(100);
        height += leading;
        line.setPosition(QPointF(0, height));
        height += line.height();
   }

   m_layout_mgr->endLayout();

   m_layout_mgr->draw(p, QPoint(0, leading));
   return height;
}

void text_view::text_view_context::insert_char(const std::string &a_char)
{
  int line = m_text_cursor_pos.y();
  int column = m_text_cursor_pos.x();

  m_doc.insert(line, m_caret_pos, a_char);
  m_doc.parse();
}

void document::insert(int x, int y, const std::string &a_char) {
  m_data.insert(y + 1, a_char);
}

}
