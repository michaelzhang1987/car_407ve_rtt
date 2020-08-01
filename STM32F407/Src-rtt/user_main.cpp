#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdlib.h>
#include "rtt_interface.h"
#include "AC_Base.h"
#include "Logger.h"
#include "AP_Buffer.h"
#include "mode.h"
#if defined(__ICCARM__) || defined(__GNUC__)
#include "AP_RangeFinder.h"
#include "AP_KF.h"
#endif

using namespace rtthread;

#if defined(__ICCARM__) || defined(__GNUC__)
static rt_timer_t vl53lxx_timer;
#endif

extern rt_device_t vcom;

AC_Base      *base;
AP_Buffer    *buffer;
Mode         *car_mode;
ModeManual   *mode_manual;
ModeAuto     *mode_auto;
ModeROS      *mode_ros;

Mode::Number     current_mode = Mode::Number::MAN;
Mode::Number     prev_mode    = Mode::Number::MAN;
Mode::ModeReason mode_reason  = Mode::ModeReason::UNKNOWN;

#if defined(__ICCARM__) || defined(__GNUC__)
RangeFinder *range_finder;
AP_KF *kalman_filter;
#endif

extern "C" {
#if defined(__ICCARM__) || defined(__GNUC__)
static int sensor_timer_create();
static void vl53lxx_timeout(void *parameter);
#endif

void setup(void)
{
  base        = new AC_Base(AC_Base::Type::MECANUM_4WD);
  buffer      = new AP_Buffer();
  mode_manual = new ModeManual();
  mode_auto   = new ModeAuto();
  mode_ros    = new ModeROS();
  car_mode    = mode_manual;
  
  base->init();
  buffer->init(AP_Buffer::RING);
#if defined(__ICCARM__) || defined(__GNUC__)
  range_finder =  new RangeFinder();
  range_finder->init(RangeFinder::Type::VL53L0X);
  sensor_timer_create();
  kalman_filter = new AP_KF();
  kalman_filter->set_var(1e-2, 6.75);
#endif
  //Log_Init();
}

void loop(void* parameter)
{  
  while(1)
  {
    update_mode();
  
    rt_thread_delay(50);
  }
}

#if defined(__ICCARM__) || defined(__GNUC__)
static int sensor_timer_create()
{
  RTT_TIMER_CREATE(vl53lxx,vl53lxx_timeout,RT_NULL,33,RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_CTRL_SET_PERIODIC);
  return 0;
}

static void vl53lxx_timeout(void *parameter)
{
  // read vl53lxx
  range_finder->timer(parameter);
  
  // updae data
  range_finder->update();
  
  //char buf[100];
  //sprintf(buf, "dist_cm: %d\r\n", range_finder->distance_cm());
  //rt_kputs(buf);
  
  //Write_RangeFinder(range_finder->distance_cm());
}
#endif //#if defined(__ICCARM__) || defined(__GNUC__)
} // extern "C"
