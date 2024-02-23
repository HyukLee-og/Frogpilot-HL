#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_3585317521272958006);
void live_err_fun(double *nom_x, double *delta_x, double *out_5894257164096913053);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5254837349202704713);
void live_H_mod_fun(double *state, double *out_329589871290340028);
void live_f_fun(double *state, double dt, double *out_5591562348585879218);
void live_F_fun(double *state, double dt, double *out_8977838800168055418);
void live_h_4(double *state, double *unused, double *out_6568051884677364030);
void live_H_4(double *state, double *unused, double *out_600614122843459950);
void live_h_9(double *state, double *unused, double *out_7986763754593148191);
void live_H_9(double *state, double *unused, double *out_2288247429436619392);
void live_h_10(double *state, double *unused, double *out_1696844284174150102);
void live_H_10(double *state, double *unused, double *out_8296381704530721339);
void live_h_12(double *state, double *unused, double *out_7690125021142739883);
void live_H_12(double *state, double *unused, double *out_7066514190838990542);
void live_h_35(double *state, double *unused, double *out_5360877677096077428);
void live_H_35(double *state, double *unused, double *out_2766047934529147426);
void live_h_32(double *state, double *unused, double *out_3283961989127718109);
void live_H_32(double *state, double *unused, double *out_2897212719469887895);
void live_h_13(double *state, double *unused, double *out_4973029580970525387);
void live_H_13(double *state, double *unused, double *out_996979337112772560);
void live_h_14(double *state, double *unused, double *out_7986763754593148191);
void live_H_14(double *state, double *unused, double *out_2288247429436619392);
void live_h_33(double *state, double *unused, double *out_5581355254746335032);
void live_H_33(double *state, double *unused, double *out_5484109845906689761);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}