#ifndef TIME_SEGMENT_H
#define TIME_SEGMENT_H

#include "time_event.h"
#include <config.h>
#include <ck_window.h>

#include <ck_desktop_controller_interface.h>

class date_controller;

typedef std::function<void (cherry_kit::window *)> new_event_callback_t;

class time_segment : public cherry_kit::widget {
public:
  typedef enum {
    kAMTime,
    kPMTime,
    kNoonTime
  } segment_t;
  time_segment(date_controller *a_controller,
               cherry_kit::session_sync *a_session,
               cherry_kit::widget *a_parent_ref = 0)
      : m_controller(a_controller), m_session(a_session),
        cherry_kit::widget(a_parent_ref), m_heighlight(0), m_duration(0) {}
  virtual ~time_segment() {}

  void set_time_value(int time_value);
  int time_value() const;

  segment_t time_type() const;
  void set_time_type(const segment_t &time_type);

  bool heighlight() const;
  void set_heighlight(bool a_is_enabled);

  void add_event(const time_event &a_event);
  event_list_t events() const;

  void create_new(new_event_callback_t a_callback);

protected:
  virtual void paint_view(QPainter *a_ctx, const QRectF &a_rect);

private:
  int m_time_value;
  int m_duration;
  segment_t m_time_type;
  bool m_heighlight;

  event_list_t m_events;

  cherry_kit::session_sync *m_session;
  date_controller *m_controller;
};

typedef std::vector<time_segment *> time_segment_list_t;


#endif // TIME_SEGMENT_H