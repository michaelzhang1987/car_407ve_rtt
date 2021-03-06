#ifndef __AP_KF_H__
#define __AP_KF_H__
#include <matrixN.h>
#include <LowPassFilter.h>

typedef VectorN<float,4> _Vector4f;
typedef MatrixN<float,4> _Matrix4f;

class AP_KF
{
public:
  AP_KF();
  ~AP_KF();
  
  void      set_dt(const float &dt);
  void      set_var(const float &var_acc, const float &var_gyro);
  _Vector4f run(const Vector2f &att, const Vector2f &gyro);
  
  Vector2f  get_att_var(void)  { return _att_var; }
  Vector2f  get_gyro_var(void) { return _gyro_var; }

private:  
  float _dt;
  float _var_att_init[2];  // 0-roll, 1-pitch
  float _var_gyro_init[2]; // 0-roll, 1-pitch

  Vector2f  _att_var;
  Vector2f  _gyro_var;
  _Vector4f _state_estimate;
  
  LowPassFilterVector2f  _att_flt_1{5.0f},  _att_flt_2{3.0f};
  LowPassFilterVector2f _gyro_flt_1{2.0f}, _gyro_flt_2{2.0f};
};
#endif /* __AP_KF_H__ */